#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(SpanTest)
    {
    public:

        TEST_METHOD(constructor_1)
        {
            const Span<int> span;
            Assert::IsTrue(span.empty());
            Assert::IsTrue(0 == span.size());  // NOLINT(readability-container-size-empty)
        }

        TEST_METHOD(constructor_2)
        {
            int numbers[] { 1, 2, 3 };
            const Span<int> span(numbers, 3);
            Assert::IsFalse(span.empty());
            Assert::IsTrue(3 == span.size());
        }

        TEST_METHOD(constructor_3)
        {
            const std::vector<int> numbers { 1, 2, 3 };
            const Span<int> span(numbers);
            Assert::IsFalse(span.empty());
            Assert::IsTrue(numbers.size() == span.size());
        }

        TEST_METHOD(constructor_4)
        {
            const std::string text("Hello");
            const Span<char> span(text);
            Assert::IsFalse(span.empty());
            Assert::IsTrue(text.size() == span.size());
        }

        TEST_METHOD(constructor_5)
        {
            const std::wstring text(L"Hello");
            const Span<wchar_t> span(text);
            Assert::IsFalse(span.empty());
            Assert::IsTrue(text.size() == span.size());
        }

        TEST_METHOD(data_1)
        {
            int numbers[] { 1, 2, 3 };
            const Span<int> span(numbers, 3 );
            Assert::IsTrue(span.data()[0] == 1);
        }

        TEST_METHOD(cdata_1)
        {
            int numbers[] { 1, 2, 3 };
            const Span<int> span(numbers, 3 );
            Assert::IsTrue(span.cdata()[0] == 1);
        }

        TEST_METHOD(fromString_1)
        {
            const Span<char> span = Span<char>::fromString("Hello");
            Assert::IsFalse(span.empty());
            Assert::IsTrue(5 == span.size());
            Assert::IsTrue(span.data() != nullptr);
        }

        TEST_METHOD(index_1)
        {
            const std::string text("Hello");
            const Span<char> span(text);
            Assert::IsTrue(span[0] == 'H');
            Assert::IsTrue(span[1] == 'e');
            Assert::IsTrue(span[2] == 'l');
            Assert::IsTrue(span[3] == 'l');
            Assert::IsTrue(span[4] == 'o');
        }

        TEST_METHOD(index_2)
        {
            const std::string text("Hello");
            const Span<char> span(text);
            span[0] = 'w';
            span[1] = 'o';
            span[2] = 'r';
            span[3] = 'l';
            span[4] = 'd';
            Assert::IsTrue(text == "world");
        }

        TEST_METHOD(fill_1)
        {
            int numbers[] { 1, 2, 3 };
            Span<int> span(numbers, 3 );
            span.fill(123);
            Assert::IsTrue(span[0] == 123);
            Assert::IsTrue(span[1] == 123);
            Assert::IsTrue(span[2] == 123);
        }

        TEST_METHOD(begin_1)
        {
            int numbers[] { 1, 2, 3 };
            const Span<int> span(numbers, 3 );
            auto iter = span.begin();
            Assert::IsTrue(*iter == 1);
            iter++;
            Assert::IsTrue(*iter == 2);
            iter++;
            Assert::IsTrue(*iter == 3);
        }

        TEST_METHOD(contains_1)
        {
            int numbers[] { 1, 2, 3 };
            const Span<int> span(numbers, 3);
            Assert::IsFalse(span.contains(0));
            Assert::IsTrue(span.contains(1));
            Assert::IsTrue(span.contains(2));
            Assert::IsTrue(span.contains(3));
            Assert::IsFalse(span.contains(4));
        }

        TEST_METHOD(indexOf_1)
        {
            int numbers[] { 1, 2, 3, 1, 2, 3 };
            const Span<int> span(numbers, 6);
            Assert::IsTrue(span.indexOf(0) < 0);
            Assert::IsTrue(span.indexOf(1) == 0);
            Assert::IsTrue(span.indexOf(2) == 1);
            Assert::IsTrue(span.indexOf(3) == 2);
            Assert::IsTrue(span.indexOf(4) < 0);
        }

        TEST_METHOD(lastIndexOf_1)
        {
            int numbers[] { 1, 2, 3, 1, 2, 3 };
            const Span<int> span(numbers, 6);
            Assert::IsTrue(span.lastIndexOf(0) < 0);
            Assert::IsTrue(span.lastIndexOf(1) == 3);
            Assert::IsTrue(span.lastIndexOf(2) == 4);
            Assert::IsTrue(span.lastIndexOf(3) == 5);
            Assert::IsTrue(span.lastIndexOf(4) < 0);
        }

        TEST_METHOD(toVector_1)
        {
            const std::vector<int> vec1 { 1, 2, 3 };
            const Span<int> span(vec1);
            const std::vector<int> vec2 = span.toVector();
            Assert::IsTrue(vec1 == vec2);
        }

        TEST_METHOD(toString_1)
        {
            char chars[] { 'H', 'e', 'l', 'l', 'o' };
            const Span<char> span(chars, sizeof(chars));
            const std::string text = span.toString();
            Assert::IsTrue(text == "Hello");
        }

        TEST_METHOD(slice_1)
        {
            const std::vector<int> vec1 { 1, 2, 3, 4, 5 };
            const Span<int> span1 (vec1);
            const Span<int> span2 = span1.slice(0, 3);
            Assert::IsTrue(span2.size() == 3);
            Assert::IsTrue(span2[0] == 1);
        }

        TEST_METHOD(slice_2)
        {
            const std::vector<int> vec1 { 1, 2, 3, 4, 5 };
            const Span<int> span1 (vec1);
            const Span<int> span2 = span1.slice(2);
            Assert::IsTrue(span2.size() == 3);
            Assert::IsTrue(span2[0] == 3);
        }

        TEST_METHOD(slice_3)
        {
            const std::vector<int> vec1 { 1, 2, 3, 4, 5 };
            const Span<int> span1 (vec1);
            const Span<int> span2 = span1.slice(2, 0);
            Assert::IsTrue(span2.size() == 0);  // NOLINT(readability-container-size-empty)
            Assert::IsTrue(span2.empty());
        }

        TEST_METHOD(startsWith_1)
        {
            int numbers1[] { 1, 2, 3, 4, 5, 6 };
            int numbers2[] { 1, 2, 3 };
            const Span<int> span(numbers1, 6);
            const Span<int> prefix(numbers2, 3);
            Assert::IsTrue(span.startsWith(prefix));
        }

        TEST_METHOD(startsWith_2)
        {
            int numbers1[] { 1, 2, 3, 4, 5, 6 };
            int numbers2[] { 7, 8, 9 };
            const Span<int> span(numbers1, 6);
            const Span<int> prefix(numbers2, 3);
            Assert::IsFalse(span.startsWith(prefix));
        }

        TEST_METHOD(endsWith_1)
        {
            int numbers1[] { 1, 2, 3, 4, 5, 6 };
            int numbers2[] { 4, 5, 6 };
            const Span<int> span(numbers1, 6);
            const Span<int> suffix(numbers2, 3);
            Assert::IsTrue(span.endsWith(suffix));
        }

        TEST_METHOD(endsWith_2)
        {
            int numbers1[] { 1, 2, 3, 4, 5, 6 };
            int numbers2[] { 7, 8, 9 };
            const Span<int> span(numbers1, 6);
            const Span<int> suffix(numbers2, 3);
            Assert::IsFalse(span.endsWith(suffix));
        }

        TEST_METHOD(compare_1)
        {
            int numbers1[] { 1, 2, 3, 4, 5, 6 };
            int numbers2[] { 1, 2, 3, 4, 5, 6 };
            const Span<int> first(numbers1, 6);
            const Span<int> second(numbers2, 6);
            Assert::IsTrue(first.compare(second) == 0);
        }

        TEST_METHOD(compare_2)
        {
            int numbers1[] { 1, 2, 3, 4, 5, 6 };
            int numbers2[] { 1, 2, 3, 4, 5, 7 };
            const Span<int> first(numbers1, 6);
            const Span<int> second(numbers2, 6);
            Assert::IsTrue(first.compare(second) < 0);
        }

        TEST_METHOD(compare_3)
        {
            int numbers1[] { 1, 2, 3, 4, 5, 6 };
            int numbers2[] { 1, 2, 3, 4, 5, 5 };
            const Span<int> first(numbers1, 6);
            const Span<int> second(numbers2, 6);
            Assert::IsTrue(first.compare(second) > 0);
        }

        TEST_METHOD(compare_4)
        {
            int numbers1[] { 1, 2, 3, 4, 5, 6 };
            int numbers2[] { 1, 2, 3, 4, 5 };
            const Span<int> first(numbers1, 6);
            const Span<int> second(numbers2, 5);
            Assert::IsTrue(first.compare(second) > 0);
        }

        TEST_METHOD(compare_5)
        {
            int numbers1[] { 1, 2, 3, 4, 5, 6 };
            int numbers2[] { 1, 2, 3, 4, 5, 6, 7 };
            const Span<int> first(numbers1, 6);
            const Span<int> second(numbers2, 7);
            Assert::IsTrue(first.compare(second) < 0);
        }

        TEST_METHOD(trimStart_1)
        {
            const CharSpan span = CharSpan::fromString("  Hello!");
            const CharSpan trimmed = trimStart(span);

            Assert::IsTrue(trimmed[0] == 'H');
            Assert::IsTrue(trimmed.length == 6);
        }

        TEST_METHOD(trimStart_2)
        {
            const WideSpan span = WideSpan::fromString(L"  Hello!");
            const WideSpan trimmed = trimStart(span);

            Assert::IsTrue(trimmed[0] == L'H');
            Assert::IsTrue(trimmed.length == 6);
        }

        TEST_METHOD(trimEnd_1)
        {
            const CharSpan span = CharSpan::fromString("Hello!  ");
            const CharSpan trimmed = trimEnd(span);

            Assert::IsTrue(trimmed.length == 6);
        }

        TEST_METHOD(trimEnd_2)
        {
            const WideSpan span = WideSpan::fromString(L"Hello!  ");
            const WideSpan trimmed = trimEnd(span);

            Assert::IsTrue(trimmed.length == 6);
        }

        TEST_METHOD(trim_1)
        {
            const CharSpan span = CharSpan::fromString("  Hello!  ");
            const CharSpan trimmed = trim(span);

            Assert::IsTrue(trimmed[0] == 'H');
            Assert::IsTrue(trimmed.length == 6);
        }

        TEST_METHOD(trim_2)
        {
            const WideSpan span = WideSpan::fromString(L"  Hello!  ");
            const WideSpan trimmed = trim(span);

            Assert::IsTrue(trimmed[0] == L'H');
            Assert::IsTrue(trimmed.length == 6);
        }

        TEST_METHOD(toupper_1)
        {
            char chars[] { 'H', 'e', 'l', 'l', 'o' };
            const CharSpan span(chars, sizeof(chars)/sizeof(chars[0]));
            toupper(span);
            Assert::IsTrue(chars[0] == 'H');
            Assert::IsTrue(chars[1] == 'E');
            Assert::IsTrue(chars[2] == 'L');
            Assert::IsTrue(chars[3] == 'L');
            Assert::IsTrue(chars[4] == 'O');
        }

        TEST_METHOD(toupper_2)
        {
            Char chars[] { L'H', L'e', L'l', L'l', L'o' };
            const WideSpan span(chars, sizeof(chars)/sizeof(chars[0]));
            toupper(span);
            Assert::IsTrue(chars[0] == L'H');
            Assert::IsTrue(chars[1] == L'E');
            Assert::IsTrue(chars[2] == L'L');
            Assert::IsTrue(chars[3] == L'L');
            Assert::IsTrue(chars[4] == L'O');
        }

        TEST_METHOD(tolower_1)
        {
            char chars[] { 'H', 'e', 'l', 'l', 'o' };
            const CharSpan span(chars, sizeof(chars)/sizeof(chars[0]));
            tolower(span);
            Assert::IsTrue(chars[0] == 'h');
            Assert::IsTrue(chars[1] == 'e');
            Assert::IsTrue(chars[2] == 'l');
            Assert::IsTrue(chars[3] == 'l');
            Assert::IsTrue(chars[4] == 'o');
        }

        TEST_METHOD(tolower_2)
        {
            Char chars[] { L'H', L'e', L'l', L'l', L'o' };
            const WideSpan span(chars, sizeof(chars)/sizeof(chars[0]));
            tolower(span);
            Assert::IsTrue(chars[0] == L'h');
            Assert::IsTrue(chars[1] == L'e');
            Assert::IsTrue(chars[2] == L'l');
            Assert::IsTrue(chars[3] == L'l');
            Assert::IsTrue(chars[4] == L'o');
        }

        TEST_METHOD(split_1)
        {
            const CharSpan span = CharSpan::fromString("Hello,world,here");
            std::vector<CharSpan> parts = span.split(',');
            Assert::IsTrue(parts.size() == 3);
            Assert::IsTrue(parts[0].toString() == "Hello");
            Assert::IsTrue(parts[1].toString() == "world");
            Assert::IsTrue(parts[2].toString() == "here");
        }

        TEST_METHOD(split_2)
        {
            const WideSpan span = WideSpan::fromString(L"Hello,world,here");
            std::vector<WideSpan> parts = span.split(L',');
            Assert::IsTrue(parts.size() == 3);
            Assert::IsTrue(parts[0].toString() == L"Hello");
            Assert::IsTrue(parts[1].toString() == L"world");
            Assert::IsTrue(parts[2].toString() == L"here");
        }

        TEST_METHOD(split_3)
        {
            const CharSpan span = CharSpan::fromString("Hello,world,here");
            std::vector<CharSpan> parts = span.split(',', 2);
            Assert::IsTrue(parts.size() == 2);
            Assert::IsTrue(parts[0].toString() == "Hello");
            Assert::IsTrue(parts[1].toString() == "world,here");
        }

        TEST_METHOD(parseInt32_1)
        {
            Assert::IsTrue(CharSpan::fromString("").parseInt32() == 0);
            Assert::IsTrue(CharSpan::fromString("0").parseInt32() == 0);
            Assert::IsTrue(CharSpan::fromString("1").parseInt32() == 1);
            Assert::IsTrue(CharSpan::fromString("12").parseInt32() == 12);
            Assert::IsTrue(CharSpan::fromString("123").parseInt32() == 123);
            Assert::IsTrue(CharSpan::fromString("1234").parseInt32() == 1234);
            Assert::IsTrue(CharSpan::fromString("12345").parseInt32() == 12345);
            Assert::IsTrue(CharSpan::fromString("123456").parseInt32() == 123456);
        }

        TEST_METHOD(parseInt32_2)
        {
            Assert::IsTrue(WideSpan::fromString(L"").parseInt32() == 0);
            Assert::IsTrue(WideSpan::fromString(L"0").parseInt32() == 0);
            Assert::IsTrue(WideSpan::fromString(L"1").parseInt32() == 1);
            Assert::IsTrue(WideSpan::fromString(L"12").parseInt32() == 12);
            Assert::IsTrue(WideSpan::fromString(L"123").parseInt32() == 123);
            Assert::IsTrue(WideSpan::fromString(L"1234").parseInt32() == 1234);
            Assert::IsTrue(WideSpan::fromString(L"12345").parseInt32() == 12345);
            Assert::IsTrue(WideSpan::fromString(L"123456").parseInt32() == 123456);
        }

        TEST_METHOD(parseInt64_1)
        {
            Assert::IsTrue(CharSpan::fromString("").parseInt64() == 0);
            Assert::IsTrue(CharSpan::fromString("0").parseInt64() == 0);
            Assert::IsTrue(CharSpan::fromString("1").parseInt64() == 1);
            Assert::IsTrue(CharSpan::fromString("12").parseInt64() == 12);
            Assert::IsTrue(CharSpan::fromString("123").parseInt64() == 123);
            Assert::IsTrue(CharSpan::fromString("1234").parseInt64() == 1234);
            Assert::IsTrue(CharSpan::fromString("12345").parseInt64() == 12345);
            Assert::IsTrue(CharSpan::fromString("123456").parseInt64() == 123456);
        }

        TEST_METHOD(parseInt64_2)
        {
            Assert::IsTrue(WideSpan::fromString(L"").parseInt64() == 0);
            Assert::IsTrue(WideSpan::fromString(L"0").parseInt64() == 0);
            Assert::IsTrue(WideSpan::fromString(L"1").parseInt64() == 1);
            Assert::IsTrue(WideSpan::fromString(L"12").parseInt64() == 12);
            Assert::IsTrue(WideSpan::fromString(L"123").parseInt64() == 123);
            Assert::IsTrue(WideSpan::fromString(L"1234").parseInt64() == 1234);
            Assert::IsTrue(WideSpan::fromString(L"12345").parseInt64() == 12345);
            Assert::IsTrue(WideSpan::fromString(L"123456").parseInt64() == 123456);
        }
    };
}

