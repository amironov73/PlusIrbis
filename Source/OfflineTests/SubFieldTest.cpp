#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

// ReSharper disable StringLiteralTypo

namespace OfflineTests
{
    TEST_CLASS(SubFieldTest)
    {
    public:

        TEST_METHOD(constructor_1)
        {
            const SubField sub;
            Assert::IsTrue(sub.empty());
        }

        TEST_METHOD(constructor_2)
        {
            const SubField sub{'a', nullptr};
            Assert::IsTrue(sub.empty());
            Assert::IsTrue(sub.code == 'a');
            Assert::IsTrue(sub.wstr() == L"^a");
        }

        TEST_METHOD(constructor_3)
        {
            const SubField sub{'a', L"SubfieldA"};
            Assert::IsFalse(sub.empty());
            Assert::IsTrue(sub.code == 'a');
            Assert::IsTrue(sub.value == L"SubfieldA");
            Assert::IsTrue(sub.wstr() == L"^aSubfieldA");
        }

        TEST_METHOD(verify_1)
        {
            const SubField sub;
            Assert::IsFalse(sub.verify(false));
        }

        TEST_METHOD(verify_2)
        {
            const SubField sub{'a', nullptr};
            Assert::IsFalse(sub.verify(false));
        }

        TEST_METHOD(verify_3)
        {
            const SubField sub{'a', L"SubA"};
            Assert::IsTrue(sub.verify(false));
        }

    };
}
