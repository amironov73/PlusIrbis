#include "pch.h"

TEST(SpanTest, constructor_1)
{
    const irbis::Span<int> span;
    EXPECT_TRUE(span.empty());
    EXPECT_TRUE(0 == span.size());  // NOLINT(readability-container-size-empty)
}

TEST(SpanTest, constructor_2)
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3);
    EXPECT_FALSE(span.empty());
    EXPECT_TRUE(3 == span.size());
}

TEST(SpanTest, constructor_3)
{
    const std::vector<int> numbers { 1, 2, 3 };
    const irbis::Span<int> span(numbers);
    EXPECT_FALSE(span.empty());
    EXPECT_TRUE(numbers.size() == span.size());
}

TEST(SpanTest, constructor_4)
{
    const std::string text("Hello");
    const irbis::Span<char> span(text);
    EXPECT_FALSE(span.empty());
    EXPECT_TRUE(text.size() == span.size());
}

TEST(SpanTest, constructor_5)
{
    const std::wstring text(L"Hello");
    const irbis::Span<wchar_t> span(text);
    EXPECT_FALSE(span.empty());
    EXPECT_TRUE(text.size() == span.size());
}

TEST(SpanTest, data_1)
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3 );
    EXPECT_TRUE(span.data()[0] == 1);
}

TEST(SpanTest, data_2)
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span;
    EXPECT_TRUE(span.data() == nullptr);
}

TEST(SpanTest, cdata_1)
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3 );
    EXPECT_TRUE(span.cdata()[0] == 1);
}

TEST(SpanTest, ñdata_2)
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span;
    EXPECT_TRUE(span.cdata() == nullptr);
}

TEST(SpanTest, fromString_1)
{
    const irbis::Span<char> span = irbis::Span<char>::fromString("Hello");
    EXPECT_FALSE(span.empty());
    EXPECT_TRUE(5 == span.size());
    EXPECT_TRUE(span.data() != nullptr);
}

TEST(SpanTest, index_1)
{
    const std::string text("Hello");
    const irbis::Span<char> span(text);
    EXPECT_TRUE(span[0] == 'H');
    EXPECT_TRUE(span[1] == 'e');
    EXPECT_TRUE(span[2] == 'l');
    EXPECT_TRUE(span[3] == 'l');
    EXPECT_TRUE(span[4] == 'o');
}

TEST(SpanTest, index_2)
{
    const std::string text("Hello");
    const irbis::Span<char> span(text);
    span[0] = 'w';
    span[1] = 'o';
    span[2] = 'r';
    span[3] = 'l';
    span[4] = 'd';
    EXPECT_TRUE(text == "world");
}

TEST(SpanTest, fill_1)
{
    int numbers[] { 1, 2, 3 };
    irbis::Span<int> span(numbers, 3 );
    span.fill(123);
    EXPECT_TRUE(span[0] == 123);
    EXPECT_TRUE(span[1] == 123);
    EXPECT_TRUE(span[2] == 123);
}

TEST(SpanTest, begin_1)
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3 );
    auto iter = span.begin();
    EXPECT_TRUE(*iter == 1);
    iter++;
    EXPECT_TRUE(*iter == 2);
    iter++;
    EXPECT_TRUE(*iter == 3);
}

TEST(SpanTest, cbegin_1)
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3 );
    auto iter = span.cbegin();
    EXPECT_TRUE(*iter == 1);
    iter++;
    EXPECT_TRUE(*iter == 2);
    iter++;
    EXPECT_TRUE(*iter == 3);
}

TEST(SpanTest, contains_1)
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3);
    EXPECT_FALSE(span.contains(0));
    EXPECT_TRUE(span.contains(1));
    EXPECT_TRUE(span.contains(2));
    EXPECT_TRUE(span.contains(3));
    EXPECT_FALSE(span.contains(4));
}

TEST(SpanTest, contains_2)
{
    wchar_t characters[] { L'A', L'B', L'C' };
    const irbis::Span<wchar_t> span(characters, 3);
    EXPECT_FALSE(span.contains(0));
    EXPECT_TRUE(span.contains(L'A'));
    EXPECT_TRUE(span.contains('A'));
    EXPECT_FALSE(span.contains(L'a'));
    EXPECT_FALSE(span.contains(L'D'));
    EXPECT_FALSE(span.contains('D'));
    EXPECT_FALSE(span.contains(L'd'));
}

TEST(SpanTest, indexOf_1)
{
    int numbers[] { 1, 2, 3, 1, 2, 3 };
    const irbis::Span<int> span(numbers, 6);
    EXPECT_TRUE(span.indexOf(0) < 0);
    EXPECT_TRUE(span.indexOf(1) == 0);
    EXPECT_TRUE(span.indexOf(2) == 1);
    EXPECT_TRUE(span.indexOf(3) == 2);
    EXPECT_TRUE(span.indexOf(4) < 0);
}

TEST(SpanTest, indexOf_2)
{
    wchar_t characters[] { L'A', L'B', L'C', L'A', L'B', L'C' };
    const irbis::Span<wchar_t> span(characters, 6);
    EXPECT_TRUE(span.indexOf(0) < 0);
    EXPECT_TRUE(span.indexOf(L'A') == 0);
    EXPECT_TRUE(span.indexOf(L'B') == 1);
    EXPECT_TRUE(span.indexOf(L'C') == 2);
    EXPECT_TRUE(span.indexOf('A') == 0);
    EXPECT_TRUE(span.indexOf('B') == 1);
    EXPECT_TRUE(span.indexOf('C') == 2);
    EXPECT_TRUE(span.indexOf(L'D') < 0);
    EXPECT_TRUE(span.indexOf(L'a') < 0);
    EXPECT_TRUE(span.indexOf(L'b') < 0);
    EXPECT_TRUE(span.indexOf(L'c') < 0);
}

TEST(SpanTest, lastIndexOf_1)
{
    int numbers[] { 1, 2, 3, 1, 2, 3 };
    const irbis::Span<int> span(numbers, 6);
    EXPECT_TRUE(span.lastIndexOf(0) < 0);
    EXPECT_TRUE(span.lastIndexOf(1) == 3);
    EXPECT_TRUE(span.lastIndexOf(2) == 4);
    EXPECT_TRUE(span.lastIndexOf(3) == 5);
    EXPECT_TRUE(span.lastIndexOf(4) < 0);
}

TEST(SpanTest, toVector_1)
{
    const std::vector<int> vec1 { 1, 2, 3 };
    const irbis::Span<int> span(vec1);
    const std::vector<int> vec2 = span.toVector();
    EXPECT_TRUE(vec1 == vec2);
}

TEST(SpanTest, toString_1)
{
    char chars[] { 'H', 'e', 'l', 'l', 'o' };
    const irbis::Span<char> span(chars, sizeof(chars));
    const std::string text = span.toString();
    EXPECT_TRUE(text == "Hello");
}

TEST(SpanTest, slice_1)
{
    const std::vector<int> vec1 { 1, 2, 3, 4, 5 };
    const irbis::Span<int> span1 (vec1);
    const irbis::Span<int> span2 = span1.slice(0, 3);
    EXPECT_TRUE(span2.size() == 3);
    EXPECT_TRUE(span2[0] == 1);
}

TEST(SpanTest, slice_2)
{
    const std::vector<int> vec1 { 1, 2, 3, 4, 5 };
    const irbis::Span<int> span1 (vec1);
    const irbis::Span<int> span2 = span1.slice(2);
    EXPECT_TRUE(span2.size() == 3);
    EXPECT_TRUE(span2[0] == 3);
}

TEST(SpanTest, slice_3)
{
    const std::vector<int> vec1 { 1, 2, 3, 4, 5 };
    const irbis::Span<int> span1 (vec1);
    const irbis::Span<int> span2 = span1.slice(2, 0);
    EXPECT_TRUE(span2.size() == 0);  // NOLINT(readability-container-size-empty)
    EXPECT_TRUE(span2.empty());
}

TEST(SpanTest, startsWith_1)
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers1, 6);
    const irbis::Span<int> prefix(numbers2, 3);
    EXPECT_TRUE(span.startsWith(prefix));
}

TEST(SpanTest, startsWith_2)
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 7, 8, 9 };
    const irbis::Span<int> span(numbers1, 6);
    const irbis::Span<int> prefix(numbers2, 3);
    EXPECT_FALSE(span.startsWith(prefix));
}

TEST(SpanTest, endsWith_1)
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 4, 5, 6 };
    const irbis::Span<int> span(numbers1, 6);
    const irbis::Span<int> suffix(numbers2, 3);
    EXPECT_TRUE(span.endsWith(suffix));
}

TEST(SpanTest, endsWith_2)
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 7, 8, 9 };
    const irbis::Span<int> span(numbers1, 6);
    const irbis::Span<int> suffix(numbers2, 3);
    EXPECT_FALSE(span.endsWith(suffix));
}

TEST(SpanTest, compare_1)
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5, 6 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 6);
    EXPECT_TRUE(first.compare(second) == 0);
}

TEST(SpanTest, compare_2)
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5, 7 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 6);
    EXPECT_TRUE(first.compare(second) < 0);
}

TEST(SpanTest, compare_3)
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5, 5 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 6);
    EXPECT_TRUE(first.compare(second) > 0);
}

TEST(SpanTest, compare_4)
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 5);
    EXPECT_TRUE(first.compare(second) > 0);
}

TEST(SpanTest, compare_5)
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5, 6, 7 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 7);
    EXPECT_TRUE(first.compare(second) < 0);
}

TEST(SpanTest, trimStart_1)
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("  Hello!");
    const irbis::CharSpan trimmed = irbis::trimStart(span);

    EXPECT_TRUE(trimmed[0] == 'H');
    EXPECT_TRUE(trimmed.length == 6);
}

TEST(SpanTest, trimStart_2)
{
    const irbis::WideSpan span = irbis::WideSpan::fromString(L"  Hello!");
    const irbis::WideSpan trimmed = irbis::trimStart(span);

    EXPECT_TRUE(trimmed[0] == L'H');
    EXPECT_TRUE(trimmed.length == 6);
}

TEST(SpanTest, trimEnd_1)
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("Hello!  ");
    const irbis::CharSpan trimmed = irbis::trimEnd(span);

    EXPECT_TRUE(trimmed.length == 6);
}

TEST(SpanTest, trimEnd_2)
{
    const irbis::WideSpan span = irbis::WideSpan::fromString(L"Hello!  ");
    const irbis::WideSpan trimmed = irbis::trimEnd(span);

    EXPECT_TRUE(trimmed.length == 6);
}

TEST(SpanTest, trim_1)
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("  Hello!  ");
    const irbis::CharSpan trimmed = trim(span);

    EXPECT_TRUE(trimmed[0] == 'H');
    EXPECT_TRUE(trimmed.length == 6);
}

TEST(SpanTest, trim_2)
{
    const irbis::WideSpan span = irbis::WideSpan::fromString(L"  Hello!  ");
    const irbis::WideSpan trimmed = irbis::trim(span);

    EXPECT_TRUE(trimmed[0] == L'H');
    EXPECT_TRUE(trimmed.length == 6);
}

TEST(SpanTest, toupper_1)
{
    char chars[] { 'H', 'e', 'l', 'l', 'o' };
    const irbis::CharSpan span(chars, sizeof(chars)/sizeof(chars[0]));
    irbis::toupper(span);
    EXPECT_TRUE(chars[0] == 'H');
    EXPECT_TRUE(chars[1] == 'E');
    EXPECT_TRUE(chars[2] == 'L');
    EXPECT_TRUE(chars[3] == 'L');
    EXPECT_TRUE(chars[4] == 'O');
}

TEST(SpanTest, toupper_2)
{
    irbis::Char chars[] { L'H', L'e', L'l', L'l', L'o' };
    const irbis::WideSpan span(chars, sizeof(chars)/sizeof(chars[0]));
    irbis::toupper(span);
    EXPECT_TRUE(chars[0] == L'H');
    EXPECT_TRUE(chars[1] == L'E');
    EXPECT_TRUE(chars[2] == L'L');
    EXPECT_TRUE(chars[3] == L'L');
    EXPECT_TRUE(chars[4] == L'O');
}

TEST(SpanTest, tolower_1)
{
    char chars[] { 'H', 'e', 'l', 'l', 'o' };
    const irbis::CharSpan span(chars, sizeof(chars)/sizeof(chars[0]));
    irbis::tolower(span);
    EXPECT_TRUE(chars[0] == 'h');
    EXPECT_TRUE(chars[1] == 'e');
    EXPECT_TRUE(chars[2] == 'l');
    EXPECT_TRUE(chars[3] == 'l');
    EXPECT_TRUE(chars[4] == 'o');
}

TEST(SpanTest, tolower_2)
{
    irbis::Char chars[] { L'H', L'e', L'l', L'l', L'o' };
    const irbis::WideSpan span(chars, sizeof(chars)/sizeof(chars[0]));
    irbis::tolower(span);
    EXPECT_TRUE(chars[0] == L'h');
    EXPECT_TRUE(chars[1] == L'e');
    EXPECT_TRUE(chars[2] == L'l');
    EXPECT_TRUE(chars[3] == L'l');
    EXPECT_TRUE(chars[4] == L'o');
}

TEST(SpanTest, split_1)
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("Hello,world,here");
    std::vector<irbis::CharSpan> parts = span.split(',');
    EXPECT_TRUE(parts.size() == 3);
    EXPECT_TRUE(parts[0].toString() == "Hello");
    EXPECT_TRUE(parts[1].toString() == "world");
    EXPECT_TRUE(parts[2].toString() == "here");
}

TEST(SpanTest, split_2)
{
    const irbis::WideSpan span = irbis::WideSpan::fromString(L"Hello,world,here");
    std::vector<irbis::WideSpan> parts = span.split(L',');
    EXPECT_TRUE(parts.size() == 3);
    EXPECT_TRUE(parts[0].toString() == L"Hello");
    EXPECT_TRUE(parts[1].toString() == L"world");
    EXPECT_TRUE(parts[2].toString() == L"here");
}

TEST(SpanTest, split_3)
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("Hello,world,here");
    std::vector<irbis::CharSpan> parts = span.split(',', 2);
    EXPECT_TRUE(parts.size() == 2);
    EXPECT_TRUE(parts[0].toString() == "Hello");
    EXPECT_TRUE(parts[1].toString() == "world,here");
}

TEST(SpanTest, parseInt32_1)
{
    EXPECT_TRUE(irbis::CharSpan::fromString("").parseInt32() == 0);
    EXPECT_TRUE(irbis::CharSpan::fromString("0").parseInt32() == 0);
    EXPECT_TRUE(irbis::CharSpan::fromString("1").parseInt32() == 1);
    EXPECT_TRUE(irbis::CharSpan::fromString("12").parseInt32() == 12);
    EXPECT_TRUE(irbis::CharSpan::fromString("123").parseInt32() == 123);
    EXPECT_TRUE(irbis::CharSpan::fromString("1234").parseInt32() == 1234);
    EXPECT_TRUE(irbis::CharSpan::fromString("12345").parseInt32() == 12345);
    EXPECT_TRUE(irbis::CharSpan::fromString("123456").parseInt32() == 123456);
}

TEST(SpanTest, parseInt32_2)
{
    EXPECT_TRUE(irbis::WideSpan::fromString(L"").parseInt32() == 0);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"0").parseInt32() == 0);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"1").parseInt32() == 1);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"12").parseInt32() == 12);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"123").parseInt32() == 123);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"1234").parseInt32() == 1234);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"12345").parseInt32() == 12345);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"123456").parseInt32() == 123456);
}

TEST(SpanTest, parseInt64_1)
{
    EXPECT_TRUE(irbis::CharSpan::fromString("").parseInt64() == 0);
    EXPECT_TRUE(irbis::CharSpan::fromString("0").parseInt64() == 0);
    EXPECT_TRUE(irbis::CharSpan::fromString("1").parseInt64() == 1);
    EXPECT_TRUE(irbis::CharSpan::fromString("12").parseInt64() == 12);
    EXPECT_TRUE(irbis::CharSpan::fromString("123").parseInt64() == 123);
    EXPECT_TRUE(irbis::CharSpan::fromString("1234").parseInt64() == 1234);
    EXPECT_TRUE(irbis::CharSpan::fromString("12345").parseInt64() == 12345);
    EXPECT_TRUE(irbis::CharSpan::fromString("123456").parseInt64() == 123456);
}

TEST(SpanTest, parseInt64_2)
{
    EXPECT_TRUE(irbis::WideSpan::fromString(L"").parseInt64() == 0);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"0").parseInt64() == 0);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"1").parseInt64() == 1);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"12").parseInt64() == 12);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"123").parseInt64() == 123);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"1234").parseInt64() == 1234);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"12345").parseInt64() == 12345);
    EXPECT_TRUE(irbis::WideSpan::fromString(L"123456").parseInt64() == 123456);
}

