#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(SubFieldTest)
    {
    public:

        TEST_METHOD(constructor_1)
        {
            SubField sub;
            Assert::IsTrue(sub.empty());
        }

        TEST_METHOD(constructor_2)
        {
            SubField sub{'a'};
            Assert::IsTrue(sub.empty());
            Assert::IsTrue(sub.code == 'a');
            Assert::IsTrue(sub.wstr() == L"^a");
        }

        TEST_METHOD(constructor_3)
        {
            SubField sub{'a', L"SubfieldA"};
            Assert::IsFalse(sub.empty());
            Assert::IsTrue(sub.code == 'a');
            Assert::IsTrue(sub.value == L"SubfieldA");
            Assert::IsTrue(sub.wstr() == L"^aSubfieldA");
        }

        TEST_METHOD(verify_1)
        {
            SubField sub;
            Assert::IsFalse(sub.verify(false));
        }

        TEST_METHOD(verify_2)
        {
            SubField sub{'a'};
            Assert::IsFalse(sub.verify(false));
        }

        TEST_METHOD(verify_3)
        {
            SubField sub{'a', L"SubA"};
            Assert::IsTrue(sub.verify(false));
        }

    };
}
