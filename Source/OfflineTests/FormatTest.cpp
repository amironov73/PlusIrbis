#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(FormatTest)
    {
    public:

        TEST_METHOD(removeComments_1)
        {
            Assert::AreEqual(removeComments(L""), std::wstring(L""));
            Assert::AreEqual(removeComments(L"Hello"), std::wstring(L"Hello"));
            Assert::AreEqual(removeComments(L"Hello/*comment"), std::wstring(L"Hello"));
        }

        TEST_METHOD(prepareFormat_1)
        {
            Assert::AreEqual(prepareFormat(L""), std::wstring(L""));
            Assert::AreEqual(prepareFormat(L"Hello"), std::wstring(L"Hello"));
            Assert::AreEqual(prepareFormat(L"Hello/*comment"), std::wstring(L"Hello"));
            Assert::AreEqual(prepareFormat(L"Hel\rlo"), std::wstring(L"Hello"));
        }

    };
}
