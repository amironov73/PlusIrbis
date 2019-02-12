#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(FormatTest)
    {
    public:

        TEST_METHOD(removeComments_1)
        {
            Assert::AreEqual(Format::removeComments(L""), std::wstring(L""));
            Assert::AreEqual(Format::removeComments(L"Hello"), std::wstring(L"Hello"));
            Assert::AreEqual(Format::removeComments(L"Hello/*comment"), std::wstring(L"Hello"));
        }

        TEST_METHOD(prepareFormat_1)
        {
            Assert::AreEqual(Format::prepareFormat(L""), std::wstring(L""));
            Assert::AreEqual(Format::prepareFormat(L"Hello"), std::wstring(L"Hello"));
            Assert::AreEqual(Format::prepareFormat(L"Hello/*comment"), std::wstring(L"Hello"));
            Assert::AreEqual(Format::prepareFormat(L"Hel\rlo"), std::wstring(L"Hello"));
        }

    };
}
