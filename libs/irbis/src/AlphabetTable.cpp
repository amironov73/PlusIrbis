// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

// ReSharper disable StringLiteralTypo

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

/*!
    \file AlphabetTable.cpp

    \class irbis::AlphabetTable

    Таблица алфавитных символов используется системой ИРБИС
    при разбиении текста на слова и представляет собой список
    кодов символов, которые считаются алфавитными.
    Таблица реализована в виде текстового файла.

    Местонахождение и имя файла по умолчанию:
    `<IRBIS_SERVER_ROOT>\ISISACW.TAB`.

    Местонахождение и имя файла определяется значением
    параметра ACTABPATH в конфигурационном файле
    ИРБИС и может быть изменено.

    Стандартное содержимое

    ```
    038 064 065 066 067 068 069 070 071 072 073 074 075 076 077 078 079 080 081 082 083 084 085 086 087 088 089 090 097 098 099 100
    101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 128 129 130 131 132 133 134 135 136 137
    138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169
    170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201
    202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233
    234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255
    ```

 */


namespace irbis {

/// \brief Стандартный файл с алфавитной таблицей.
const String AlphabetTable::FileName = L"ISISACW.TAB";

/// \brief Настройка таблицы на указанный набор байтов.
/// \param bytes Байты, считающиеся входящими в слова.
void AlphabetTable::setup (const Bytes &bytes)
{
    const auto encoding = Encoding::ansi();
    const auto converted = encoding->toUnicode(bytes.data(), bytes.size());
    for (const auto byte : converted) {
        this->characters.insert (byte);
    }
}

static Byte _bytes[] =
{
     38,  64,  65,  66,  67,  68,  69,  70,  71,  72,
     73,  74,  75,  76,  77,  78,  79,  80,  81,  82,
     83,  84,  85,  86,  87,  88,  89,  90,  97,  98,
     99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
    109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
    119, 120, 121, 122, 128, 129, 130, 131, 132, 133,
    134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
    154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
    164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
    174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
    184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
    194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
    204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
    214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
    234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
    244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
    254, 255
};

static AlphabetTable *_instance;

/// \brief Общий глобальный экземпляр.
/// \return Экземпляр.
const AlphabetTable& AlphabetTable::instance()
{
    if (_instance == nullptr) {
        _instance = new AlphabetTable();
        Bytes bytes;
        bytes.insert (std::end (bytes), _bytes, _bytes + sizeof (_bytes));
        _instance->setup (bytes);
    }
    return *_instance;
}

/// \brief Проверка, является ли указанный символ буквой.
/// \param c Символ.
/// \return `true`, если является.
bool AlphabetTable::isAlpha (const Char &c) const
{
    return this->characters.find (c) != std::end (this->characters);
}

/// \brief Разбор ответа сервера или прочитанного файла.
/// \param lines Вектор строк.
void AlphabetTable::parse (const StringList &lines)
{
    Bytes bytes;
    for (const auto &line : lines) {
        if (!line.empty()) {
            const auto parts = split (line, L' ');
            for (const auto &part : parts) {
                if (!part.empty()) {
                    const auto byte = fastParse32 (part);
                    if (byte > 0 && byte < 256) {
                        bytes.push_back ((Byte)byte);
                    }
                }
            }
        }
    }
    this->setup (bytes);
}

/// \brief Чтение локального файла.
/// \param fileName Имя файла.
/// \return Таблица.
AlphabetTable AlphabetTable::readLocalFile (const String &fileName)
{
    const auto lines = Text::readAnsiLines (fileName);
    AlphabetTable result;
    result.parse (lines);
    return result;
}

/// \brief Удаление пробелов в начале и в конце строки.
/// \param text Текст для обработки.
/// \return Текст с удалёнными пробелами.
String AlphabetTable::trimText (const String &text) const
{
    if (text.empty()) {
        return text;
    }

    if (isAlpha(text.at (0)) && isAlpha (text.at (text.size() - 1))) {
        return text;
    }

    String result = text;
    while (result.length() && !isAlpha (result.at (0))) {
        result = result.substr (1);
    }

    while (result.length() && !isAlpha (result.at (result.size() - 1))) {
        result = result.substr (0, result.size() - 1);
    }

    return result;
}

/// \brief Разбиение текста на слова.
/// \param text Текст для обработки
/// \return
StringList AlphabetTable::splitWords (const String &text) const
{
    StringList result;

    if (text.empty()) {
        return result;
    }

    String accumulator;
    for (const Char &c : text) {
        if (this->isAlpha (c)) {
            accumulator.push_back (c);
        } else {
            if (!accumulator.empty()) {
                result.push_back (accumulator);
                accumulator.clear();
            }
        }
    }

    if (!accumulator.empty()) {
        result.push_back (accumulator);
    }

    return result;
}

/// \brief Верификация.
/// \param throwOnError Бросать исключение при обнаружении ошибки?
/// \return Результат верификации.
bool AlphabetTable::verify (bool throwOnError) const
{
    bool result = !characters.empty();
    if (!result && throwOnError) {
        throw VerificationException();
    }

    return result;
}

}
