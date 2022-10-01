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
        noexcept
    {
        return std::toupper (first) == std::toupper (second);
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
        noexcept
    {
        if (first.size() != second.size()) {
            return false;
        }

        return std::equal
            (
                first.begin(),
                first.end(),
                second.begin(),
                [](char a, char b) { return std::toupper (a) == std::toupper (b); }
            );
    }

    /// \brief Первый символ строки.
    /// \param text Текст.
    /// \return Первый символ либо 0.
    char firstChar
        (
            const std::string &text
        )
        noexcept
    {
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
        noexcept
    {
        for (auto &i: text) {
            i = (char) std::tolower (i);
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
        noexcept
    {
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
        )
    {
        return text.find (fragment) != std::string::npos;
    }

    /// \brief Содержит ли строка заданный символ?
    /// \param text Текст для изучения.
    /// \param c Символ для поиска.
    /// \return Возвращает true, если символ найден, иначе false.
    bool contains
        (
            const std::string &text,
            char c
        )
    {
        return text.find (c) != std::string::npos;
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
        )
    {
        auto result = text;
        std::size_t index = 0;
        while (true) {
            index = result.find (from, index);
            if (index == std::string::npos) {
                break;
            }

            result.replace (index, from.length(), to);
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
        )
    {
        auto length = text.length();
        std::size_t i = 0;
        for (; i < length; ++i) {
            const auto chr = static_cast<unsigned char> (text[i]);
            if (!std::isspace (chr)) {
                break;
            }
        }

        if (i == 0) {
            return text;
        }

        if (i == length) {
            return {};
        }

        return text.substr (i);
    }

    /// \brief Удаление пробельных символов в конце строки.
    /// \param text Текст для обработки.
    /// \return Обработанный текст.
    std::string trimEnd
        (
            const std::string &text
        )
    {
        auto length = text.length();
        auto i = static_cast<std::ptrdiff_t > (length - 1);
        for (; i >= 0; --i) {
            const auto chr = static_cast<unsigned char> (text[i]);
            if (!std::isspace (chr)) {
                break;
            }
        }

        if (i == static_cast<std::ptrdiff_t> (length - 1)) {
            return text;
        }

        if (i < 0) {
            return {};
        }

        return text.substr (0, i + 1);
    }

    /// \brief Удаление пробельных символов в начале и в конце строки.
    /// \param text Текст для обработки.
    /// \return Обработанный текст.
    std::string trim
        (
            const std::string &text
        )
    {
        const auto length = static_cast<std::ptrdiff_t> (text.length());
        std::ptrdiff_t start = 0, end = static_cast<std::ptrdiff_t> (length - 1);
        for (; start < length; ++start) {
            const auto chr = static_cast<unsigned char> (text[end]);
            if (!std::isspace (chr)) {
                break;
            }
        }

        for (; end >= 0; --end) {
            const auto chr = static_cast<unsigned char> (text[end]);
            if (!std::isspace (chr)) {
                break;
            }
        }

        if (end - start < 0) {
            return {};
        }

        if (end - start == length - 1) {
            return text;
        }

        return text.substr (start, end - start + 1);
    }


    /// \brief Быстрый и грязный разбор строки как целого числа без знака.
    /// \param text Текст для разбора.
    /// \return Мусор на входе - мусор на выходе!
    int fastParse32
        (
            const std::string &text
        )
        noexcept
    {
        // TODO обрабатывать знак числа

        auto result = 0;
        const std::size_t length = text.length();
        for (std::size_t offset = 0; offset < length; offset++) {
            result = result * 10 + text.at (offset) - '0';
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
        noexcept
    {
        auto result = 0u;
        const std::size_t length = text.length();
        for (std::size_t offset = 0; offset < length; offset++) {
            result = result * 10 + text.at (offset) - '0';
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
        noexcept
    {
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
        noexcept
    {
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
    bool IniLine::modified() const noexcept
    {
        return this->_modified;
    }

    /// \brief Установка состояния строки "не модифицирована".
    void IniLine::notModified() noexcept
    {
        this->_modified = false;
    }

    /// \brief Смена ключа строки.
    /// \param newKey Новый ключ строки.
    ///
    /// Устанавливает состояние "строка модифицирована".
    void IniLine::setKey (const std::string &newKey)
    {
        this->key = newKey;
        this->_modified = true;
    }

    /// \brief Смена ключа строки.
    /// \param newKey Новый ключ строки.
    ///
    /// Устанавливает состояние "строка модифицирована".
    void IniLine::setKey (std::string &&newKey) noexcept
    {
        this->key = newKey;
        this->_modified = true;
    }

    /// \brief Смена значения строки.
    /// \param newValue Новое значение строки.
    ///
    /// Устанавливает состояние "строка модифицирована".
    void IniLine::setValue (const std::string &newValue)
    {
        this->value = newValue;
        this->_modified = true;
    }

    /// \brief Смена значения строки.
    /// \param newValue Новое значение строки.
    ///
    /// Устанавливает состояние "строка модифицирована".
    void IniLine::setValue (std::string &&newValue) noexcept
    {
        this->value = newValue;
        this->_modified = true;
    }

    /// \brief Формирует текстовое представление строки.
    /// \return Ключ=значение.
    std::string IniLine::toString() const
    {
        return this->key + "=" + this->value;
    }

    //=========================================================

    /// \brief Очищает секцию (удаляет все строки).
    /// \return `this`.
    IniSection& IniSection::clear()
    {
        this->lines.clear();
        this->notModified();

        return *this;
    }

    /// \brief Содержит ли данная секция строку с указанным ключом?
    /// \param key Искомый ключ.
    /// \return `true` если содержит.
    bool IniSection::containsKey (const std::string &key) const noexcept
    {
        return std::any_of
            (
                std::begin (this->lines),
                std::end (this->lines),
                [&key] (const IniLine &x) { return sameString (x.key, key); }
            );
    }

    /// \brief Получение индекса строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \return Индекс строки, либо отрицательное значение,
    /// если строка не найдена.
    std::ptrdiff_t IniSection::getIndex (const std::string &key) const noexcept
    {
        for (std::size_t i=0; i < this->lines.size(); i++) {
            if (sameString (this->lines[i].key, key)) {
                return static_cast<std::ptrdiff_t> (i);
            }
        }

        return -1;
    }

    /// \brief Поиск строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \return Указатель на найденную строку, либо `nullptr`.
    IniLine* IniSection::getLine (const std::string &key) const noexcept
    {
        for (const auto &line : this->lines) {
            if (sameString (line.key, key)) {
                return const_cast<IniLine*> (&line);
            }
        }

        return nullptr;
    }

    /// \brief Получение значения строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \param defaultValue Значение, возвращаемое, если строка не найдена.
    /// \return Найденное значение либо значение по умолчанию.
    std::string IniSection::getValue (const std::string &key, const std::string &defaultValue) const noexcept
    {
        const auto line = this->getLine (key);

        return line ? line->value : defaultValue;
    }

    /// \brief Есть ли модифицированные строки в данной секции?
    /// \return `true`, если модифицированные строки есть.
    bool IniSection::modified() const noexcept
    {
        return std::any_of
            (
                begin (this->lines),
                end (this->lines),
                [] (const IniLine &x) { return x.modified(); }
            );
    }

    /// \brief Пометка всех строк в данной секции как немодифицированных.
    void IniSection::notModified()
    {
        std::for_each
            (
                begin (this->lines),
                end (this->lines),
                [] (IniLine &x) { x.notModified(); }
            );
    }

    /// \brief Удаление строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \return `this`.
    IniSection& IniSection::removeValue (const std::string &key)
    {
        auto index = this->getIndex (key);
        if (index >= 0) {
            this->lines.erase (std::begin(this->lines) + index);
        }

        return *this;
    }

    /// \brief Установка значения строки с указанным ключом.
    /// \param key Искомый ключ.
    /// \param value Новое значение строки.
    /// \return `this`.
    IniSection& IniSection::setValue (const std::string &key, const std::string &value)
    {
        auto *line = this->getLine (key);
        if (line != nullptr) {
            line->setValue (value);
        }
        else {
            IniLine item;
            item.setKey (key);
            item.setValue (value);
            this->lines.push_back (item);
        }

        return *this;
    }

    /// \brief Получение текстового представления секции.
    /// \return Текстовое представление.
    std::string IniSection::toString() const
    {
        std::string result;
        if (!this->name.empty()) {
            result = std::string ("[") + this->name + std::string ("]\n");
        }

        for(const auto &line : lines) {
            result.append (line.toString());
            result.append ("\n");
        }

        return result;
    }

    /// Получение значения строки с указанным ключом.
    /// \param index Искомый ключ.
    /// \return Значение найденной строки либо пустая строка.
    std::string IniSection::operator[] (const std::string &index) const noexcept
    {
        return this->getValue (index, "");
    }

    //=========================================================

    /// \brief Удаление всех секций из файла.
    /// \return `this`.
    IniFile& IniFile::clear()
    {
        this->sections.clear();

        return *this;
    }

    /// \brief Есть ли секция с указанным именем?
    /// \param name Искомое имя секции.
    /// \return `true` если секция существует.
    bool IniFile::containsSection (const std::string &name) const noexcept
    {
        return std::any_of
            (
                std::begin(this->sections),
                std::end(this->sections),
                [&name] (const IniSection &x) { return sameString(x.name, name); }
            );
    }

    /// \brief Создание секции с указанным именем.
    /// \param name Имя секции.
    /// \return Созданная либо найденная секция.
    ///
    /// Если секция с указанным именем уже существует, возвращается именно она,
    /// создания новой секции не происходит.
    IniSection& IniFile::createSection (const std::string &name)
    {
        auto section = this->getSection (name);
        if (section) {
            return *section;
        }

        this->sections.emplace_back ();
        auto &result = this->sections.back ();
        result.name = name;

        return result;
    }

    /// \brief Есть ли модифицированные секции?
    /// \return `true`, если есть модифицированные секции.
    bool IniFile::modified() const noexcept
    {
        return std::any_of
            (
                std::begin (this->sections),
                std::end (this->sections),
                [] (const IniSection &x) { return x.modified(); }
            );
    }

    /// \brief Установка состояния "не модифицировано" для всех секций.
    void IniFile::notModified()
    {
        std::for_each
            (
                std::begin (this->sections),
                std::end (this->sections),
                [] (IniSection &x) { x.notModified(); }
            );
    }

    /// \brief Получение индекса секции с указанным именем.
    /// \param name Искомое имя секции.
    /// \return Индекс секции либо отрицательное число, если секция не найдена.
    std::ptrdiff_t IniFile::getIndex (const std::string &name) const noexcept
    {
        for (std::size_t i = 0; i < this->sections.size(); i++) {
            if (sameString (this->sections[i].name, name)) {
                return static_cast <ptrdiff_t> (i);
            }
        }

        return -1;
    }

    /// \brief Получение указателя на секцию с указанным именем.
    /// \param name Искомое имя секции.
    /// \return Указатель на найденную секцию, либо `nullptr`, если секция не найдена.
    IniSection* IniFile::getSection (const std::string &name) const noexcept
    {
        for (const auto &section : this->sections) {
            if (sameString (section.name, name)) {
                return const_cast<IniSection*> (&section);
            }
        }

        return nullptr;
    }

    /// \brief Получение значения строки с указанным ключом в указанной секции.
    /// \param sectionName Имя секции.
    /// \param keyName Ключ строки.
    /// \param defaultValue Значение, возвращаемое, если строка не найдена.
    /// \return Значение найденной строки либо значение по умолчанию.
    std::string IniFile::getValue (const std::string &sectionName, const std::string &keyName, const std::string &defaultValue) const noexcept
    {
        const auto section = this->getSection (sectionName);

        return section ? section->getValue (keyName, defaultValue) : defaultValue;
    }

    /// \brief Разбор текстового представления INI-файла.
    /// \param lines Строки INI-файла.
    void IniFile::parse (const StringList &lines)
    {
        IniSection *section = nullptr;

        for (const auto &line : lines) {
            auto trimmed = trim (line);
            if (trimmed.empty()) {
                continue;
            }

            if (trimmed[0] == '[') {
                auto name = trimmed.substr(1, trimmed.size()-2);
                section = &this->createSection(name);
            }
            else if (section) {
                const auto parts = maxSplit (trimmed, '=', 2);
                auto key = trim (parts[0]);
                auto value = trim (safeAt (parts, 1));
                IniLine item;
                item.setKey (key);
                item.setValue (value);
                section->lines.push_back (item);
            }
        }
    }

    /// \brief Удаление секции с указанным именем.
    /// \param sectionName Имя секции.
    /// \return `this`.
    IniFile& IniFile::removeSection (const std::string &sectionName)
    {
        const auto index = this->getIndex (sectionName);
        if (index >= 0) {
            sections.erase (begin (this->sections) + index);
        }

        return *this;
    }

    /// \brief Удаление строки с указанным ключом в указанной секции.
    /// \param sectionName Имя секции.
    /// \param keyName Ключ.
    /// \return `this`.
    IniFile& IniFile::removeValue (const std::string &sectionName, const std::string &keyName)
    {
        const auto section = this->getSection (sectionName);
        if (section) {
            section->removeValue (keyName);
        }
        return *this;
    }

    /// \brief Установка значения строки с указанным ключом в указанной секции.
    /// \param sectionName Имя секции.
    /// \param keyName Ключ.
    /// \param value Значение.
    /// \return `this`.
    IniFile& IniFile::setValue (const std::string &sectionName, const std::string &keyName, const std::string &value)
    {
        auto section = this->createSection (sectionName);
        section.setValue (keyName, value);
        return *this;
    }

    /// \brief Конверсия в текстовое представление.
    /// \return Текст.
    std::string IniFile::toString() const
    {
        std::string result;
        for (const auto &section : this->sections) {
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
    RecordField::RecordField (int tag_, std::initializer_list <SubField> subfields_)
        : tag { tag_ }, subfields { subfields_ }
    {
        // пустое тело конструктора
    }

    /// \brief Добавление подполя.
    /// \param subFieldCode Код подполя.
    /// \param subFieldValue Значение подполя.
    /// \return this.
    RecordField& RecordField::add (char subFieldCode, const std::string &subFieldValue)
    {
        this->subfields.emplace_back (subFieldCode, subFieldValue);
        return *this;
    }

    /// \brief Добавление подполя.
    /// \param subFieldCode Код подполя.
    /// \param subFieldValue Значение подполя.
    /// \return this.
    RecordField& RecordField::add (char subFieldCode, std::string &&subFieldValue)
    {
        this->subfields.emplace_back (subFieldCode, std::move (subFieldValue));
        return *this;
    }

    /// \brief Очистка (удаление всех подполей).
    /// \return this.
    RecordField& RecordField::clear()
    {
        value.clear();
        this->subfields.clear();
        return *this;
    }

    /// \brief Создание глубокой копии поля.
    /// \return Глубокая копия.
    RecordField RecordField::clone() const
    {
        RecordField result (this->tag, this->value);
        for (const auto &sub : this->subfields) {
            result.subfields.emplace_back (sub.code, sub.value);
        }
        return result;
    }

    /// \brief Декодирование текстового представления тела поля.
    /// \param body Текст для декодирования.
    void RecordField::decodeBody (const std::string &body)
    {
        StringList all;
        if (body[0] == '^') {
            all = split (body, '^');
        } else {
            const auto parts2 = maxSplit (body, '#', 2);
            this->value = parts2[0];
            all = parts2.size() == 1 ? StringList () : split (parts2[1], '^');
        }
        for (const auto &one : all) {
            if (!one.empty()) {
                SubField subField;
                subField.decode (one);
                this->subfields.push_back (subField);
            }
        }
    }

    /// \brief Декодирование текстового представления поля,
    /// \param line Текст для декодирования.
    void RecordField::decode (const std::string &line)
    {
        const auto parts = maxSplit (line, '#', 2);
        this->tag = fastParse32 (parts[0]);
        if (parts.size() == 1 || parts[1].empty()) {
            return;
        }
        const auto &body = parts[1];
        this->decodeBody (body);
    }

    /// \brief Пустое поле (нет значения и подполей)?
    /// \return true если пустое.
    bool RecordField::empty() const noexcept
    {
        return !this->tag || (this->value.empty() && this->subfields.empty());
    }

    /// \brief Получение указателя на первое подполе с указанным кодом.
    /// \param code Искомый код подполя.
    /// \return Указатель на подполе либо `nullptr`.
    SubField* RecordField::getFirstSubfield (char code) const noexcept
    {
        for (const auto &one : this->subfields) {
            if (sameChar(one.code, code)) {
                return const_cast<SubField*> (&one);
            }
        }
        return nullptr;
    }

    /// \brief Получение значения первого подполя с указанным кодом.
    /// \param code Искомый код подполя.
    /// \return Значение первого подполя либо пустая строка.
    std::string RecordField::getFirstSubfieldValue (char code) const noexcept
    {
        for (const auto &one : this->subfields) {
            if (sameChar(one.code, code)) {
                return one.value;
            }
        }
        return {};
    }

/// \brief Удаление подполя с указанным кодом.
/// \param code Искомый код подполя.
/// \return this.
    RecordField& RecordField::removeSubfield (char code)
    {
        const auto end = std::end (this->subfields);
        const auto trash = std::remove_if
            (
                std::begin(this->subfields),
                end,
                [code] (SubField &sf) { return sameChar (sf.code, code); }
            );
        this->subfields.erase (trash, end);

        return *this;
    }

    /// \brief Установка значения подполя с указанным кодом.
    /// \param code Искомый код подполя.
    /// \param newValue Новое значение поля.
    /// \return this.
    RecordField& RecordField::setSubfield (char code, const std::string &newValue)
    {
        if (newValue.empty()) {
            this->removeSubfield (code);
            return *this;
        }
        auto *subField = this->getFirstSubfield (code);
        if (!subField) {
            this->add (code, newValue);
        }
        else {
            subField->value = newValue;
        }
        return *this;
    }

    /// \brief Установка значения подполя с указанным кодом.
    /// \param code Искомый код подполя.
    /// \param newValue Новое значение поля.
    /// \return this.
    RecordField& RecordField::setSubfield (char code, std::string &&newValue)
    {
        if (newValue.empty()) {
            this->removeSubfield (code);
            return *this;
        }
        auto *subField = this->getFirstSubfield (code);
        if (!subField) {
            this->add (code, std::move (newValue));
        }
        else {
            subField->value = std::move (newValue);
        }
        return *this;
    }

    /// \brief Получение строкового представления поля.
    /// \return Строковое представление поля.
    std::string RecordField::toString() const
    {
        std::string result = std::to_string(tag) + std::string("#") + value;
        for (const auto &sub : subfields) {
            result += sub.toString();
        }
        return result;
    }

    std::ostream& operator << (std::ostream &stream, const RecordField &field)
    {
        stream << std::to_string (field.tag) << std::string ("#") << field.value;
        for (const auto &sub : field.subfields) {
            stream << sub;
        }

        return stream;
    }

    /// \brief Добавление подполя в конец поля.
    /// \param subfield Подполе для добавления.
    /// \return this.
    RecordField& RecordField::operator << (const SubField &subfield)
    {
        this->subfields.push_back (subfield);
        return *this;
    }

    /// \brief Добавление подполя в конец поля.
    /// \param subfield Подполе для добавления.
    /// \return this.
    RecordField& RecordField::operator << (SubField &&subfield)
    {
        this->subfields.push_back (std::move (subfield));
        return *this;
    }

    /// \brief Изменение значения поля. Если в записи присутствуют подполя, то изменяется значение последнего подполя.
    /// \param body Новое значение поля.
    /// \return this.
    RecordField& RecordField::operator << (const std::string &body)
    {
        if (contains(body, '^')) {
            this->decodeBody (body);
        }
        else {
            if (this->subfields.empty()) {
                this->value = body;
            }
            else {
                this->subfields.back().value = body;
            }
        }
        return *this;
    }

    /// \brief Изменение значения поля. Если в записи присутствуют подполя, то изменяется значение последнего подполя.
    /// \param body Новое значение поля.
    /// \return this.
    RecordField& RecordField::operator << (std::string &&body)
    {
        if (contains(body, '^')) {
            this->decodeBody (body);
        }
        else {
            if (this->subfields.empty()) {
                this->value = std::move (body);
            }
            else {
                this->subfields.back().value = std::move (body);
            }
        }
        return *this;
    }

    /// \brief Изменение значения поля.
    /// \param body Новое значение поля.
    /// \return this.
    RecordField& RecordField::operator << (const char *body)
    {
        return *this << std::string (body);
    }

    /// \brief Добавление подполя в конец поля.
    /// \param code Код подполя.
    /// \return this.
    RecordField& RecordField::operator << (char code)
    {
        return *this << SubField (code);
    }

    //================================================================

    /// \brief Конструктор.
    /// \param fields_ Список полей.
    MarcRecord::MarcRecord  (std::initializer_list <RecordField> fields_)
        : fields { fields_ }
    {
        // пустое тело конструктора
    }

    /// \brief Добавление в конец записи поля с указанными меткой и значением.
    /// \param tag Метка добавляемого поля.
    /// \param value Значение поля (может быть пустым).
    /// \return Вновь созданное поле.
    RecordField& MarcRecord::add (int tag, const std::string &value)
    {
        this->fields.emplace_back (tag, value);
        return this->fields.back();
    }

    /// \brief Добавление в конец записи поля с указанными меткой и значением.
    /// \param tag Метка добавляемого поля.
    /// \param value Значение поля (может быть пустым).
    /// \return Вновь созданное поле.
    RecordField& MarcRecord::add (int tag, std::string &&value)
    {
        this->fields.emplace_back (tag, std::move (value));
        return this->fields.back();
    }

    /// \brief Создание клона записи.
    /// \return Клон записи.
    MarcRecord MarcRecord::clone() const
    {
        MarcRecord result;
        result.mfn      = this->mfn;
        result.status   = this->status;
        result.version  = this->version;
        result.database = this->database;
        for (const auto &one : this->fields) {
            result.fields.push_back (std::move (one.clone()));
        }

        return result;
    }

    /// \brief Разбор текстового представления записи.
    /// \param lines Строки с полями записи.
    void MarcRecord::decode (const StringList &lines)
    {
        if (lines.size() < 2) {
            return;
        }

        // mfn and status of the record
        const auto firstLine = split (lines[0], '#');
        this->mfn = fastParseUnsigned32 (firstLine[0]);
        this->status = static_cast<RecordStatus> (fastParseUnsigned32 (safeAt (firstLine, 1)));

        // version of the record
        const auto secondLine = split (lines[1], '#');
        this->version = fastParseUnsigned32 (safeAt (secondLine, 1));

        // fields
        for (std::size_t i = 2; i < lines.size(); i++) {
            const auto &line = lines[i];
            if (!line.empty()) {
                RecordField field;
                field.decode (line);
                this->fields.push_back (field);
            }
        }
    }

    /// \brief Запись удалена (логически или физически)?
    /// \return true если удалена.
    bool MarcRecord::deleted() const noexcept
    {
        return (this->status & RecordStatus::Deleted) != RecordStatus::None;
    }

    /// \brief Кодирование записи в текстовую форму.
    /// \param delimiter Разделитель строк.
    /// \return Текстовое представление записи.
    std::string MarcRecord::encode (const std::string &delimiter) const
    {
        std::string result = std::to_string (this->mfn) + "#"
                             + std::to_string (static_cast<int> (this->status)) + delimiter
                             + "0#" + std::to_string(this->version) + delimiter;
        for (const auto &field : this->fields) {
            result.append (field.toString());
            result.append (delimiter);
        }
        return result;
    }

    /// \brief Получение значения поля/подполя.
    /// \param tag Метка поля.
    /// \param code Код подполя (опционально).
    /// \return Значение поля/подполя либо пустая строка.
    std::string MarcRecord::fm (int tag, char code) const noexcept
    {
        for (const auto &field : this->fields) {
            if (field.tag == tag) {
                if (code) {
                    for (const auto &subfield : field.subfields) {
                        if (sameChar (subfield.code, code)) {
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
    StringList MarcRecord::fma (int tag, char code) const
    {
        StringList result;
        for (const auto &field : this->fields) {
            if (field.tag == tag) {
                if (code) {
                    for (const auto &subfield : field.subfields) {
                        if (sameChar (subfield.code, code)) {
                            if (!subfield.value.empty()) {
                                result.push_back (subfield.value);
                            }
                        }
                    }
                } else {
                    if (!field.value.empty()) {
                        result.push_back (field.value);
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
    RecordField* MarcRecord::getField (int tag, int occurrence) const noexcept
    {
        for (const auto &field : this->fields) {
            if (field.tag == tag) {
                if (!occurrence) {
                    return const_cast<RecordField*> (&field);
                }
                --occurrence;
            }
        }
        return nullptr;
    }

    /// \brief Получение вектора указателей на поля с указанной меткой.
    /// \param tag Метка поля.
    /// \return Вектор указателей (возможно, пустой).
    std::vector<RecordField*> MarcRecord::getFields (int tag) const
    {
        std::vector<RecordField*> result;
        for (const auto &field : this->fields) {
            if (field.tag == tag) {
                auto ptr = const_cast<RecordField*>(&field);
                result.push_back(ptr);
            }
        }
        return result;
    }

    /// \brief Удаление всех повторений поля с указанной меткой.
    /// \param tag Метка поля.
    /// \return this.
    MarcRecord& MarcRecord::removeField (int tag)
    {
        this->fields.erase
            (
                std::remove_if
                    (
                        this->fields.begin(),
                        this->fields.end(),
                        [tag] (RecordField &field) { return field.tag == tag; }
                    ),
                this->fields.end()
            );

        return *this;
    }

    /// \brief Установка значения поля.
    /// \param tag Метка поля.
    /// \param value Новое значение поля.
    /// \return this.
    MarcRecord& MarcRecord::setField
        (
            int tag,
            const std::string &value
        )
    {
        if (value.empty()) {
            return this->removeField (tag);
        }
        auto field = this->getField (tag);
        if (!field) {
            field = &this->add (tag);
        }
        field->value = value;

        return *this;
    }

    /// \brief Установка значения поля.
    /// \param tag Метка поля.
    /// \param value Новое значение поля.
    /// \return this.
    MarcRecord& MarcRecord::setField (int tag, std::string &&value)
    {
        if (value.empty()) {
            return this->removeField (tag);
        }
        auto field = this->getField (tag);
        if (!field) {
            field = &this->add (tag);
        }
        field->value = std::move (value);
        return *this;
    }

    /// \brief Сброс состояния записи.
    /// \return this.
    /// \details Может потребоваться, например,
    /// при переносе записи в другую базу данных.
    MarcRecord& MarcRecord::reset() noexcept
    {
        this->mfn = 0;
        this->status = RecordStatus::None;
        this->version = 0;
        this->database.clear();
        return *this;
    }


    std::ostream& operator << (std::ostream &stream, const MarcRecord &record)
    {
        stream << std::to_string (record.mfn) << std::string ("#")
               << std::to_string (static_cast<int> (record.status)) << std::endl;
        stream << std::string ("0#") << record.version << std::endl;
        for (const RecordField &field : record.fields) {
            stream << field << std::endl;
        }

        return stream;
    }

    /// \brief Добавление поля в конец записи.
    /// \param field Поле для дополнения.
    /// \return this.
    MarcRecord& MarcRecord::operator << (const RecordField &field)
    {
        this->fields.push_back (field);
        return *this;
    }

    /// \brief Добавление поля в конец записи.
    /// \param field Поле для дополнения.
    /// \return this.
    MarcRecord& MarcRecord::operator << (RecordField &&field)
    {
        this->fields.push_back (std::move (field));
        return *this;
    }

    /// \brief Добавление поля в конец записи.
    /// \param text Поле для дополнения в закодированном виде.
    /// \return this.
    MarcRecord& MarcRecord::operator << (const std::string &text)
    {
        if (contains (text, '#')) {
            this->fields.emplace_back ();
            auto &field = this->fields.back ();
            field.decode (text);
        }
        else {
            auto &field = this->fields.back ();
            if (contains (text, '^')) {
                field.decodeBody (text);
            }
            else {
                if (field.subfields.empty ()) {
                    field.value = text;
                }
                else {
                    field.subfields.back().value = text;
                }
            }
        }
        return *this;
    }

    /// \brief Добавление поля в конец записи.
    /// \param tag Метка поля.
    /// \return this.
    MarcRecord& MarcRecord::operator << (int tag)
    {
        return *this << RecordField (tag);
    }

    /// \brief Добавление подполя в конец последнего поля записи.
    /// \param code Код подполя.
    /// \return this.
    MarcRecord& MarcRecord::operator << (char code)
    {
        this->fields.back() << code;
        return *this;
    }

    /// \brief Установка значения последнего подполя последнего поля записи.
    /// \param value Значение.
    /// \return this.
    MarcRecord& MarcRecord::operator && (const std::string &value)
    {
        this->fields.back().subfields.back().value = value;
        return *this;
    }

    /// \brief Установка значения последнего подполя последнего поля записи.
    /// \param value Значение.
    /// \return this.
    MarcRecord& MarcRecord::operator && (const char *value)
    {
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
        )
    {
        this->addAnsi (commandCode)             .newLine();
        this->addAnsi (connection.workstation)  .newLine();
        this->addAnsi (commandCode)             .newLine();
        this->add     (connection.clientId)    .newLine();
        this->add     (connection.queryId)     .newLine();
        this->addAnsi (connection.password)     .newLine();
        this->addAnsi (connection.username)     .newLine();
        this->                                       newLine();
        this->                                       newLine();
        this->                                       newLine();
    }

    /// \brief Добавление информации к запросу.
    /// \param bytes Массив с информацией.
    /// \param size Количество байт.
    void ClientQuery::_write (const unsigned char *bytes, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i++) {
            this->_content.push_back (bytes[i]);
        }
    }

    /// \brief Добавление байта к запросу.
    /// \param byte Добавляемый байт.
    void ClientQuery::_write (unsigned char byte)
    {
        _content.push_back (byte);
    }

    /// \brief Добавление целого числа к запросу.
    /// \param value Добавляемое значение.
    /// \return this.
    ClientQuery& ClientQuery::add (int value)
    {
        const auto s = std::to_string (value);
        return this->addAnsi (s);
    }

    /// \brief Добавление файловой спецификации к запросу.
    /// \param specification Спецификация файла.
    /// \return this.
    ClientQuery& ClientQuery::add (const FileSpecification &specification)
    {
        // return this->addAnsi (specification.toString());
        return *this;
    }

    /// \brief Добавление строки в кодировке ANSI.
    /// \param text Добавляемый текст.
    /// \return `this`.
    /// \warning Предполагается, что переданный текст в кодировке ANSI!
    ClientQuery& ClientQuery::addAnsi (const std::string &text)
    {
        const std::size_t size = text.length();
        if (!size) {
            return *this;
        }

        const auto *ptr = reinterpret_cast<const unsigned char*> (text.c_str());
        this->_write (ptr, size);
        return *this;
    }

    /// \brief Добавление строки в формате UTF-8.
    /// \param text Добавляемый текст.
    /// \return this.
    ClientQuery& ClientQuery::addUtf (const std::string &text)
    {
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
    void ClientQuery::dump (std::ostream &stream) const
    {
        for (const auto value : this->_content) {
            // stream << std::hex << std::setw(2) << value << " ";
            stream << value;
        }

        stream.flush();
    }

    /// \brief Кодирование запроса.
    /// \return Закодированный запрос.
    std::vector<unsigned char> ClientQuery::encode() const
    {
        std::vector<unsigned char> result;
        result.reserve (this->_content.size() + 10);
        const auto prefix = std::to_string (this->_content.size());
        const auto ptr = prefix.c_str();
        for (std::size_t i = 0; i < prefix.length(); i++) {
            result.push_back (static_cast<unsigned char>(ptr[i]));
        }
        result.push_back (0x0a);
        result.insert (result.end(), this->_content.begin(), this->_content.end());

        return result;
    }

    /// \brief Добавление перевода строки.
    /// \return `this`.
    ClientQuery& ClientQuery::newLine()
    {
        this->_write (0x0A);
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
        )
    {
        std::lock_guard<std::mutex> guard (connection._mutex);

        this->_connection = &connection;
        this->returnCode  = 0;
        this->_success    = false;
        this->_position   = 0;

        // Read the response
        auto &socket = *connection.socket;
        socket.host  = connection.host;
        socket.port  = connection.port;
        socket.open();

        auto encoded    = query.encode();
        const auto data = encoded.data();
        const auto size = encoded.size();
        socket.send (data, size);

        unsigned char buffer[2048];
        while(true) {
            const auto received = socket.receive (buffer, sizeof (buffer));
            if (received <= 0) {
                break;
            }
            _write (buffer, received);
        }
        socket.close();

        // decode the response
        this->command       = this->readAnsi();
        this->clientId      = this->readInteger();
        this->queryId       = this->readInteger();
        this->answerSize    = this->readInteger();
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
    bool ServerResponse::checkReturnCode()
    {
        return this->checkReturnCode(0);
    }

    /// \brief Проверка, достигнут ли конец ответа сервера.
    /// \return true, если достигнут конец, иначе false.
    bool ServerResponse::eot() const
    {
        return this->_position >= this->_content.size();
    }

    /// \brief Проверка кода возврата последней операции, выполненной сервером.
    /// \param nargs Количество разрешённых кодов. Далее идут разрешённые коды (отрицательные числа, не означающие сбой при выполнении операции).
    /// \return Результат проверки: true означает успешное завершение операии.
    ///
    /// В любом случае у соединения выставляется значение поля lastError.
    bool ServerResponse::checkReturnCode (int nargs, ...)
    {
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
            for(auto i = 0; i < nargs; i++) {
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
    std::string ServerResponse::getLine()
    {
        std::string result;
        const auto size = this->_content.size();
        while (this->_position < size) {
            auto c = static_cast<char> (this->_content.at (_position));
            this->_position++;

            if (c == 13) {
                if (this->_position < size) {
                    c = static_cast<char> (this->_content.at (_position));
                    if (c == 10) {
                        this->_position++;
                    }
                }
                break;
            }

            result.push_back (c);
        }

        return result;
    }

    /// \brief Чтение оставшейся части ответа сервера без преобразования кодировок.
    /// \return Прочитанный ответ сервера. Если достигнут конец, строка будет пустая.
    std::string ServerResponse::getRemaining()
    {
        std::string result;
        const auto size = this->_content.size();
        if (this->_position < size) {
            const auto data = reinterpret_cast<const char*> (_content.data() + this->_position);
            std::size_t remaining = size - this->_position;
            // Убираем переводы строки в конце.
            while (remaining > 0) {
                const auto c = data[remaining-1];
                if (c != '\r' && c != '\n') {
                    break;
                }
                remaining--;
            }
            const std::string s2 (data, remaining);
            result = s2;
            this->_position = size;
        }

        return result;
    }

    /// \brief Получение кода возврата.
    /// \return Полученный от сервера код возврата. Отрицательное значение свидетельствует о проблеме (но не всегда).
    ///
    /// Получение кода возврата. Вызывается один раз в свой час и только тогда.
    int ServerResponse::getReturnCode()
    {
        this->returnCode = this->readInteger();
        return this->returnCode;
    }

    /// \brief Чтение строки в кодировке ANSI.
    /// \return Полученная строка. Если достигнут конец ответа, строка будет пустой.
    std::string ServerResponse::readAnsi()
    {
        auto line = this->getLine();
        return line;
    }

    /// \brief Чтение целого числа.
    /// \return Прочитанное число.
    ///
    /// Предполагается, что число занимает всю строку.
    int ServerResponse::readInteger()
    {
        const auto line = this->getLine();
        return std::stoi (line);
    }

    /// \brief Чтение оставшихся строк в кодировке ANSI.
    /// \return Вектор прочитанных строк.
    StringList ServerResponse::readRemainingAnsiLines()
    {
        const auto text = this->readRemainingAnsiText();
        return split (text, '\n');
    }

    /// \brief Чтение оставшегося текста в кодировке ANSI.
    /// \return Прочитанный текст.
    std::string ServerResponse::readRemainingAnsiText()
    {
        auto line = this->getRemaining();
        return line;
    }

    /// \brief Чтение оставшихся строк в кодировке UTF-8.
    /// \return Вектор прочитанных строк.
    StringList ServerResponse::readRemainingUtfLines()
    {
        StringList result;
        while (!this->eot()) {
            const auto line = readUtf();
            result.push_back (line);
        }
        return result;
    }

    /// \brief Чтение оставшихся строк в кодировке UTF-8.
    /// \return Вектор прочитанных строк.
    std::vector<std::string> ServerResponse::readRemainingLinesUtf()
    {
        std::vector<std::string> result;
        while (!this->eot()) {
            const auto line = getLine();
            result.push_back (line);
        }
        return result;
    }

    /// \brief Чтение оставшегося текста в кодировке UTF-8.
    /// \return Прочитанный текст.
    std::string ServerResponse::readRemainingUtfText()
    {
        auto line = getRemaining();
        return line;
    }

    /// \brief Чтение строки в кодировке UTF-8.
    /// \return Полученная строка. Если достигнут конец ответа, строка будет пустой.
    std::string ServerResponse::readUtf()
    {
        auto line = this->getLine();
        return line;
    }

    /// \brief Сетевой обмен с сервером завершился успешно?
    /// \return Признак успешности.
    bool ServerResponse::success() const
    {
        return this->_success;
    }

    void ServerResponse::_write(const unsigned char *bytes, std::size_t size)
    {
        const auto newSize = this->_content.size() + size;
        if (this->_content.capacity() < newSize) {
            this->_content.reserve(newSize);
        }

        for (std::size_t i = 0; i < size; i++) {
            this->_content.push_back (bytes[i]);
        }
    }

    /// \brief Создание пустого ответа сервера (для целей тестирования).
    ServerResponse* ServerResponse::emptyResonse()
    {
        auto result = new ServerResponse();
        return result;
    }

    //================================================================

    RecordStatus operator | (RecordStatus left, RecordStatus right)
    {
        return static_cast <RecordStatus > (
            static_cast <std::underlying_type<RecordStatus>::type> (left)
            | static_cast <std::underlying_type<RecordStatus>::type> (right)
        );
    }

    RecordStatus operator & (RecordStatus left, RecordStatus right)
    {
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
        )
    {
        const auto lines = response.readRemainingAnsiLines();
        if (lines.size() == 3) {
            this->version          = lines[0];
            this->connectedClients = fastParse32 (lines[1]);
            this->maxClients       = fastParse32 (lines[2]);
        } else {
            this->organization     = lines[0];
            this->version          = lines[1];
            this->connectedClients = fastParse32 (lines[2]);
            this->maxClients       = fastParse32 (lines[3]);
        }
    }

    /// \brief Строковое представление.
    /// \return Строковое представление.
    std::string Version::toString() const
    {
        return std::string ("organization=") + this->organization +
               ", version=" + this->version +
               ", maxClients=" + std::to_string (this->maxClients) +
               ", connectedClients=" + std::to_string (this->connectedClients);
    }

    //================================================================

    class ClientSocketImpl final
    {
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
        : _impl (new ClientSocketImpl)
    {
        this->host = host;
        this->port = port;

        #ifdef WIN32

        if (WSAStartup (0x0202, &this->_impl->wsaData)) {
            throw std::exception();
        }

        #endif
    }

    ClientSocket::~ClientSocket()
    {
        #ifdef WIN32

        WSACleanup();

        #endif
    }


    void ClientSocket::open()
    {
        this->_impl->socket = socket (AF_INET, SOCK_STREAM, 0);

        sockaddr_in destinationAddress {};
        destinationAddress.sin_family = AF_INET;
        destinationAddress.sin_port = htons (this->port);
        auto ansi = this->host;
        auto inaddr = inet_addr (ansi.c_str());
        if (inaddr != INADDR_NONE) {
            destinationAddress.sin_addr.s_addr = inaddr;
        }
        else {

            HOSTENT *hostentry = gethostbyname (ansi.c_str());
            if (hostentry) {
                ((unsigned long*) &destinationAddress.sin_addr)[0] =
                    ((unsigned long **) hostentry->h_addr_list)[0][0];
            }
            else {
                throw std::exception();
            }
        }

        if (connect (this->_impl->socket, (sockaddr*)&destinationAddress, sizeof(destinationAddress))) {
            throw std::exception();
        }
    }

    void ClientSocket::close()
    {
        closesocket (this->_impl->socket);
    }

    void ClientSocket::send
        (
            const unsigned char *buffer,
            std::size_t size
        )
    {
        const auto ptr = reinterpret_cast <const char*> (buffer);
        const auto size2 = static_cast <int> (size);
        ::send (this->_impl->socket, ptr, size2, 0);
    }

    std::size_t ClientSocket::receive
        (
            unsigned char *buffer,
            std::size_t size
        )
    {
        const auto ptr = reinterpret_cast <char*> (buffer);
        const auto size2 = static_cast <int> (size);
        const std::size_t result = ::recv (this->_impl->socket, ptr, size2, 0);
        return result;
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

