#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(EncodingTest)
    {
    public:

        TEST_METHOD(cp1251_1)
        {
            const std::string source = "У попа была собака";
            const std::wstring destination = cp1251_to_unicode(source);
            Assert::AreEqual(std::wstring(L"У попа была собака"), destination);
        }

    };
}
