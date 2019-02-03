#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(RecordFieldTest)
    {
    public:

        TEST_METHOD(constructor_1)
        {
            RecordField field;
            Assert::IsTrue(field.empty());
            Assert::IsTrue(field.subfields.empty());
        }

        TEST_METHOD(constructor_2)
        {
            RecordField field(100);
            Assert::IsTrue(field.empty());
            Assert::IsTrue(field.subfields.empty());
            Assert::IsTrue(field.tag == 100);
        }

        TEST_METHOD(constructor_3)
        {
            RecordField field(100, L"Field100");
            Assert::IsFalse(field.empty());
            Assert::IsTrue(field.tag == 100);
            Assert::IsTrue(field.value == L"Field100");
            Assert::IsTrue(field.subfields.empty());
            Assert::IsTrue(field.wstr() == L"100#Field100");
        }

        TEST_METHOD(constructor_4)
        {
            RecordField field(100);
            field.add('a', L"SubA").add('b', L"SubB");
            Assert::IsFalse(field.empty());
            Assert::IsTrue(field.value.empty());
            Assert::IsTrue(field.subfields.size() == 2);
            Assert::IsTrue(field.wstr() == L"100#^aSubA^bSubB");
        }

        TEST_METHOD(verify_1)
        {
            RecordField field;
            Assert::IsFalse(field.verify(false));
        }

        TEST_METHOD(verify_2)
        {
            RecordField field(100);
            Assert::IsFalse(field.verify(false));
        }

        TEST_METHOD(verify_3)
        {
            RecordField field(100, L"Field100");
            Assert::IsTrue(field.verify(false));
        }

        TEST_METHOD(verify_4)
        {
            RecordField field(100);
            field.add('a', L"SubA").add('b', L"SubB");
            Assert::IsTrue(field.verify(false));
        }

    };
}
