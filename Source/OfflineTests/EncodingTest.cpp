#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(EncodingTest)
    {
    public:

        TEST_METHOD(cp1251_1)
        {
            const std::string source = "\xD3\x20\xEF\xEE\xEF\xE0\x20\xE1\xFB\xEB\xE0\x20\xF1\xEE\xE1\xE0\xEA\xE0";
            const std::wstring destination = cp1251_to_unicode(source);
            Assert::AreEqual(std::wstring(L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430"), destination);
        }

        TEST_METHOD(cp1251_2)
        {
            const std::wstring source = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
            const std::string destination = unicode_to_cp1251(source);
            Assert::AreEqual(std::string("\xD3\x20\xEF\xEE\xEF\xE0\x20\xE1\xFB\xEB\xE0\x20\xF1\xEE\xE1\xE0\xEA\xE0"), destination);
        }

        TEST_METHOD(toUtf_1)
        {
            wchar_t text[] = L"Hello! \u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
            BYTE buffer[1024], *ptr;
            ptr = toUtf(buffer, text, wcslen(text));
            const int expected = 40;
            const int actual = static_cast<int>(ptr - buffer);
            Assert::AreEqual(expected, actual);
            BYTE goodBytes[40] {0x48, 0x65, 0x6C, 0x6C, 0x6F,
                0x21, 0x20, 0xD0, 0xA3, 0x20, 0xD0, 0xBF, 0xD0,
                0xBE, 0xD0, 0xBF, 0xD0, 0xB0, 0x20, 0xD0, 0xB1,
                0xD1, 0x8B, 0xD0, 0xBB, 0xD0, 0xB0, 0x20, 0xD1,
                0x81, 0xD0, 0xBE, 0xD0, 0xB1, 0xD0, 0xB0, 0xD0,
                0xBA, 0xD0, 0xB0 };
            for (int i = 0; i < 40; i++)
            {
                Assert::AreEqual(goodBytes[i], buffer[i]);
            }
        }

        TEST_METHOD(toUtf_2)
        {
            const std::wstring text = L"Hello! \u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
            BYTE buffer[1024], *ptr;
            ptr = toUtf(buffer, text);
            const int expected = 40;
            const int actual = static_cast<int>(ptr - buffer);
            Assert::AreEqual(expected, actual);
            BYTE goodBytes[40] {0x48, 0x65, 0x6C, 0x6C, 0x6F,
                0x21, 0x20, 0xD0, 0xA3, 0x20, 0xD0, 0xBF, 0xD0,
                0xBE, 0xD0, 0xBF, 0xD0, 0xB0, 0x20, 0xD0, 0xB1,
                0xD1, 0x8B, 0xD0, 0xBB, 0xD0, 0xB0, 0x20, 0xD1,
                0x81, 0xD0, 0xBE, 0xD0, 0xB1, 0xD0, 0xB0, 0xD0,
                0xBA, 0xD0, 0xB0 };
            for (int i = 0; i < 40; i++)
            {
                Assert::AreEqual(goodBytes[i], buffer[i]);
            }
        }

        TEST_METHOD(fromUtf_1)
        {
            BYTE text[40] {0x48, 0x65, 0x6C, 0x6C, 0x6F,
                0x21, 0x20, 0xD0, 0xA3, 0x20, 0xD0, 0xBF, 0xD0,
                0xBE, 0xD0, 0xBF, 0xD0, 0xB0, 0x20, 0xD0, 0xB1,
                0xD1, 0x8B, 0xD0, 0xBB, 0xD0, 0xB0, 0x20, 0xD1,
                0x81, 0xD0, 0xBE, 0xD0, 0xB1, 0xD0, 0xB0, 0xD0,
                0xBA, 0xD0, 0xB0 };
            wchar_t buffer[1024], *ptr;
            ptr = fromUtf(buffer, text, 40);
            int expected = 25;
            int actual = static_cast<int>(ptr - buffer);
            Assert::AreEqual(expected, actual);
            wchar_t goodChars[] = L"Hello! \u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
            for (int i = 0; i < 25; i++)
            {
                Assert::AreEqual(goodChars[i], buffer[i]);
            }
        }

        TEST_METHOD(fromUtf_2)
        {
            BYTE src[] {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21,
                0x10, 0x20, 0xD0, 0xA3, 0x20, 0xD0, 0xBF, 0xD0,
                0xBE, 0xD0, 0xBF, 0xD0, 0xB0, 0x20, 0xD0, 0xB1,
                0xD1, 0x8B, 0xD0, 0xBB, 0xD0, 0xB0, 0x20, 0xD1,
                0x81, 0xD0, 0xBE, 0xD0, 0xB1, 0xD0, 0xB0, 0xD0,
                0xBA, 0xD0, 0xB0 };
            size_t size = sizeof(src);
            std::wstring result;
            const BYTE *end = fromUtf(src, size, 0x10, result);
            Assert::AreEqual(std::wstring(L"Hello!"), result);
            end++;
            size--;
            end++;
            size--;
            fromUtf(end, size, 0x10, result);
            Assert::AreEqual(std::wstring(L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430"), result);
        }
    };
}
