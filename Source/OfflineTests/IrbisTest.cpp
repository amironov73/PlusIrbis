#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

// ReSharper disable StringLiteralTypo

namespace OfflineTests
{
    TEST_CLASS(IrbisTest)
    {
    public:

        TEST_METHOD(sameChar_1)
        {
            Assert::IsTrue(sameChar(L'a', L'A'));
            Assert::IsTrue(sameChar(L'a', L'a'));
            Assert::IsFalse(sameChar(L'1', L'2'));
            Assert::IsTrue(sameChar(L'1', L'1'));
        }

        TEST_METHOD(sameString_1)
        {
            Assert::IsTrue(sameString(L"Hello", L"HELLO"));
            Assert::IsTrue(sameString(L"Hello", L"Hello"));
            Assert::IsTrue(sameString(L"Hello", L"hello"));
            Assert::IsFalse(sameString(L"123", L"321"));
            Assert::IsFalse(sameString(L"123", L"1234"));
            Assert::IsTrue(sameString(L"123", L"123"));
        }

        TEST_METHOD(toLower_1)
        {
            String text(L"Hello");
            Assert::IsTrue(toLower(text) == L"hello");
            Assert::IsTrue(toLower(text) == L"hello");
        }

        TEST_METHOD(toUpper_1)
        {
            String text(L"Hello");
            Assert::IsTrue(toUpper(text) == L"HELLO");
            Assert::IsTrue(toUpper(text) == L"HELLO");
        }
    };
}
