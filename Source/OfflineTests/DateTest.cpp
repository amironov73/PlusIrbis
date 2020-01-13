#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

// ReSharper disable StringLiteralTypo

namespace OfflineTests
{
    TEST_CLASS(DateTest)
    {
    public:

        TEST_METHOD(constructor_1)
        {
            const Date date(2020, 1, 13);
            Assert::IsTrue(date.date.tm_year == 120);
            Assert::IsTrue(date.date.tm_mon == 0);
            Assert::IsTrue(date.date.tm_mday == 13);
            Assert::IsTrue(date.text == L"20200113");
        }

        TEST_METHOD(constructor_2)
        {
            const Date date(L"20200113");
            Assert::IsTrue(date.date.tm_year == 120);
            Assert::IsTrue(date.date.tm_mon == 0);
            Assert::IsTrue(date.date.tm_mday == 13);
            Assert::IsTrue(date.text == L"20200113");
        }

        TEST_METHOD(constructor_3)
        {
            struct tm t {0};
            t.tm_year = 120;
            t.tm_mon = 0;
            t.tm_mday = 13;
            const Date date(&t);
            Assert::IsTrue(date.date.tm_year == 120);
            Assert::IsTrue(date.date.tm_mon == 0);
            Assert::IsTrue(date.date.tm_mday == 13);
            Assert::IsTrue(date.text == L"20200113");
        }

        TEST_METHOD(convert_1)
        {
            struct tm t {0};
            t.tm_year = 120;
            t.tm_mon = 0;
            t.tm_mday = 13;
            Assert::IsTrue(Date::convert(&t) == L"20200113");
        }

        TEST_METHOD(parse_1)
        {
            struct tm t {0};
            String s(L"20200113");
            Assert::IsTrue(Date::parse(s, &t));
            Assert::IsTrue(t.tm_year == 120);
            Assert::IsTrue(t.tm_mon == 0);
            Assert::IsTrue(t.tm_mday == 13);
        }

        TEST_METHOD(today_1)
        {
            const Date date = Date::today();
            Assert::IsTrue(date.text.size() == 8);
            Logger::WriteMessage(std::to_string(date.date.tm_year).data());
            Logger::WriteMessage(std::to_string(date.date.tm_mon).data());
            Logger::WriteMessage(std::to_string(date.date.tm_mday).data());
        }
    };
}
