// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_direct.h"
#include "irbis_private.h"

#include <cstring>
#include <sys/stat.h>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

// ReSharper disable CommentTypo

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

static const int LengthOfLength = 5;

/// \brief Чтение записей в формате ISO 2709
/// \param device Файл.
/// \param encoding Кодировка
/// \return `nullptr` если прочитать не удалось.
MarcRecord* Iso2709::readRecord (File *device, const Encoding *encoding)
{
    // Считываем длину записи
    Byte marker [LengthOfLength];
    if (device->read (marker, LengthOfLength) != LengthOfLength) {
        return nullptr;
    }

    // а затем и ее остаток
    const auto recordLength = static_cast<const std::size_t> (fastParse32 (reinterpret_cast<const char*> (marker), LengthOfLength));
    const bool useHeap = recordLength >= 4096;
    Byte *record = useHeap ? new Byte [recordLength] : static_cast<Byte*> (alloca (recordLength));
    PointerGuard<Byte> guard (record, useHeap);
    const auto need = recordLength - LengthOfLength;
    if (device->read (record + LengthOfLength, need) != need) {
        return nullptr;
    }

    // Простая проверка, что мы имеем дело
    // с нормальной ISO-записью
    if (record [recordLength - 1] != RecordDelimiter) {
        return nullptr;
    }

    const auto baseAddress     = static_cast<std::size_t> (fastParse32 (reinterpret_cast<const char*> (record + 12), 5));
    const auto indicatorLength = static_cast<std::size_t> (fastParse32 (reinterpret_cast<const char*> (record + 10), 1)); // как правило, 2
    auto *result = new MarcRecord {};

    // Пошли по полям при помощи справочника
    for (std::size_t directory = MarkerLength;; directory += 12) {
        // Переходим к очередному полю.
        // Если нарвались на разделитель, значит, справочник закончился
        if (record [directory] == FieldDelimiter) {
            break;
        }

        auto tag = fastParse32 (reinterpret_cast<const char*> (record + directory), 3);
        const auto fieldLength = static_cast<const std::size_t> (fastParse32 (reinterpret_cast<const char*> (record + directory + 3), 4));
        const std::size_t fieldOffset = baseAddress + fastParse32 (reinterpret_cast<const char*> (record + directory + 7), 5);
        RecordField field;
        field.tag = tag;
        result->fields.push_back (field);
        if (tag < 10) {
            // Фиксированное поле
            // не может содержать подполей и индикаторов
            field.value = encoding->toUnicode(record + fieldOffset, fieldLength - 1);
        } else {
            // Поле переменной длины
            // Содержит два однобайтных индикатора
            // Может содержать подполя

            // Пропускаем индикаторы
            std::size_t start = fieldOffset + indicatorLength;
            const std::size_t stop = fieldOffset + fieldLength - indicatorLength + 1;
            std::size_t position = start;

            // Ищем значение поля до первого разделителя
            while (position < stop) {
                if (record [start] == SubFieldDelimiter) {
                    break;
                }
                position++;
            }

            // Если есть текст до первого разделителя, запоминаем его
            if (position != start) {
                field.value = encoding->toUnicode (record + start, position - start);
            }

            // Просматриваем подполя
            start = position;
            while (start < stop) {
                position = start + 1;
                while (position < stop) {
                    if (record [position] == SubFieldDelimiter) {
                        break;
                    }
                    position++;
                }
                SubField subField;
                subField.code = record [start + 1];
                subField.value = encoding->toUnicode (record + start + 2, position - start - 1);
                field.subfields.push_back (subField);
                start = position;
            }
        }
    }

    return result;
}

static void encode (Byte *bytes, std::size_t pos, std::size_t len, std::size_t val) noexcept
{
    len--;
    for (pos += len;; len--) {
        bytes[pos] = static_cast<Byte> (val % 10 + '0');
        if (len == 0) {
            break;
        }
        val /= 10;
        pos--;
    }
}

static std::size_t encode (Byte *bytes, std::size_t pos, const String &str, const Encoding *encoding)
{
    if (!str.empty()) {
        auto encoded = encoding->fromUnicode (str);
        for (std::size_t i = 0; i < encoded.size(); pos++, i++) {
            bytes[pos] = encoded[i];
        }
    }

    return pos;
}

static std::size_t countBytes (const String &text, const Encoding *encoding)
{
    if (text.empty()) {
        return 0;
    }
    return encoding->getSize(text);
}

/// \brief Сохранение записи в файл.
/// \param device Файл.
/// \param record Запись.
/// \param encoding Кодировка.
bool Iso2709::writeRecord (File *device, const MarcRecord &record, const Encoding *encoding)
{
    std::size_t recordLength = MarkerLength;
    std::size_t dictionaryLength = 1; // с учетом ограничителя справочника
    std::vector <std::size_t> fieldLength;
    fieldLength.reserve (record.fields.size());

    // Сначала подсчитываем общую длину
    for (const auto &field : record.fields) {
        dictionaryLength += 12; // одна статья справочника

        if (field.tag <= 0 || field.tag >= 1000) {
            return false;
        }

        std::size_t fldlen = 0;
        if (field.tag < 10) {
            // В фиксированном поле не бывает подполей и индикаторов
            fldlen += countBytes (field.value, encoding);
        } else {
            fldlen += 2; // индиакторы
            fldlen += countBytes (field.value, encoding);
            for (const auto &subfield : field.subfields) {
                if (subfield.code <= ' ' || subfield.code >= 127) {
                    return false;
                }

                fldlen += 2; // признак подполя и его код
                fldlen += countBytes(subfield.value, encoding);
            }
        }

        ++fldlen; // разделитель полей

        if (fldlen >= 10000) {
            return false;
        }

        fieldLength.push_back (fldlen);
        recordLength += fldlen;
    }

    recordLength += dictionaryLength; // справочник
    ++recordLength; // разделитель записей

    if (recordLength >= 100000) {
        throw IrbisException();
    }

    // Приступаем к кодированию
    std::size_t dictionaryPosition = MarkerLength;
    const std::size_t baseAddress = MarkerLength + dictionaryLength;
    const bool useHeap = recordLength >= 4096;
    Byte *bytes = useHeap ? new Byte[recordLength] : static_cast<Byte*> (alloca (recordLength));
    PointerGuard<Byte> guard (bytes, useHeap);
    memset (bytes, ' ', recordLength);

    // Маркер записи
    encode(bytes,  0, 5, recordLength);
    encode(bytes, 12, 5, baseAddress);
    bytes[5]  = 'n'; // Record status //-V557
    bytes[6]  = 'a'; // Record type //-V557
    bytes[7]  = 'm'; // Bibligraphical index //-V557
    bytes[8]  = '2'; //-V557
    bytes[10] = '2'; //-V557
    bytes[11] = '2'; //-V557
    bytes[17] = ' '; // Bibliographical level //-V557
    bytes[18] = 'i'; // Cataloging rules //-V557
    bytes[19] = ' '; // Related record //-V557
    bytes[20] = '4'; // Field length //-V557
    bytes[21] = '5'; // Field offset //-V557
    bytes[22] = '0'; //-V557

    // Конец справочника
    bytes[baseAddress - 1] = FieldDelimiter;

    // Проходим по полям
    std::size_t index = 0;
    std::size_t currentAddress = baseAddress;
    for (const auto &field : record.fields) {
        // Справочник
        encode (bytes, dictionaryPosition, 3, field.tag);
        encode (bytes, dictionaryPosition + 3, 4, fieldLength[index++]); //-V112
        encode (bytes, dictionaryPosition + 7, 5, currentAddress - baseAddress);
        dictionaryPosition += 12;

        // Собственно поле
        if (field.tag < 10) {
            // В фиксированных полях не бывает подполей и индикаторов
            currentAddress = encode(bytes, currentAddress, field.value, encoding);
        } else {
            // Индискаторы
            bytes[currentAddress++] = ' ';
            bytes[currentAddress++] = ' ';

            // Значение поля
            currentAddress = encode(bytes, currentAddress, field.value, encoding);

            // Подполя
            for (const auto &subfield : field.subfields) {
                bytes[currentAddress++] = SubFieldDelimiter;
                bytes[currentAddress++] = static_cast<Byte>(subfield.code);
                currentAddress = encode (bytes, currentAddress, subfield.value, encoding);
            }
        }

        // Ограничитель поля
        bytes[currentAddress++] = FieldDelimiter;
    }

    //assert(currentAddress == recordLength - 1);

    // Конец записи
    bytes [recordLength - 1] = RecordDelimiter;

    // Собственно запись в поток
    if (device->write (bytes, recordLength) != recordLength) {
        throw IrbisException();
    }

    return true;
}

}
