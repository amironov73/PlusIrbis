#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(UtilsTest)
    {
    public:

        TEST_METHOD(iif_1)
        {
            const std::string s1 = "Hello", s2 = "World";
            const std::string s3 = iif(s1, s2);
            Assert::AreEqual(s1, s3);
        }

        TEST_METHOD(iif_2)
        {
            const std::string s1, s2 = "World";
            const std::string s3 = iif(s1, s2);
            Assert::AreEqual(s2, s3);
        }

        TEST_METHOD(iif_3)
        {
            const std::string s1 = "Hello", s2 = "World", s3 = "Again";
            const std::string s4 = iif(s1, s2, s3);
            Assert::AreEqual(s1, s4);
        }

        TEST_METHOD(iif_4)
        {
            const std::string s1, s2 = "World", s3 = "Again";
            const std::string s4 = iif(s1, s2, s3);
            Assert::AreEqual(s2, s4);
        }

        TEST_METHOD(iif_5)
        {
            const std::string s1, s2, s3 = "Again";
            const std::string s4 = iif(s1, s2, s3);
            Assert::AreEqual(s3, s4);
        }

        TEST_METHOD(safeAt_1)
        {
            std::vector<std::wstring> v{L"1", L"3", L"5"};
            std::wstring i = safeAt(v, 1);
            Assert::AreEqual(i, std::wstring(L"3"));
        }

        TEST_METHOD(safeAt_2)
        {
            std::vector<std::wstring> v{L"1", L"3", L"5"};
            std::wstring i = safeAt(v, 11);
            Assert::AreEqual(i, std::wstring());
        }

        TEST_METHOD(maxSplit_1)
        {
            std::wstring text(L"Hello,world,again");
            std::vector<std::wstring> a = maxSplit(text, ',', 2);
            std::vector<std::wstring>::size_type expected = 2;
            Assert::AreEqual(expected, a.size());
            Assert::AreEqual(a[0], std::wstring(L"Hello"));
            Assert::AreEqual(a[1], std::wstring(L"world,again"));
        }

        TEST_METHOD(maxSplit_2)
        {
            std::wstring text(L"Hello,world,again");
            std::vector<std::wstring> a = maxSplit(text, ',', 3);
            std::vector<std::wstring>::size_type expected = 3;
            Assert::AreEqual(expected, a.size());
            Assert::AreEqual(a[0], std::wstring(L"Hello"));
            Assert::AreEqual(a[1], std::wstring(L"world"));
            Assert::AreEqual(a[2], std::wstring(L"again"));
        }

    };

}