// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "micro.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <random>

#pragma ide diagnostic ignored "bugprone-reserved-identifier"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#ifdef _MSC_VER

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>

#pragma comment (lib, "ws2_32.lib")

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define HOSTENT struct hostent
#define closesocket(__x) ::close(__x)

#endif

namespace irbis
{
    class Cp1251Encoding final
        : public Encoding
    {
        public:

        Bytes        fromUnicode (const std::wstring &text)             const override;
        std::wstring toUnicode   (const Byte *bytes, std::size_t count) const override;
        std::size_t  getSize     (const std::wstring &text)             const override;
    };

    class Utf8Encoding final
        : public Encoding
    {
        public:

        Bytes        fromUnicode (const std::wstring &text)             const override;
        std::wstring toUnicode   (const Byte *bytes, std::size_t count) const override;
        std::size_t  getSize     (const std::wstring &text)             const override;
    };

    //================================================================

    /// \brief Сравнение двух символов с точностью до регистра.
    /// \param first Первый символ.
    /// \param second Второй символ.
    /// \return Возвращает true, если символы равны с точностью до регистра,
    /// иначе false.
    bool sameChar
        (
            char first,
            char second
        )
    noexcept {
        return std::toupper(first) == std::toupper(second);
    }

    /// \brief Сравнение двух строк с точностью до регистра символов.
    /// \param first Первая строка.
    /// \param second Вторая строка.
    /// \return Возвращает true, если строки равны с точностью до регистра,
    /// иначе false.
    bool sameString
        (
            const std::string &first,
            const std::string &second
        )
    noexcept {
        if (first.size() != second.size()) {
            return false;
        }

        return std::equal
            (
                first.begin(),
                first.end(),
                second.begin(),
                [](char a, char b) { return std::toupper(a) == std::toupper(b); }
            );
    }

    /// \brief Первый символ строки.
    /// \param text Текст.
    /// \return Первый символ либо 0.
    char firstChar
        (
            const std::string &text
        )
    noexcept {
        return text.empty() ? '\0' : text[0];
    }

    /// \brief Преобразование строки к нижнему регистру.
    /// \param text Текст, подлежащий трансформации.
    /// \return Возвращает преобразованную строку.
    /// \warning Трансформация осуществляется "по месту"!
    std::string toLower
        (
            std::string &text
        )
    noexcept {
        for (auto &i: text) {
            i = (char) std::tolower(i);
        }

        return text;
    }

    /// \brief Преобразование строки к верхнему регистру.
    /// \param text Текст, подлежащий трансформации.
    /// \return Возвращает преобразованную строку.
    /// \warning Трансформация осуществляется "по месту"!
    std::string toUpper
        (
            std::string &text
        )
    noexcept {
        for (auto &i: text) {
            i = (char) std::toupper(i);
        }

        return text;
    }

    /// \brief Содержит ли строка заданную подстроку?
    /// \param text Текст для изучения.
    /// \param fragment Подстрока для поиска.
    /// \return Возвращает true, если подстрока найдена, иначе false.
    bool contains
        (
            const std::string &text,
            const std::string &fragment
        ) {
        return text.find(fragment) != std::string::npos;
    }

    /// \brief Содержит ли строка заданный символ?
    /// \param text Текст для изучения.
    /// \param c Символ для поиска.
    /// \return Возвращает true, если символ найден, иначе false.
    bool contains
        (
            const std::string &text,
            char c
        ) {
        return text.find(c) != std::string::npos;
    }

    /// \brief Заменяет все вхождения одной подстроки на другую.
    /// \param text Текст для обработки.
    /// \param from Заменяемый текст.
    /// \param to Заменяющий текст.
    /// \return Обработанный текст.
    std::string replace
        (
            const std::string &text,
            const std::string &from,
            const std::string &to
        ) {
        auto result = text;
        std::size_t index = 0;
        while (true) {
            index = result.find(from, index);
            if (index == std::string::npos) {
                break;
            }

            result.replace(index, from.length(), to);
            index += to.length();
        }

        return result;
    }

    /// \brief Удаление пробельных символов в начале строки.
    /// \param text Текст для обработки.
    /// \return Обработанный текст.
    std::string trimStart
        (
            const std::string &text
        ) {
        auto length = text.length();
        std::size_t i = 0;
        for (; i < length; ++i) {
            const auto chr = static_cast<unsigned char> (text[i]);
            if (!std::isspace(chr)) {
                break;
            }
        }

        if (i == 0) {
            return text;
        }

        if (i == length) {
            return {};
        }

        return text.substr(i);
    }

    /// \brief Удаление пробельных символов в конце строки.
    /// \param text Текст для обработки.
    /// \return Обработанный текст.
    std::string trimEnd
        (
            const std::string &text
        ) {
        auto length = text.length();
        auto i = static_cast<std::ptrdiff_t > (length - 1);
        for (; i >= 0; --i) {
            const auto chr = static_cast<unsigned char> (text[i]);
            if (!std::isspace(chr)) {
                break;
            }
        }

        if (i == static_cast<std::ptrdiff_t> (length - 1)) {
            return text;
        }

        if (i < 0) {
            return {};
        }

        return text.substr(0, i + 1);
    }

    /// \brief Удаление пробельных символов в начале и в конце строки.
    /// \param text Текст для обработки.
    /// \return Обработанный текст.
    std::string trim
        (
            const std::string &text
        ) {
        const auto length = static_cast<std::ptrdiff_t> (text.length());
        std::ptrdiff_t start = 0, end = static_cast<std::ptrdiff_t> (length - 1);
        for (; start < length; ++start) {
            const auto chr = static_cast<unsigned char> (text[end]);
            if (!std::isspace(chr)) {
                break;
            }
        }

        for (; end >= 0; --end) {
            const auto chr = static_cast<unsigned char> (text[end]);
            if (!std::isspace(chr)) {
                break;
            }
        }

        if (end - start < 0) {
            return {};
        }

        if (end - start == length - 1) {
            return text;
        }

        return text.substr(start, end - start + 1);
    }


    /// \brief Быстрый и грязный разбор строки как целого числа без знака.
    /// \param text Текст для разбора.
    /// \return Мусор на входе - мусор на выходе!
    int fastParse32
        (
            const std::string &text
        )
    noexcept {
        // TODO обрабатывать знак числа

        auto result = 0;
        const std::size_t length = text.length();
        for (std::size_t offset = 0; offset < length; offset++) {
            result = result * 10 + text.at(offset) - '0';
        }

        return result;
    }

    /// \brief Быстрый и грязный разбор строки как целого числа без знака.
    /// \param text Текст для разбора.
    /// \return Мусор на входе - мусор на выходе!
    unsigned int fastParseUnsigned32
        (
            const std::string &text
        )
    noexcept {
        auto result = 0u;
        const std::size_t length = text.length();
        for (std::size_t offset = 0; offset < length; offset++) {
            result = result * 10 + text.at(offset) - '0';
        }

        return result;
    }

    /// \brief Быстрый и грязный разбор строки как целого числа без знака.
    /// \param text Текст для разбора.
    /// \return Мусор на входе - мусор на выходе!
    int fastParse32
        (
            const char *text
        )
    noexcept {
        // TODO обрабатывать знак числа

        auto result = 0;
        while (*text != 0) {
            const auto chr = static_cast<unsigned char> (*text);
            result = result * 10 + chr - '0';
            text++;
        }

        return result;
    }

    /// \brief Быстрый и грязный разбор строки как целого числа без знака.
    /// \param text Текст для разбора.
    /// \return Мусор на входе - мусор на выходе!
    unsigned int fastParseUnsigned32
        (
            const char *text
        )
    noexcept {
        auto result = 0u;
        while (*text != 0) {
            const auto chr = static_cast<unsigned char> (*text);
            result = result * 10 + chr - '0';
            text++;
        }

        return result;
    }

    /// \brief Быстрый и грязный разбор строки как целого числа без знака.
    /// \param text Текст для разбора.
    /// \return Мусор на входе - мусор на выходе!
    int fastParse32(const char *text, std::size_t length) noexcept {
        auto result = 0;
        while (length > 0) {
            result = result * 10 + *text - '0';
            text++;
            length--;
        }

        return result;
    }

    /// \brief Быстрый и грязный разбор строки как целого числа без знака.
    /// \param text Текст для разбора.
    /// \return Мусор на входе - мусор на выходе!
    unsigned int fastParseUnsigned32(const char *text, std::size_t length) noexcept {
        auto result = 0u;
        while (length > 0) {
            result = result * 10 + *text - '0';
            text++;
            length--;
        }

        return result;
    }

    /// \brief Разбор строки по указанному разделителю.
    /// \param text Текст для разбора.
    /// \param separator Разделитель.
    /// \param count Не больше указанного количества подстрок.
    /// \return Вектор подстрок.
    StringList maxSplit(const std::string &text, char separator, int count) {
        StringList result;
        std::size_t position = 0;
        const std::size_t length = text.length();
        while ((count > 1) && (position < length)) {
            const auto index = text.find(separator, position);
            if (index != std::wstring::npos) {
                result.push_back(text.substr(position, index - position));
                position = index + 1;
            } else {
                result.push_back(text.substr(position));
                position = length + 1;
                break;
            }
            count--;
        }
        if (position < length) {
            result.push_back(text.substr(position));
        }
        return result;
    }

    /// \brief Разбор строки по указанному разделителю.
    /// \param text Текст для разбора.
    /// \param delimiter Разделитель.
    /// \return Вектор подстрок.
    StringList split(const std::string &text, char delimiter) {
        std::vector<std::string> result;
        std::string token;
        std::istringstream stream(text);
        while (std::getline(stream, token, delimiter)) {
            result.push_back(token);
        }

        return result;
    }

    /// \brief Разбиение на строки по разделителю.
    /// \param text Текст, подлежащий разбиению.
    /// \param delimiter Разделитель.
    /// \return Вектор строк. Включает, кроме прочего, пустые строки.
    std::vector<std::string> split(const std::string &text, const std::string &delimiter) {
        std::vector<std::string> result;
        std::string token;
        std::size_t start = 0, end;
        while ((end = text.find(delimiter, start)) != std::string::npos) {
            token = text.substr(start, end - start);
            result.push_back(token);
            start = end + delimiter.length();
        }

        if (start < text.length()) {
            token = text.substr(start, std::string::npos);
            result.push_back(token);
        }

        return result;
    }

    //================================================================

    /// \brief Модифицирована ли данная строка?
    /// \return `true` если строка модифицирована.
    ///
    /// Состояние "строка модифицирована" устанавливается
    /// при вызове `setKey` и `setValue`.
    /// \see IniLine::setKey(), IniLine::setValue()
    bool IniLine::modified() const noexcept {
        return this->_modified;
    }

    /// \brief Установка состояния строки "не модифицирована".
    void IniLine::notModified() noexcept {
        this->_modified = false;
    }

    /// \brief Смена ключа строки.
    /// \param newKey Новый ключ строки.
    ///
    /// Устанавливает состояние "строка модифицирована".
    void IniLine::setKey(const std::string &newKey) {
        this->key = newKey;
        this->_modified = true;
    }

    /// \brief Смена ключа строки.
    /// \param newKey Новый ключ строки.
    ///
    /// Устанавливает состояние "строка модифицирована".
    void IniLine::setKey(std::string &&newKey) noexcept {
        this->key = newKey;
        this->_modified = true;
    }

    /// \brief Смена значения строки.
    /// \param newValue Новое значение строки.
    ///
    /// Устанавливает состояние "строка модифицирована".
    void IniLine::setValue(const std::string &newValue) {
        this->value = newValue;
        this->_modified = true;
    }

    /// \brief Смена значения строки.
    /// \param newValue Новое значение строки.
    ///
    /// Устанавливает состояние "строка модифицирована".
    void IniLine::setValue(std::string &&newValue) noexcept {
        this->value = newValue;
        this->_modified = true;
    }

    /// \brief Формирует текстовое представление строки.
    /// \return Ключ=значение.
    std::string IniLine::toString() const {
        return this->key + "=" + this->value;
    }

    //=========================================================

    /// \brief Очищает секцию (удаляет все строки).
    /// \return `this`.
    IniSection &IniSection::clear() {
        this->lines.clear();
        this->notModified();

        return *this;
    }

    /// \brief Содержит ли данная секция строку с указанным ключом?
    /// \param key Искомый ключ.
    /// \return `true` если содержит.
    bool IniSection::containsKey(const std::string &key) const noexcept {
        return std::any_of
            (
                std::begin(this->lines),
                std::end(this->lines),
                [&key](const IniLine &x) { return sameString(x.key, key); }
            );
    }

    /// \brief Получение индекса строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \return Индекс строки, либо отрицательное значение,
    /// если строка не найдена.
    std::ptrdiff_t IniSection::getIndex(const std::string &key) const noexcept {
        for (std::size_t i = 0; i < this->lines.size(); i++) {
            if (sameString(this->lines[i].key, key)) {
                return static_cast<std::ptrdiff_t> (i);
            }
        }

        return -1;
    }

    /// \brief Поиск строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \return Указатель на найденную строку, либо `nullptr`.
    IniLine *IniSection::getLine(const std::string &key) const noexcept {
        for (const auto &line: this->lines) {
            if (sameString(line.key, key)) {
                return const_cast<IniLine *> (&line);
            }
        }

        return nullptr;
    }

    /// \brief Получение значения строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \param defaultValue Значение, возвращаемое, если строка не найдена.
    /// \return Найденное значение либо значение по умолчанию.
    std::string IniSection::getValue(const std::string &key, const std::string &defaultValue) const noexcept {
        const auto line = this->getLine(key);

        return line ? line->value : defaultValue;
    }

    /// \brief Есть ли модифицированные строки в данной секции?
    /// \return `true`, если модифицированные строки есть.
    bool IniSection::modified() const noexcept {
        return std::any_of
            (
                begin(this->lines),
                end(this->lines),
                [](const IniLine &x) { return x.modified(); }
            );
    }

    /// \brief Пометка всех строк в данной секции как немодифицированных.
    void IniSection::notModified() {
        std::for_each
            (
                begin(this->lines),
                end(this->lines),
                [](IniLine &x) { x.notModified(); }
            );
    }

    /// \brief Удаление строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \return `this`.
    IniSection &IniSection::removeValue(const std::string &key) {
        auto index = this->getIndex(key);
        if (index >= 0) {
            this->lines.erase(std::begin(this->lines) + index);
        }

        return *this;
    }

    /// \brief Установка значения строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \param value Новое значение строки.
    /// \return `this`.
    IniSection &IniSection::setValue(const std::string &key, const std::string &value) {
        auto *line = this->getLine(key);
        if (line != nullptr) {
            line->setValue(value);
        } else {
            IniLine item;
            item.setKey(key);
            item.setValue(value);
            this->lines.push_back(item);
        }

        return *this;
    }

    /// \brief Получение текстового представления секции.
    /// \return Текстовое представление.
    std::string IniSection::toString() const {
        std::string result;
        if (!this->name.empty()) {
            result = std::string("[") + this->name + std::string("]\n");
        }

        for (const auto &line: lines) {
            result.append(line.toString());
            result.append("\n");
        }

        return result;
    }

    /// Получение значения строки с указанным ключом.
    /// \param index Искомый ключ.
    /// \return Значение найденной строки либо пустая строка.
    std::string IniSection::operator[](const std::string &index) const noexcept {
        return this->getValue(index, "");
    }

    //=========================================================

    /// \brief Удаление всех секций из файла.
    /// \return `this`.
    IniFile &IniFile::clear() {
        this->sections.clear();

        return *this;
    }

    /// \brief Есть ли секция с указанным именем?
    /// \param name Искомое имя секции.
    /// \return `true` если секция существует.
    bool IniFile::containsSection(const std::string &name) const noexcept {
        return std::any_of
            (
                std::begin(this->sections),
                std::end(this->sections),
                [&name](const IniSection &x) { return sameString(x.name, name); }
            );
    }

    /// \brief Создание секции с указанным именем.
    /// \param name Имя секции.
    /// \return Созданная либо найденная секция.
    ///
    /// Если секция с указанным именем уже существует, возвращается именно она,
    /// создания новой секции не происходит.
    IniSection &IniFile::createSection(const std::string &name) {
        auto section = this->getSection(name);
        if (section) {
            return *section;
        }

        this->sections.emplace_back();
        auto &result = this->sections.back();
        result.name = name;

        return result;
    }

    /// \brief Есть ли модифицированные секции?
    /// \return `true`, если есть модифицированные секции.
    bool IniFile::modified() const noexcept {
        return std::any_of
            (
                std::begin(this->sections),
                std::end(this->sections),
                [](const IniSection &x) { return x.modified(); }
            );
    }

    /// \brief Установка состояния "не модифицировано" для всех секций.
    void IniFile::notModified() {
        std::for_each
            (
                std::begin(this->sections),
                std::end(this->sections),
                [](IniSection &x) { x.notModified(); }
            );
    }

    /// \brief Получение индекса секции с указанным именем.
    /// \param name Искомое имя секции.
    /// \return Индекс секции либо отрицательное число, если секция не найдена.
    std::ptrdiff_t IniFile::getIndex(const std::string &name) const noexcept {
        for (std::size_t i = 0; i < this->sections.size(); i++) {
            if (sameString(this->sections[i].name, name)) {
                return static_cast <ptrdiff_t> (i);
            }
        }

        return -1;
    }

    /// \brief Получение указателя на секцию с указанным именем.
    /// \param name Искомое имя секции.
    /// \return Указатель на найденную секцию, либо `nullptr`, если секция не найдена.
    IniSection *IniFile::getSection(const std::string &name) const noexcept {
        for (const auto &section: this->sections) {
            if (sameString(section.name, name)) {
                return const_cast<IniSection *> (&section);
            }
        }

        return nullptr;
    }

    /// \brief Получение значения строки с указанным ключом в указанной секции.
    /// \param sectionName Имя секции.
    /// \param keyName Ключ строки.
    /// \param defaultValue Значение, возвращаемое, если строка не найдена.
    /// \return Значение найденной строки либо значение по умолчанию.
    std::string IniFile::getValue(const std::string &sectionName, const std::string &keyName,
                                  const std::string &defaultValue) const noexcept {
        const auto section = this->getSection(sectionName);

        return section ? section->getValue(keyName, defaultValue) : defaultValue;
    }

    /// \brief Разбор текстового представления INI-файла.
    /// \param lines Строки INI-файла.
    void IniFile::parse(const StringList &lines) {
        IniSection *section = nullptr;

        for (const auto &line: lines) {
            auto trimmed = trim(line);
            if (trimmed.empty()) {
                continue;
            }

            if (trimmed[0] == '[') {
                auto name = trimmed.substr(1, trimmed.size() - 2);
                section = &this->createSection(name);
            } else if (section) {
                const auto parts = maxSplit(trimmed, '=', 2);
                auto key = trim(parts[0]);
                auto value = trim(safeAt(parts, 1));
                IniLine item;
                item.setKey(key);
                item.setValue(value);
                section->lines.push_back(item);
            }
        }
    }

    /// \brief Удаление секции с указанным именем.
    /// \param sectionName Имя секции.
    /// \return `this`.
    IniFile &IniFile::removeSection(const std::string &sectionName) {
        const auto index = this->getIndex(sectionName);
        if (index >= 0) {
            sections.erase(begin(this->sections) + index);
        }

        return *this;
    }

    /// \brief Удаление строки с указанным ключом в указанной секции.
    /// \param sectionName Имя секции.
    /// \param keyName Ключ.
    /// \return `this`.
    IniFile &IniFile::removeValue(const std::string &sectionName, const std::string &keyName) {
        const auto section = this->getSection(sectionName);
        if (section) {
            section->removeValue(keyName);
        }
        return *this;
    }

    /// \brief Установка значения строки с указанным ключом в указанной секции.
    /// \param sectionName Имя секции.
    /// \param keyName Ключ.
    /// \param value Значение.
    /// \return `this`.
    IniFile &IniFile::setValue(const std::string &sectionName, const std::string &keyName, const std::string &value) {
        auto section = this->createSection(sectionName);
        section.setValue(keyName, value);
        return *this;
    }

    /// \brief Конверсия в текстовое представление.
    /// \return Текст.
    std::string IniFile::toString() const {
        std::string result;
        for (const auto &section: this->sections) {
            if (!result.empty()) {
                result += "\n";
            }

            if (!section.lines.empty()) {
                result += section.toString();
            }
        }

        return result;
    }

    //================================================================

    /// \brief Клонирование подполя.
    /// \return Точная копия подполя.
    SubField SubField::clone() const {
        SubField result;
        result.code = this->code;
        result.value = this->value;
        return result;
    }

    /// \brief Декодирование подполя из клиентского представления.
    /// \param line Строка с клиентским представлением (не должна быть пустой).
    void SubField::decode(const std::string &line) {
        this->code = line[0];
        this->value = line.substr(1);
    }

    /// \brief Пустое подполе?
    /// \return true, если подполе пустое.
    bool SubField::empty() const noexcept {
        return code == NoCode || value.empty();
    }

    /// \brief Кодирование подполя в клиентское (строковое) представление.
    /// \return Закодированное подполе.
    std::string SubField::toString() const {
        return std::string("^") + this->code + this->value;
    }

    std::ostream &operator<<(std::ostream &stream, const SubField &subfield) {
        return stream << std::string("^") << subfield.code << subfield.value;
    }

    /// \brief Изменение значения подполя.
    /// \param value_ Новое значение подполя.
    /// \return this.
    SubField &SubField::operator<<(const std::string &value_) {
        this->value = value_;
        return *this;
    }

    /// \brief Изменение значения подполя.
    /// \param value_ Новое значение подполя.
    /// \return this.
    SubField &SubField::operator<<(std::string &&value_) {
        this->value = std::move(value_);
        return *this;
    }

    /// \brief Изменение значения подполя.
    /// \param value_ Новое значение подполя.
    /// \return this.
    SubField &SubField::operator<<(const char *value_) {
        this->value = value_;
        return *this;
    }

    //================================================================

    /// \brief Конструктор.
    /// \param tag Метка поля.
    /// \param subfields_ Перечень подполей.
    RecordField::RecordField(int tag_, std::initializer_list<SubField> subfields_)
        : tag{tag_}, subfields{subfields_} {
        // пустое тело конструктора
    }

    /// \brief Добавление подполя.
    /// \param subFieldCode Код подполя.
    /// \param subFieldValue Значение подполя.
    /// \return this.
    RecordField &RecordField::add(char subFieldCode, const std::string &subFieldValue) {
        this->subfields.emplace_back(subFieldCode, subFieldValue);
        return *this;
    }

    /// \brief Добавление подполя.
    /// \param subFieldCode Код подполя.
    /// \param subFieldValue Значение подполя.
    /// \return this.
    RecordField &RecordField::add(char subFieldCode, std::string &&subFieldValue) {
        this->subfields.emplace_back(subFieldCode, std::move(subFieldValue));
        return *this;
    }

    /// \brief Очистка (удаление всех подполей).
    /// \return this.
    RecordField &RecordField::clear() {
        value.clear();
        this->subfields.clear();
        return *this;
    }

    /// \brief Создание глубокой копии поля.
    /// \return Глубокая копия.
    RecordField RecordField::clone() const {
        RecordField result(this->tag, this->value);
        for (const auto &sub: this->subfields) {
            result.subfields.emplace_back(sub.code, sub.value);
        }
        return result;
    }

    /// \brief Декодирование текстового представления тела поля.
    /// \param body Текст для декодирования.
    void RecordField::decodeBody(const std::string &body) {
        StringList all;
        if (body[0] == '^') {
            all = split(body, '^');
        } else {
            const auto parts2 = maxSplit(body, '#', 2);
            this->value = parts2[0];
            all = parts2.size() == 1 ? StringList() : split(parts2[1], '^');
        }
        for (const auto &one: all) {
            if (!one.empty()) {
                SubField subField;
                subField.decode(one);
                this->subfields.push_back(subField);
            }
        }
    }

    /// \brief Декодирование текстового представления поля,
    /// \param line Текст для декодирования.
    void RecordField::decode(const std::string &line) {
        const auto parts = maxSplit(line, '#', 2);
        this->tag = fastParse32(parts[0]);
        if (parts.size() == 1 || parts[1].empty()) {
            return;
        }
        const auto &body = parts[1];
        this->decodeBody(body);
    }

    /// \brief Пустое поле (нет значения и подполей)?
    /// \return true если пустое.
    bool RecordField::empty() const noexcept {
        return !this->tag || (this->value.empty() && this->subfields.empty());
    }

    /// \brief Получение указателя на первое подполе с указанным кодом.
    /// \param code Искомый код подполя.
    /// \return Указатель на подполе либо `nullptr`.
    SubField *RecordField::getFirstSubfield(char code) const noexcept {
        for (const auto &one: this->subfields) {
            if (sameChar(one.code, code)) {
                return const_cast<SubField *> (&one);
            }
        }
        return nullptr;
    }

    /// \brief Получение значения первого подполя с указанным кодом.
    /// \param code Искомый код подполя.
    /// \return Значение первого подполя либо пустая строка.
    std::string RecordField::getFirstSubfieldValue(char code) const noexcept {
        for (const auto &one: this->subfields) {
            if (sameChar(one.code, code)) {
                return one.value;
            }
        }
        return {};
    }

/// \brief Удаление подполя с указанным кодом.
/// \param code Искомый код подполя.
/// \return this.
    RecordField &RecordField::removeSubfield(char code) {
        const auto end = std::end(this->subfields);
        const auto trash = std::remove_if
            (
                std::begin(this->subfields),
                end,
                [code](SubField &sf) { return sameChar(sf.code, code); }
            );
        this->subfields.erase(trash, end);

        return *this;
    }

    /// \brief Установка значения подполя с указанным кодом.
    /// \param code Искомый код подполя.
    /// \param newValue Новое значение поля.
    /// \return this.
    RecordField &RecordField::setSubfield(char code, const std::string &newValue) {
        if (newValue.empty()) {
            this->removeSubfield(code);
            return *this;
        }
        auto *subField = this->getFirstSubfield(code);
        if (!subField) {
            this->add(code, newValue);
        } else {
            subField->value = newValue;
        }
        return *this;
    }

    /// \brief Установка значения подполя с указанным кодом.
    /// \param code Искомый код подполя.
    /// \param newValue Новое значение поля.
    /// \return this.
    RecordField &RecordField::setSubfield(char code, std::string &&newValue) {
        if (newValue.empty()) {
            this->removeSubfield(code);
            return *this;
        }
        auto *subField = this->getFirstSubfield(code);
        if (!subField) {
            this->add(code, std::move(newValue));
        } else {
            subField->value = std::move(newValue);
        }
        return *this;
    }

    /// \brief Получение строкового представления поля.
    /// \return Строковое представление поля.
    std::string RecordField::toString() const {
        std::string result = std::to_string(tag) + std::string("#") + value;
        for (const auto &sub: subfields) {
            result += sub.toString();
        }
        return result;
    }

    std::ostream &operator<<(std::ostream &stream, const RecordField &field) {
        stream << std::to_string(field.tag) << std::string("#") << field.value;
        for (const auto &sub: field.subfields) {
            stream << sub;
        }

        return stream;
    }

    /// \brief Добавление подполя в конец поля.
    /// \param subfield Подполе для добавления.
    /// \return this.
    RecordField &RecordField::operator<<(const SubField &subfield) {
        this->subfields.push_back(subfield);
        return *this;
    }

    /// \brief Добавление подполя в конец поля.
    /// \param subfield Подполе для добавления.
    /// \return this.
    RecordField &RecordField::operator<<(SubField &&subfield) {
        this->subfields.push_back(std::move(subfield));
        return *this;
    }

    /// \brief Изменение значения поля. Если в записи присутствуют подполя, то изменяется значение последнего подполя.
    /// \param body Новое значение поля.
    /// \return this.
    RecordField &RecordField::operator<<(const std::string &body) {
        if (contains(body, '^')) {
            this->decodeBody(body);
        } else {
            if (this->subfields.empty()) {
                this->value = body;
            } else {
                this->subfields.back().value = body;
            }
        }
        return *this;
    }

    /// \brief Изменение значения поля. Если в записи присутствуют подполя, то изменяется значение последнего подполя.
    /// \param body Новое значение поля.
    /// \return this.
    RecordField &RecordField::operator<<(std::string &&body) {
        if (contains(body, '^')) {
            this->decodeBody(body);
        } else {
            if (this->subfields.empty()) {
                this->value = std::move(body);
            } else {
                this->subfields.back().value = std::move(body);
            }
        }
        return *this;
    }

    /// \brief Изменение значения поля.
    /// \param body Новое значение поля.
    /// \return this.
    RecordField &RecordField::operator<<(const char *body) {
        return *this << std::string(body);
    }

    /// \brief Добавление подполя в конец поля.
    /// \param code Код подполя.
    /// \return this.
    RecordField &RecordField::operator<<(char code) {
        return *this << SubField(code);
    }

    //================================================================

    /// \brief Конструктор.
    /// \param fields_ Список полей.
    MarcRecord::MarcRecord(std::initializer_list<RecordField> fields_)
        : fields{fields_} {
        // пустое тело конструктора
    }

    /// \brief Добавление в конец записи поля с указанными меткой и значением.
    /// \param tag Метка добавляемого поля.
    /// \param value Значение поля (может быть пустым).
    /// \return Вновь созданное поле.
    RecordField &MarcRecord::add(int tag, const std::string &value) {
        this->fields.emplace_back(tag, value);
        return this->fields.back();
    }

    /// \brief Добавление в конец записи поля с указанными меткой и значением.
    /// \param tag Метка добавляемого поля.
    /// \param value Значение поля (может быть пустым).
    /// \return Вновь созданное поле.
    RecordField &MarcRecord::add(int tag, std::string &&value) {
        this->fields.emplace_back(tag, std::move(value));
        return this->fields.back();
    }

    /// \brief Создание клона записи.
    /// \return Клон записи.
    MarcRecord MarcRecord::clone() const {
        MarcRecord result;
        result.mfn = this->mfn;
        result.status = this->status;
        result.version = this->version;
        result.database = this->database;
        for (const auto &one: this->fields) {
            result.fields.push_back(std::move(one.clone()));
        }

        return result;
    }

    /// \brief Разбор текстового представления записи.
    /// \param lines Строки с полями записи.
    void MarcRecord::decode(const StringList &lines) {
        if (lines.size() < 2) {
            return;
        }

        // mfn and status of the record
        const auto firstLine = split(lines[0], '#');
        this->mfn = fastParseUnsigned32(firstLine[0]);
        this->status = static_cast<RecordStatus> (fastParseUnsigned32(safeAt(firstLine, 1)));

        // version of the record
        const auto secondLine = split(lines[1], '#');
        this->version = fastParseUnsigned32(safeAt(secondLine, 1));

        // fields
        for (std::size_t i = 2; i < lines.size(); i++) {
            const auto &line = lines[i];
            if (!line.empty()) {
                RecordField field;
                field.decode(line);
                this->fields.push_back(field);
            }
        }
    }

    /// \brief Запись удалена (логически или физически)?
    /// \return true если удалена.
    bool MarcRecord::deleted() const noexcept {
        return (this->status & RecordStatus::Deleted) != RecordStatus::None;
    }

    /// \brief Кодирование записи в текстовую форму.
    /// \param delimiter Разделитель строк.
    /// \return Текстовое представление записи.
    std::string MarcRecord::encode(const std::string &delimiter) const {
        std::string result = std::to_string(this->mfn) + "#"
                             + std::to_string(static_cast<int> (this->status)) + delimiter
                             + "0#" + std::to_string(this->version) + delimiter;
        for (const auto &field: this->fields) {
            result.append(field.toString());
            result.append(delimiter);
        }
        return result;
    }

    /// \brief Получение значения поля/подполя.
    /// \param tag Метка поля.
    /// \param code Код подполя (опционально).
    /// \return Значение поля/подполя либо пустая строка.
    std::string MarcRecord::fm(int tag, char code) const noexcept {
        for (const auto &field: this->fields) {
            if (field.tag == tag) {
                if (code) {
                    for (const auto &subfield: field.subfields) {
                        if (sameChar(subfield.code, code)) {
                            return subfield.value;
                        }
                    }
                } else {
                    return field.value;
                }
            }
        }
        return {};
    }

    /// \brief Получение вектора значений поля/подполя.
    /// \param tag Метка поля.
    /// \param code Код подполя (опционально).
    /// \return Вектор значений (возможно, пустой).
    StringList MarcRecord::fma(int tag, char code) const {
        StringList result;
        for (const auto &field: this->fields) {
            if (field.tag == tag) {
                if (code) {
                    for (const auto &subfield: field.subfields) {
                        if (sameChar(subfield.code, code)) {
                            if (!subfield.value.empty()) {
                                result.push_back(subfield.value);
                            }
                        }
                    }
                } else {
                    if (!field.value.empty()) {
                        result.push_back(field.value);
                    }
                }
            }
        }
        return result;
    }

    /// \brief Получение указателя на поле с указанной меткой.
    /// \param tag Метка поля.
    /// \param occurrence Повторение поля (нумерация с 0).
    /// \return Указатель на поле либо `nullptr`.
    RecordField *MarcRecord::getField(int tag, int occurrence) const noexcept {
        for (const auto &field: this->fields) {
            if (field.tag == tag) {
                if (!occurrence) {
                    return const_cast<RecordField *> (&field);
                }
                --occurrence;
            }
        }
        return nullptr;
    }

    /// \brief Получение вектора указателей на поля с указанной меткой.
    /// \param tag Метка поля.
    /// \return Вектор указателей (возможно, пустой).
    std::vector<RecordField *> MarcRecord::getFields(int tag) const {
        std::vector<RecordField *> result;
        for (const auto &field: this->fields) {
            if (field.tag == tag) {
                auto ptr = const_cast<RecordField *>(&field);
                result.push_back(ptr);
            }
        }
        return result;
    }

    /// \brief Удаление всех повторений поля с указанной меткой.
    /// \param tag Метка поля.
    /// \return this.
    MarcRecord &MarcRecord::removeField(int tag) {
        this->fields.erase
            (
                std::remove_if
                    (
                        this->fields.begin(),
                        this->fields.end(),
                        [tag](RecordField &field) { return field.tag == tag; }
                    ),
                this->fields.end()
            );

        return *this;
    }

    /// \brief Установка значения поля.
    /// \param tag Метка поля.
    /// \param value Новое значение поля.
    /// \return this.
    MarcRecord &MarcRecord::setField
        (
            int tag,
            const std::string &value
        ) {
        if (value.empty()) {
            return this->removeField(tag);
        }
        auto field = this->getField(tag);
        if (!field) {
            field = &this->add(tag);
        }
        field->value = value;

        return *this;
    }

    /// \brief Установка значения поля.
    /// \param tag Метка поля.
    /// \param value Новое значение поля.
    /// \return this.
    MarcRecord &MarcRecord::setField(int tag, std::string &&value) {
        if (value.empty()) {
            return this->removeField(tag);
        }
        auto field = this->getField(tag);
        if (!field) {
            field = &this->add(tag);
        }
        field->value = std::move(value);
        return *this;
    }

    /// \brief Сброс состояния записи.
    /// \return this.
    /// \details Может потребоваться, например,
    /// при переносе записи в другую базу данных.
    MarcRecord &MarcRecord::reset() noexcept {
        this->mfn = 0;
        this->status = RecordStatus::None;
        this->version = 0;
        this->database.clear();
        return *this;
    }

    std::ostream &operator<<(std::ostream &stream, const MarcRecord &record) {
        stream << std::to_string(record.mfn) << std::string("#")
               << std::to_string(static_cast<int> (record.status)) << std::endl;
        stream << std::string("0#") << record.version << std::endl;
        for (const RecordField &field: record.fields) {
            stream << field << std::endl;
        }

        return stream;
    }

    /// \brief Добавление поля в конец записи.
    /// \param field Поле для дополнения.
    /// \return this.
    MarcRecord &MarcRecord::operator<<(const RecordField &field) {
        this->fields.push_back(field);
        return *this;
    }

    /// \brief Добавление поля в конец записи.
    /// \param field Поле для дополнения.
    /// \return this.
    MarcRecord &MarcRecord::operator<<(RecordField &&field) {
        this->fields.push_back(std::move(field));
        return *this;
    }

    /// \brief Добавление поля в конец записи.
    /// \param text Поле для дополнения в закодированном виде.
    /// \return this.
    MarcRecord &MarcRecord::operator<<(const std::string &text) {
        if (contains(text, '#')) {
            this->fields.emplace_back();
            auto &field = this->fields.back();
            field.decode(text);
        } else {
            auto &field = this->fields.back();
            if (contains(text, '^')) {
                field.decodeBody(text);
            } else {
                if (field.subfields.empty()) {
                    field.value = text;
                } else {
                    field.subfields.back().value = text;
                }
            }
        }
        return *this;
    }

    /// \brief Добавление поля в конец записи.
    /// \param tag Метка поля.
    /// \return this.
    MarcRecord &MarcRecord::operator<<(int tag) {
        return *this << RecordField(tag);
    }

    /// \brief Добавление подполя в конец последнего поля записи.
    /// \param code Код подполя.
    /// \return this.
    MarcRecord &MarcRecord::operator<<(char code) {
        this->fields.back() << code;
        return *this;
    }

    /// \brief Установка значения последнего подполя последнего поля записи.
    /// \param value Значение.
    /// \return this.
    MarcRecord &MarcRecord::operator&&(const std::string &value) {
        this->fields.back().subfields.back().value = value;
        return *this;
    }

    /// \brief Установка значения последнего подполя последнего поля записи.
    /// \param value Значение.
    /// \return this.
    MarcRecord &MarcRecord::operator&&(const char *value) {
        this->fields.back().subfields.back().value = value;
        return *this;
    }

    //================================================================

    /// \brief Конструктор клиентского запроса.
    /// \param connection Подключение, для которого формируется запрос.
    /// \param commandCode Код команды.
    ClientQuery::ClientQuery
        (
            const Connection &connection,
            const std::string &commandCode
        ) {
        this->addAnsi(commandCode).newLine();
        this->addAnsi(connection.workstation).newLine();
        this->addAnsi(commandCode).newLine();
        this->add(connection.clientId).newLine();
        this->add(connection.queryId).newLine();
        this->addAnsi(connection.password).newLine();
        this->addAnsi(connection.username).newLine();
        this->newLine();
        this->newLine();
        this->newLine();
    }

    /// \brief Добавление информации к запросу.
    /// \param bytes Массив с информацией.
    /// \param size Количество байт.
    void ClientQuery::_write(const unsigned char *bytes, std::size_t size) {
        for (std::size_t i = 0; i < size; i++) {
            this->_content.push_back(bytes[i]);
        }
    }

    /// \brief Добавление байта к запросу.
    /// \param byte Добавляемый байт.
    void ClientQuery::_write(unsigned char byte) {
        _content.push_back(byte);
    }

    /// \brief Добавление целого числа к запросу.
    /// \param value Добавляемое значение.
    /// \return this.
    ClientQuery &ClientQuery::add(int value) {
        const auto s = std::to_string(value);
        return this->addAnsi(s);
    }

    /// \brief Добавление файловой спецификации к запросу.
    /// \param specification Спецификация файла.
    /// \return this.
    ClientQuery &ClientQuery::add(const FileSpecification &specification) {
        // return this->addAnsi (specification.toString());
        return *this;
    }

    /// \brief Добавление строки в кодировке ANSI.
    /// \param text Добавляемый текст.
    /// \return `this`.
    /// \warning Предполагается, что переданный текст в кодировке ANSI!
    ClientQuery &ClientQuery::addAnsi
        (
            const std::string &text
        ) {
        // TODO реализовать

        const std::size_t size = text.length();
        if (!size) {
            return *this;
        }

        const auto *ptr = reinterpret_cast<const unsigned char *> (text.c_str());
        this->_write(ptr, size);
        return *this;
    }

    /// \brief Добавление строки в формате UTF-8.
    /// \param text Добавляемый текст.
    /// \return this.
    ClientQuery &ClientQuery::addUtf
        (
            const std::string &text
        ) {
        const std::size_t size = text.length();
        if (!size) {
            return *this;
        }

        const auto *ptr = reinterpret_cast<const unsigned char *> (text.c_str());
        this->_write(ptr, size);

        return *this;

//        const auto size = text.length();
//        if (!size) {
//            return *this;
//        }
//
//        const auto src = text.c_str();
//        const auto bufSize = countUtf (src, size);
//        Bytes dst (bufSize);
//        toUtf (dst.data(), src, size);
//        this->_write (dst.data(), bufSize);
//        return *this;
    }

    /// \brief Дамп запроса.
    /// \param stream Поток, в который выводится дамп.
    void ClientQuery::dump(std::ostream &stream) const {
        for (const auto value: this->_content) {
            // stream << std::hex << std::setw(2) << value << " ";
            stream << value;
        }

        stream.flush();
    }

    /// \brief Кодирование запроса.
    /// \return Закодированный запрос.
    std::vector<unsigned char> ClientQuery::encode() const {
        std::vector<unsigned char> result;
        result.reserve(this->_content.size() + 10);
        const auto prefix = std::to_string(this->_content.size());
        const auto ptr = prefix.c_str();
        for (std::size_t i = 0; i < prefix.length(); i++) {
            result.push_back(static_cast<unsigned char>(ptr[i]));
        }
        result.push_back(0x0a);
        result.insert(result.end(), this->_content.begin(), this->_content.end());

        return result;
    }

    /// \brief Добавление перевода строки.
    /// \return `this`.
    ClientQuery &ClientQuery::newLine() {
        this->_write(0x0A);
        return *this;
    }

    //================================================================

    /// \brief Конструктор.
    /// \param connection Подключение.
    /// \param query Полностью сформированный клиентский запрос.
    ///
    /// Вычитывает ответ сервера до конца и сохраняет его во внутреннем буфере.
    ServerResponse::ServerResponse
        (
            Connection &connection,
            ClientQuery &query
        ) {
        std::lock_guard<std::mutex> guard(connection._mutex);

        this->_connection = &connection;
        this->returnCode = 0;
        this->_success = false;
        this->_position = 0;

        // Read the response
        auto &socket = *connection.socket;
        socket.host = connection.host;
        socket.port = connection.port;
        socket.open();

        auto encoded = query.encode();
        const auto data = encoded.data();
        const auto size = encoded.size();
        socket.send(data, size);

        unsigned char buffer[2048];
        while (true) {
            const auto received = socket.receive(buffer, sizeof(buffer));
            if (received <= 0) {
                break;
            }
            _write(buffer, received);
        }
        socket.close();

        // decode the response
        this->command = this->readAnsi();
        this->clientId = this->readInteger();
        this->queryId = this->readInteger();
        this->answerSize = this->readInteger();
        this->serverVersion = this->readAnsi();
        this->readAnsi();
        this->readAnsi();
        this->readAnsi();
        this->readAnsi();
        this->readAnsi();

        // set the success flag
        this->_success = true;
    }

    /// \brief Проверка кода возврата последней операции, выполненной сервером.
    /// \return Результат проверки: true означает успешное завершение операции.
    bool ServerResponse::checkReturnCode() {
        return this->checkReturnCode(0);
    }

    /// \brief Проверка, достигнут ли конец ответа сервера.
    /// \return true, если достигнут конец, иначе false.
    bool ServerResponse::eot() const {
        return this->_position >= this->_content.size();
    }

    /// \brief Проверка кода возврата последней операции, выполненной сервером.
    /// \param nargs Количество разрешённых кодов. Далее идут разрешённые коды (отрицательные числа, не означающие сбой при выполнении операции).
    /// \return Результат проверки: true означает успешное завершение операии.
    ///
    /// В любом случае у соединения выставляется значение поля lastError.
    bool ServerResponse::checkReturnCode(int nargs, ...) {
        this->_connection->lastError = 0;
        if (!this->_success) {
            this->_connection->lastError = -100002;
            return false;
        }

        va_list args;
        auto result = true;

        if (this->getReturnCode() < 0) {
                va_start (args, nargs);
            result = false;
            for (auto i = 0; i < nargs; i++) {
                const int code = va_arg (args, int);
                if (code == this->returnCode) {
                    result = true;
                    break;
                }
            }
                va_end (args);
            this->_connection->lastError = this->returnCode;
        }

        return result;
    }

    /// \brief Чтение строки без преобразования кодировок.
    /// \return Прочитанная строка. Если достигнут конец ответа сервера, строка будет пустая.
    std::string ServerResponse::getLine() {
        std::string result;
        const auto size = this->_content.size();
        while (this->_position < size) {
            auto c = static_cast<char> (this->_content.at(_position));
            this->_position++;

            if (c == 13) {
                if (this->_position < size) {
                    c = static_cast<char> (this->_content.at(_position));
                    if (c == 10) {
                        this->_position++;
                    }
                }
                break;
            }

            result.push_back(c);
        }

        return result;
    }

    /// \brief Чтение оставшейся части ответа сервера без преобразования кодировок.
    /// \return Прочитанный ответ сервера. Если достигнут конец, строка будет пустая.
    std::string ServerResponse::getRemaining() {
        std::string result;
        const auto size = this->_content.size();
        if (this->_position < size) {
            const auto data = reinterpret_cast<const char *> (_content.data() + this->_position);
            std::size_t remaining = size - this->_position;
            // Убираем переводы строки в конце.
            while (remaining > 0) {
                const auto c = data[remaining - 1];
                if (c != '\r' && c != '\n') {
                    break;
                }
                remaining--;
            }
            const std::string s2(data, remaining);
            result = s2;
            this->_position = size;
        }

        return result;
    }

    /// \brief Получение кода возврата.
    /// \return Полученный от сервера код возврата. Отрицательное значение свидетельствует о проблеме (но не всегда).
    ///
    /// Получение кода возврата. Вызывается один раз в свой час и только тогда.
    int ServerResponse::getReturnCode() {
        this->returnCode = this->readInteger();
        return this->returnCode;
    }

    /// \brief Чтение строки в кодировке ANSI.
    /// \return Полученная строка. Если достигнут конец ответа, строка будет пустой.
    std::string ServerResponse::readAnsi() {
        auto line = this->getLine();
        return line;
    }

    /// \brief Чтение целого числа.
    /// \return Прочитанное число.
    ///
    /// Предполагается, что число занимает всю строку.
    int ServerResponse::readInteger() {
        const auto line = this->getLine();
        return std::stoi(line);
    }

    /// \brief Чтение оставшихся строк в кодировке ANSI.
    /// \return Вектор прочитанных строк.
    StringList ServerResponse::readRemainingAnsiLines() {
        const auto text = this->readRemainingAnsiText();
        return split(text, '\n');
    }

    /// \brief Чтение оставшегося текста в кодировке ANSI.
    /// \return Прочитанный текст.
    std::string ServerResponse::readRemainingAnsiText() {
        auto line = this->getRemaining();
        return line;
    }

    /// \brief Чтение оставшихся строк в кодировке UTF-8.
    /// \return Вектор прочитанных строк.
    StringList ServerResponse::readRemainingUtfLines() {
        StringList result;
        while (!this->eot()) {
            const auto line = readUtf();
            result.push_back(line);
        }
        return result;
    }

    /// \brief Чтение оставшихся строк в кодировке UTF-8.
    /// \return Вектор прочитанных строк.
    std::vector<std::string> ServerResponse::readRemainingLinesUtf() {
        std::vector<std::string> result;
        while (!this->eot()) {
            const auto line = getLine();
            result.push_back(line);
        }
        return result;
    }

    /// \brief Чтение оставшегося текста в кодировке UTF-8.
    /// \return Прочитанный текст.
    std::string ServerResponse::readRemainingUtfText() {
        auto line = getRemaining();
        return line;
    }

    /// \brief Чтение строки в кодировке UTF-8.
    /// \return Полученная строка. Если достигнут конец ответа, строка будет пустой.
    std::string ServerResponse::readUtf() {
        auto line = this->getLine();
        return line;
    }

    /// \brief Сетевой обмен с сервером завершился успешно?
    /// \return Признак успешности.
    bool ServerResponse::success() const {
        return this->_success;
    }

    void ServerResponse::_write(const unsigned char *bytes, std::size_t size) {
        const auto newSize = this->_content.size() + size;
        if (this->_content.capacity() < newSize) {
            this->_content.reserve(newSize);
        }

        for (std::size_t i = 0; i < size; i++) {
            this->_content.push_back(bytes[i]);
        }
    }

    /// \brief Создание пустого ответа сервера (для целей тестирования).
    ServerResponse *ServerResponse::emptyResonse() {
        auto result = new ServerResponse();
        return result;
    }

    //================================================================

    RecordStatus operator|(RecordStatus left, RecordStatus right) {
        return static_cast <RecordStatus > (
            static_cast <std::underlying_type<RecordStatus>::type> (left)
            | static_cast <std::underlying_type<RecordStatus>::type> (right)
        );
    }

    RecordStatus operator&(RecordStatus left, RecordStatus right) {
        return static_cast <RecordStatus > (
            static_cast <std::underlying_type<RecordStatus>::type> (left)
            & static_cast <std::underlying_type<RecordStatus>::type> (right)
        );
    }

    //================================================================

    /// \brief Разбор ответа сервера.
    /// \param response Ответ сервера.
    void Version::parse
        (
            ServerResponse &response
        ) {
        const auto lines = response.readRemainingAnsiLines();
        if (lines.size() == 3) {
            this->version = lines[0];
            this->connectedClients = fastParse32(lines[1]);
            this->maxClients = fastParse32(lines[2]);
        } else {
            this->organization = lines[0];
            this->version = lines[1];
            this->connectedClients = fastParse32(lines[2]);
            this->maxClients = fastParse32(lines[3]);
        }
    }

    /// \brief Строковое представление.
    /// \return Строковое представление.
    std::string Version::toString() const {
        return std::string("organization=") + this->organization +
               ", version=" + this->version +
               ", maxClients=" + std::to_string(this->maxClients) +
               ", connectedClients=" + std::to_string(this->connectedClients);
    }

    //================================================================

    class ClientSocketImpl final {
    public:

#ifdef WIN32

        WSADATA wsaData;
        SOCKET socket;

#else

        int socket;

#endif
    };

    //================================================================

    ClientSocket::ClientSocket
        (
            const std::string &host,
            short port
        )
        : _impl(new ClientSocketImpl) {
        this->host = host;
        this->port = port;

#ifdef WIN32

        if (WSAStartup(0x0202, &this->_impl->wsaData)) {
            throw std::exception();
        }

#endif
    }

    ClientSocket::~ClientSocket() {
#ifdef WIN32

        WSACleanup();

#endif
    }


    void ClientSocket::open() {
        this->_impl->socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in destinationAddress{};
        destinationAddress.sin_family = AF_INET;
        destinationAddress.sin_port = htons(this->port);
        auto ansi = this->host;
        auto inaddr = inet_addr(ansi.c_str());
        if (inaddr != INADDR_NONE) {
            destinationAddress.sin_addr.s_addr = inaddr;
        } else {

            HOSTENT *hostentry = gethostbyname(ansi.c_str());
            if (hostentry) {
                ((unsigned long *) &destinationAddress.sin_addr)[0] =
                    ((unsigned long **) hostentry->h_addr_list)[0][0];
            } else {
                throw std::exception();
            }
        }

        if (connect(this->_impl->socket, (sockaddr *) &destinationAddress, sizeof(destinationAddress))) {
            throw std::exception();
        }
    }

    void ClientSocket::close() {
        closesocket(this->_impl->socket);
    }

    void ClientSocket::send
        (
            const unsigned char *buffer,
            std::size_t size
        ) {
        const auto ptr = reinterpret_cast <const char *> (buffer);
        const auto size2 = static_cast <int> (size);
        ::send(this->_impl->socket, ptr, size2, 0);
    }

    std::size_t ClientSocket::receive
        (
            unsigned char *buffer,
            std::size_t size
        ) {
        const auto ptr = reinterpret_cast <char *> (buffer);
        const auto size2 = static_cast <int> (size);
        const std::size_t result = ::recv(this->_impl->socket, ptr, size2, 0);
        return result;
    }

    //================================================================

    Encoding* Encoding::_ansi = nullptr;
    Encoding* Encoding::_utf = nullptr;

    Encoding* Encoding::ansi()
    {
        if (!Encoding::_ansi) {
            Encoding::_ansi = new Cp1251Encoding;
        }
        return Encoding::_ansi;
    }

    std::string Encoding::fromAnsi (const Byte *bytes, std::size_t count)
    {
        // TODO реализовать

        return {};

        // return _ansi->toUnicode(bytes, count);
    }

    std::string Encoding::fromUtf (const Byte *bytes, std::size_t count)
    {
        // TODO реализовать

        return {};

        // return _utf->toUnicode(bytes, count);
    }

    Bytes Encoding::toAnsi (const std::string &text)
    {
        return {};

        // return _ansi->fromUnicode(text);
    }

    Bytes Encoding::toUtf (const std::string &text)
    {
        return {};

        // return _utf->fromUnicode(text);
    }

    Encoding* Encoding::utf()
    {
        if (!Encoding::_utf) {
            Encoding::_utf = new Utf8Encoding();
        }

        return Encoding::_utf;
    }

    static wchar_t _cp1251_to_unicode[256]
    {
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
        0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
        0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
        0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
        0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
        0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
        0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
        0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
        0x0402, 0x0403, 0x201A, 0x0453, 0x201E, 0x2026, 0x2020, 0x2021, 0x20AC, 0x2030, 0x0409, 0x2039, 0x040A, 0x040C, 0x040B, 0x040F,
        0x0452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x0098, 0x2122, 0x0459, 0x203A, 0x045A, 0x045C, 0x045B, 0x045F,
        0x00A0, 0x040E, 0x045E, 0x0408, 0x00A4, 0x0490, 0x00A6, 0x00A7, 0x0401, 0x00A9, 0x0404, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x0407,
        0x00B0, 0x00B1, 0x0406, 0x0456, 0x0491, 0x00B5, 0x00B6, 0x00B7, 0x0451, 0x2116, 0x0454, 0x00BB, 0x0458, 0x0405, 0x0455, 0x0457,
        0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
        0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
        0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
        0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F
    };

    static wchar_t _cp1251_from_unicode[256]
    {
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
        0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
        0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
        0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
        0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
        0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
        0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
        0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
        0x0098, 0x00A0, 0x00A4, 0x00A6, 0x00A7, 0x00A9, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00B0, 0x00B1, 0x00B5, 0x00B6, 0x00B7, 0x00BB,
        0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408, 0x0409, 0x040A, 0x040B, 0x040C, 0x040E, 0x040F, 0x0410, 0x0411,
        0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F, 0x0420, 0x0421,
        0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F, 0x0430, 0x0431,
        0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F, 0x0440, 0x0441,
        0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F, 0x0451, 0x0452,
        0x0453, 0x0454, 0x0455, 0x0456, 0x0457, 0x0458, 0x0459, 0x045A, 0x045B, 0x045C, 0x045E, 0x045F, 0x0490, 0x0491, 0x2013, 0x2014,
        0x2018, 0x2019, 0x201A, 0x201C, 0x201D, 0x201E, 0x2020, 0x2021, 0x2022, 0x2026, 0x2030, 0x2039, 0x203A, 0x20AC, 0x2116, 0x2122
    };

    static unsigned char _cp1251_xlat[256]
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
        0x98, 0xA0, 0xA4, 0xA6, 0xA7, 0xA9, 0xAB, 0xAC, 0xAD, 0xAE, 0xB0, 0xB1, 0xB5, 0xB6, 0xB7, 0xBB,
        0xA8, 0x80, 0x81, 0xAA, 0xBD, 0xB2, 0xAF, 0xA3, 0x8A, 0x8C, 0x8E, 0x8D, 0xA1, 0x8F, 0xC0, 0xC1,
        0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1,
        0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1,
        0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1,
        0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0xB8, 0x90,
        0x83, 0xBA, 0xBE, 0xB3, 0xBF, 0xBC, 0x9A, 0x9C, 0x9E, 0x9D, 0xA2, 0x9F, 0xA5, 0xB4, 0x96, 0x97,
        0x91, 0x92, 0x82, 0x93, 0x94, 0x84, 0x86, 0x87, 0x95, 0x85, 0x89, 0x8B, 0x9B, 0x88, 0xB9, 0x99
    };

    std::wstring cp1251_to_unicode (const std::string &text)
    {
        std::wstring result;
        result.reserve (text.size());
        for (const char c : text) {
            const auto uc = static_cast<unsigned char> (c);
            const wchar_t wc = _cp1251_to_unicode[uc];
            result.push_back (wc);
        }

        return result;
    }

    std::string unicode_to_cp1251 (const std::wstring &text)
    {
        std::string result;
        result.reserve(text.size());
        const wchar_t *first = &_cp1251_from_unicode[0], *last = first + 256;
        for (const wchar_t c : text) {
            const std::ptrdiff_t index = std::lower_bound (first, last, c) - first;
            unsigned char sc = (index == 256) ? '?' : static_cast<unsigned char>(index);
            sc = _cp1251_xlat[sc];
            result.push_back (static_cast<char> (sc));
        }

        return result;
    }

    void unicode_to_cp1251 (Byte *dst, const wchar_t *src, std::size_t size)
    {
        const wchar_t *first = &_cp1251_from_unicode[0], *last = first + 256;
        for (std::size_t i = 0; i < size; i++) {
            wchar_t c = src[i];
            const std::ptrdiff_t index = std::lower_bound(first, last, c) - first;
            unsigned char sc = (index == 256) ? '?' : static_cast<unsigned char>(index);
            *dst++ = _cp1251_xlat[sc];
        }
    }

    //=========================================================

    Bytes Cp1251Encoding::fromUnicode (const std::wstring &text) const
    {
        const auto size = text.size();
        Bytes result;
        result.resize(size);
        unicode_to_cp1251(result.data(), text.data(), size);
        return result;
    }

    std::wstring Cp1251Encoding::toUnicode (const Byte *bytes, std::size_t count) const
    {
        std::wstring result;
        result.reserve (count);
        for (; count > 0; --count, ++bytes) {
            const auto uc = static_cast <unsigned char> (*bytes);
            const wchar_t wc = _cp1251_to_unicode [uc];
            result.push_back (wc);
        }

        return result;
    }

    std::size_t Cp1251Encoding::getSize (const std::wstring &text) const
    {
        return text.size();
    }

    // Quick and dirty UTF-8 conversion

    /// \brief Преобразует UCS-16 в UTF-8.
    /// \param dst Указатель на буфер для результата.
    /// \param src Указатель на буфер с источником.
    /// \param length Количество символов для преобразования.
    /// \return Возвращает указатель на место после последнего преобразованного символа
    Byte* toUtf (Byte *dst, const wchar_t *src, std::size_t length) noexcept
    {
        while (length > 0) {
            const unsigned int c = *src++;
            if (c < (1u << 7u)) {
                *dst++ = static_cast<Byte> (c);
            }
            else if (c < (1u << 11u)) {
                *dst++ = static_cast<Byte> ((c >> 6u) | 0xC0u);
                *dst++ = static_cast<Byte> ((c & 0x3Fu) | 0x80u);
            }
            else if (c < (1u << 16u)) { //-V547
                *dst++ = static_cast<Byte> ((c >> 12u) | 0xE0u);
                *dst++ = static_cast<Byte> (((c >> 6u) & 0x3Fu) | 0x80u);
                *dst++ = static_cast<Byte> ((c & 0x3Fu) | 0x80u);
            }
            else if (c < (1u << 21u)) {
                *dst++ = static_cast<Byte> ((c >> 18u) | 0xF0u);
                *dst++ = static_cast<Byte> (((c >> 12u) & 0x3Fu) | 0x80u);
                *dst++ = static_cast<Byte> (((c >> 6u) & 0x3Fu) | 0x80u);
                *dst++ = static_cast<Byte> ((c & 0x3Fu) | 0x80u);
            }
            else {
                return nullptr;
            }
            length--;
        }

        return dst;
    }

    /// \brief Подсчитывает число байт, необходимых для размещения в UTF-8.
    /// \param src Указатель на текст в UCS-16.
    /// \param length Длина текста в символах.
    /// \return Длина заданного текста в UTF-8 в байтах.
    std::size_t countUtf (const wchar_t *src, std::size_t length) noexcept
    {
        std::size_t result = 0;

        while (length > 0)
        {
            const unsigned int c = *src++;
            if (c < (1u << 7u))
            {
                result++;
            }
            else if (c < (1u << 11u))
            {
                result++;
                result++;
            }
            else if (c < (1u << 16u)) //-V547
            {
                result += 3;
            }
            else if (c < (1u << 21u))
            {
                result +=4;
            }
            else
            {
                return 0;
            }
            length--;
        }

        return result;
    }

    /// \brief Преобразует UTF-8 в UCS-16.
    /// \param dst Указатель на буфер для результата.
    /// \param src Указатель на буфер с источником.
    /// \param length Длина исходного текста в байтах.
    /// \return Возвращает указатель на место после последнего преобразованного символа.
    wchar_t* fromUtf (wchar_t *dst, const Byte *src, std::size_t length) noexcept
    {
        const Byte *stop = src + length;

        while (src < stop)
        {
            unsigned int c = *src++;
            if ((c & 0x80u) == 0u)
            {
                // 1-Byte sequence: 000000000xxxxxxx = 0xxxxxxx
            }
            else if ((c & 0xE0u) == 0xC0u)
            {
                // 2-Byte sequence: 00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
                c = (c & 0x1Fu) << 6u;
                c |= (*src++ & 0x3Fu);
            }
            else if ((c & 0xF0u) == 0xE0u)
            {
                // 3-Byte sequence: zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
                c = (c & 0x0Fu) << 12u;
                c |= (*src++ & 0x3Fu) << 6u;
                c |= (*src++ & 0x3Fu);
                if (c == 0xFEFFu) { // bom at start
                    continue; // skip it
                }
            }
            else if ((c & 0xF8u) == 0xF0u)
            {
                // 4-Byte sequence: 11101110wwwwzzzzyy + 110111yyyyxxxxxx = 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
                c = (c & 0x07u) << 18u;
                c |= (*src++ & 0x3Fu) << 12u;
                c |= (*src++ & 0x3Fu) << 6u;
                c |= (*src++ & 0x3Fu);
            }
            *dst++ = static_cast<wchar_t> (c);
        }

        return dst;
    }

    /// \brief Подсчитывает число Char, необходимых для размещения в UCS-16.
    /// \param src Указатель на текст в кодировке UTF-8.
    /// \param length Длина изучаемого текста в байтах.
    /// \return Длина того же текста в символах.
    std::size_t countUtf (const Byte *src, std::size_t length) noexcept
    {
        std::size_t result = 0;

        const Byte *stop = src + length;

        while (src < stop)
        {
            unsigned int c = *src++;
            if ((c & 0x80u) == 0u)
            {
            }
            else if ((c & 0xE0u) == 0xC0u)
            {
                src++;
            }
            else if ((c & 0xF0u) == 0xE0u)
            {
                c = (c & 0x0Fu) << 12u;
                c |= (*src++ & 0x3Fu) << 6u;
                c |= (*src++ & 0x3Fu);
                if (c == 0xFEFF) { // bom at start
                    continue; // skip it
                }
            }
            else if ((c & 0xF8u) == 0xF0u)
            {
                src += 3;
            }
            result++;
        }

        return result;
    }

    /// \brief Считывает строку UTF-8 вплоть до разделителя.
    /// \param src Буфер для считывания.
    /// \param size Размер буфера.
    /// \param stop Стоп-байт.
    /// \param result Строка для помещения результата.
    /// \return Указатель на буфер сразу за последним прочитанным байтом.
    const Byte* fromUtf (const Byte *src, std::size_t &size, Byte stop, std::wstring &result)
    {
        const Byte *end = src;
        while (size && (*end != stop)) {
            end++;
            size--;
        }

        const std::size_t length = end - src;
        if (length == 0) {
            result.clear();
            return end;
        }

        const std::size_t resultSize = countUtf (src, length);
        result.reserve (resultSize + 1);
        result.resize (resultSize);
        auto *dst = const_cast<wchar_t*> (result.data());
        memset (dst, 0, sizeof (wchar_t) * resultSize);

        // TODO починить
        fromUtf (dst, src, length) - dst;

        return end;
    }

    /// \brief Преобразует строку в кодировке UTF-8 в строку в кодировке UCS-16.
    /// \param text Исходная строка.
    /// \return Получившаяся строка
    std::wstring fromUtf (const std::string &text)
    {
        const auto srcSize = text.length();
        if (!srcSize) {
            return {};
        }

        const auto *src = reinterpret_cast<const Byte*> (text.c_str());
        const auto dstSize = countUtf (src, srcSize);
        std::wstring result;
        result.reserve (dstSize + 1);
        result.resize (dstSize);
        auto *dst = const_cast<wchar_t*> (result.data());
        memset (dst, 0, sizeof (wchar_t) * (dstSize + 1));
        if (!fromUtf (dst, src, srcSize)) {
            result.clear();
        }

        return result;
    }

    /// \brief Записывает строку UCS-16 в буфер в кодировке UTF-8.
    /// \param dst Указатель на буфер в кодировке UTF-8.
    /// \param text Текст, который нужно записать.
    /// \return Указатель сразу за последним записанным байтом.
    Byte* toUtf (Byte *dst, const std::wstring &text)
    {
        const std::size_t length = text.length();
        const wchar_t *src = text.c_str();

        return toUtf (dst, src, length);
    }

    /// \brief Преобразование строки из UCS-16 в UTF-8.
    /// \param text Текст для преобразования.
    /// \return Преобразованный текст.
    std::string toUtf (const std::wstring &text)
    {
        const auto srcSize = text.length();
        if (!srcSize) {
            return {};
        }

        const auto *src = reinterpret_cast<const wchar_t*> (text.c_str());
        const auto dstSize = countUtf (src, srcSize);
        auto *dst = new Byte[dstSize + 1];
        memset(dst, 0, sizeof (Byte) * (dstSize + 1));
        if (!toUtf(dst, src, srcSize)) {
            return {};
        }

        std::string result (reinterpret_cast<const char*> (dst));
        delete[] dst;

        return result;
    }

    //=========================================================

    Bytes Utf8Encoding::fromUnicode (const std::wstring &text) const
    {
        const auto size = countUtf (text.data(), text.size());
        Bytes result;
        result.reserve (size);
        irbis::toUtf (result.data(), text);

        return result;
    }

    std::wstring Utf8Encoding::toUnicode (const Byte *bytes, std::size_t count) const
    {
        std::wstring result;
        result.reserve (count);
        irbis::fromUtf (const_cast<wchar_t*> (result.data()), bytes, count);

        return result;
    }

    std::size_t Utf8Encoding::getSize (const std::wstring &text) const
    {
        return countUtf (text.data(), text.length());
    }

    //=========================================================

    /// \brief Преобразование строки из UTF16 в однобайтную русскую кодировку.
    /// \param s Текст для преобразования.
    /// \return Полученный результат.
    std::string wide2string (const std::wstring &s)
    {
        return unicode_to_cp1251(s);
    }

    /// \brief Преобразование строки из однобайтной русской кодировки в UTF16.
    /// \param s Текст для преобразования.
    /// \return Полученный результат.
    std::wstring string2wide (const std::string &s)
    {
        return cp1251_to_unicode (s);
    }

    //=========================================================

    /// \brief Преобразование строки из однобайтовой русской кодировки в UTF8.
    /// \param s Текст для преобразования.
    /// \return Полученный результат.
    std::string toUtf (const std::string &s)
    {
        // TODO реализовать
        return {};

        //const auto temp = cp1251_to_unicode (s);
        //const auto result = toUtf (temp);
        //return result;
    }

    /// \brief Преобразование строки из UTF8 в однобайтовую русскую кодировку.
    /// \param s Текст для преобразования.
    /// \return Полученный результат.
    std::string toAnsi (const std::string &s)
    {
        // TODO реализовать
        return {};

        //const auto temp = fromUtf (s);
        //const auto result = unicode_to_cp1251 (temp);
        //return result;
    }

    //================================================================

    static MfnList parseLine (const std::string &line)
    {
        MfnList result;

        if (line.empty()) {
            return result;
        }

        auto items = split (line, '\u001E');
        for (const auto &item : items) {
            auto mfn = fastParse32 (item);
            result.push_back (mfn);
        }

        return result;
    }

    /// \brief Разбор ответа сервера.
    /// \param response Ответ сервера.
    void DatabaseInfo::parse (ServerResponse &response)
    {
        this->logicallyDeletedRecords = parseLine (response.readAnsi());
        this->physicallyDeletedRecords = parseLine (response.readAnsi());
        this->nonActualizedRecords = parseLine (response.readAnsi());
        this->lockedRecords = parseLine (response.readAnsi());
        this->maxMfn = fastParse32 (response.readAnsi());
        this->databaseLocked = fastParse32 (response.readAnsi()) != 0;
    }

    /// \brief Получение текстового представления.
    /// \return Текстовое представление информации о базе данных.
    std::string DatabaseInfo::toString() const
    {
        if (this->description.empty()) {
            return name;
        }

        return this->name + " - " + this->description;
    }

    //================================================================

    /// \brief Единственный конструктор для данного класса.
    Connection::Connection()
        : _connected    { false },
          host          { "127.0.0.1" },
          port          { 6666 },
          username      { },
          password      { },
          database      { "IBIS" },
          workstation   { "C" },
          clientId      { 0 },
          queryId       { 0 },
          lastError     { 0 },
          serverVersion { },
          interval      { 600 },
          stage         { RequestStage::None },
          socket        { new ClientSocket }
    {
        // пустое тело конструктора
    }

    /// \brief Деструктор.
    ///
    /// \details Если необходимо, выполняет отключение от сервера.
    Connection::~Connection()
    {
        this->disconnect();
        // this->socket.release();
    }

    /// \brief Проверка, подключены ли мы к серверу.
    /// \return `false` если не подключены.
    ///
    /// Большинство операций нет смысла начинать,
    /// если не установлено подключение к серверу.
    bool Connection::_checkConnection()
    {
        if (!this->connected()) {
            this->lastError = -100003;
            return false;
        }
        // this->lastError = 0;
        return true;
    }

    /// \brief Подключение к серверу.
    /// \return Признак успешности выполнения операции.
    /// \warning Подключение некоторыми типами клиентов увеличивает на сервере счетчик использованных лицензий!
    ///
    /// Повторные попытки подключения с помощью того же объекта Connection игнорируются.
    /// При этом возвращается true.
    bool Connection::connect()
    {
        if (this->connected()) {
            return true;
        }

        this->lastError = 0;

        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen (rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> dis (100000, 900000);

        AGAIN: this->clientId = dis (gen);
        this->queryId = 1;
        ClientQuery query (*this, "A");
        query.addAnsi (this->username).newLine()
            .addAnsi (this->password);

        try
        {
            ServerResponse response (*this, query);
            if (!response.success()) {
                return false;
            }

            response.getReturnCode();
            if (response.returnCode == -3337) {
                // клиент с данным идентификатором уже зарегистрирован
                goto AGAIN;
            }

            if (response.returnCode < 0) {
                return false;
            }

            this->_connected = true;
            this->serverVersion = response.serverVersion;
            this->interval = response.readInteger();
            const auto lines = response.readRemainingAnsiLines();
            this->iniFile.parse (lines);
        }
        catch (...) {
            this->lastError = -100002;
            return false;
        }

        return true;
    }

    /// \brief Отключение от сервера.
    ///
    /// Повторные попытки отключения уже отключенного экземпляра клиента игнорируются.
    /// Если при подключении был увеличен счётчик использованных лицензий, он соответственно уменьшается.
    void Connection::disconnect()
    {
        if (!this->connected()) {
            return;
        }

        ClientQuery query (*this, "B");
        query.addAnsi (this->username).newLine();
        this->execute (query);
        this->_connected = false;
    }

    /// \brief Результат исполнения запроса на сервере.
    /// \param query Полностью сформированный клиентский запрос.
    /// \return Признак успешного выполнения запроса.
    /// \warning Должно быть установлено подключение к серверу!
    ///
    /// Если метод вернул false, конкретный код ошибки находится в lastError.
    bool Connection::execute (ClientQuery &query)
    {
        if (!this->_checkConnection()) {
            return false;
        }

        bool result = false;

        try {
            ServerResponse response (*this, query);
            result = response.checkReturnCode();
        }
        catch (...) {
            // Do nothing
        }

        return result;
    }

    /// \brief Получение максимального MFN для базы данных с указанным именем.
    /// \param databaseName Имя базы данных.
    /// \return Максимальный MFN. 0 или отрицательное число означают ошибку.
    int Connection::getMaxMfn (const std::string &databaseName)
    {
        if (!this->_checkConnection()) {
            return 0;
        }

        ClientQuery query (*this, "O");
        query.addAnsi (databaseName);
        ServerResponse response (*this, query);
        if (!response.checkReturnCode()) {
            return 0;
        }

        const auto result = response.returnCode;
        return result;
    }

    /// \brief Пустая операция (не требует от сервера никаких действий).
    /// \return Признак успешности выполнения операции.
    ///
    /// Используется для подтверждения подключения клиента
    /// при длительном бездействии пользователя.
    bool Connection::noOp() {
        if (!this->_checkConnection()) {
            return false;
        }

        ClientQuery query (*this, "N");
        auto result = this->execute (query);
        return result;
    }

    /// \brief Разбор строки подключения.
    /// \param connectionString Строка подключения.
    /// \throw IrbisException Ошибка в структуре строки подключения.
    void Connection::parseConnectionString (const std::string &connectionString)
    {
        const auto items = split (connectionString, ";");
        for (auto &item : items) {
            auto parts = maxSplit (item, '=', 2);
            if (parts.size() != 2) {
                throw std::exception();
            }
            auto name = toLower (parts[0]);
            const auto value = parts[1];
            if (name.empty() || value.empty()) {
                throw std::exception();
            }
            if (name == "host" || name == "server" || name == "address") {
                this->host = value;
            } else if (name == "port") {
                this->port = static_cast<short > (fastParse32 (value));
            } else if (name == "user" || name == "username"
                       || name == "name" || name == "login") {
                this->username = value;
            } else if (name == "pwd" || name == "password") {
                this->password = value;
            } else if (name == "db" || name == "catalog" || name == "database") {
                this->database = value;
            } else if (name == "arm" || name == "workstation") {
                this->workstation = value.at (0);
            } else {
                throw std::exception();
            }
        }
    }

    /// \brief Возврат к предыдущей базе данных.
    /// \return Имя базы данных, использовавшейся до возврата.
    std::string Connection::popDatabase()
    {
        auto result = this->database;
        this->database = _databaseStack.back();
        _databaseStack.pop_back();
        return result;
    }

    /// \brief Запоминание имени текущей базы данных для последующего возврата к ней.
    /// \param newDatabase Имя базы данных, устанавливаемой в качестве текущей.
    /// \return Имя базы данных, использовавшейся в качестве текущей.
    std::string Connection::pushDatabase (const std::string &newDatabase)
    {
        auto result = this->database;
        _databaseStack.push_back(newDatabase);
        this->database = newDatabase;
        return result;
    }

    /// \brief Получение актуальной строки подключения.
    /// \return Строка подключения
    std::string Connection::toConnectionString() const
    {
        // Нет смысла логировать
        return std::string("host=") + this->host
               + ";port="          + std::to_string (this->port)
               + ";username="      + this->username
               + ";password="      + this->password
               + ";database"       + this->database
               + ";arm="           + this->workstation
               + ";";
    }

    /// \brief Получение версии сервера ИРБИС64.
    /// \return Версия сервера.
    Version Connection::getServerVersion()
    {
        Version result;
        if (!this->_checkConnection()) {
            return result;
        }

        ClientQuery query (*this, "1");
        ServerResponse response (*this, query);
        response.checkReturnCode();
        result.parse (response);

        return result;
    }

    /// \brief Чтение записи с сервера.
    /// \param mfn MFN записи.
    /// \return Запись.
    MarcRecord Connection::readRecord (int mfn)
    {
        MarcRecord result;
        if (!this->_checkConnection()) {
            return result;
        }

        ClientQuery query (*this, "C");
        query.addAnsi (this->database).newLine()
            .add(mfn);

        ServerResponse response (*this, query);
        if (response.checkReturnCode (4, -201, -600, -602, -603)) {
            const auto lines = response.readRemainingUtfLines();
            result.decode (lines);
            result.database = this->database;
        }
        return result;
    }

}

