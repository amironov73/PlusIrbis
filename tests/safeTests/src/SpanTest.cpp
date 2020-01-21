#include "catch.hpp"
#include "irbis.h"

TEST_CASE("Span_constructor_1", "[span]")
{
    const irbis::Span<int> span;
    CHECK(span.empty());
    CHECK(0 == span.size());  // NOLINT(readability-container-size-empty)
}

TEST_CASE("Span_constructor_2", "[span]")
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3);
    CHECK_FALSE(span.empty());
    CHECK(3 == span.size());
}

TEST_CASE("Span_constructor_3", "[span]")
{
    const std::vector<int> numbers { 1, 2, 3 };
    const irbis::Span<int> span(numbers);
    CHECK_FALSE(span.empty());
    CHECK(numbers.size() == span.size());
}

TEST_CASE("Span_constructor_4", "[span]")
{
    const std::string text("Hello");
    const irbis::Span<char> span(text);
    CHECK_FALSE(span.empty());
    CHECK(text.size() == span.size());
}

TEST_CASE("Span_constructor_5", "[span]")
{
    const std::wstring text(L"Hello");
    const irbis::Span<wchar_t> span(text);
    CHECK_FALSE(span.empty());
    CHECK(text.size() == span.size());
}

TEST_CASE("Span_data_1", "[span]")
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3 );
    CHECK(span.data()[0] == 1);
}

TEST_CASE("Span_data_2", "[span]")
{
    const irbis::Span<int> span;
    CHECK(span.data() == nullptr);
}

TEST_CASE("Span_cdata_1", "[span]")
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3 );
    CHECK(span.cdata()[0] == 1);
}

TEST_CASE("Span_cdata_2", "[span]")
{
    const irbis::Span<int> span;
    CHECK(span.cdata() == nullptr);
}

TEST_CASE("Span_fromString_1", "[span]")
{
    const irbis::Span<char> span = irbis::Span<char>::fromString("Hello");
    CHECK_FALSE(span.empty());
    CHECK(5 == span.size());
    CHECK(span.data() != nullptr);
}

TEST_CASE("Span_index_1", "[span]")
{
    const std::string text("Hello");
    const irbis::Span<char> span(text);
    CHECK(span[0] == 'H');
    CHECK(span[1] == 'e');
    CHECK(span[2] == 'l');
    CHECK(span[3] == 'l');
    CHECK(span[4] == 'o');
}

TEST_CASE("Span_index_2", "[span]")
{
    const std::string text("Hello");
    const irbis::Span<char> span(text);
    span[0] = 'w';
    span[1] = 'o';
    span[2] = 'r';
    span[3] = 'l';
    span[4] = 'd';
    CHECK(text == "world");
}

TEST_CASE("Span_fill_1", "[span]")
{
    int numbers[] { 1, 2, 3 };
    irbis::Span<int> span(numbers, 3 );
    span.fill(123);
    CHECK(span[0] == 123);
    CHECK(span[1] == 123);
    CHECK(span[2] == 123);
}

TEST_CASE("Span_begin_1", "[span]")
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3 );
    auto iter = span.begin();
    CHECK(*iter == 1);
    iter++;
    CHECK(*iter == 2);
    iter++;
    CHECK(*iter == 3);
}

TEST_CASE("Span_cbegin_1", "[span]")
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3 );
    auto iter = span.cbegin();
    CHECK(*iter == 1);
    iter++;
    CHECK(*iter == 2);
    iter++;
    CHECK(*iter == 3);
}

TEST_CASE("Span_contains_1", "[span]")
{
    int numbers[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers, 3);
    CHECK_FALSE(span.contains(0));
    CHECK(span.contains(1));
    CHECK(span.contains(2));
    CHECK(span.contains(3));
    CHECK_FALSE(span.contains(4));
}

TEST_CASE("Span_contains_2", "[span]")
{
    wchar_t characters[] { L'A', L'B', L'C' };
    const irbis::Span<wchar_t> span(characters, 3);
    CHECK_FALSE(span.contains(0));
    CHECK(span.contains(L'A'));
    CHECK(span.contains('A'));
    CHECK_FALSE(span.contains(L'a'));
    CHECK_FALSE(span.contains(L'D'));
    CHECK_FALSE(span.contains('D'));
    CHECK_FALSE(span.contains(L'd'));
}

TEST_CASE("Span_indexOf_1", "[span]")
{
    int numbers[] { 1, 2, 3, 1, 2, 3 };
    const irbis::Span<int> span(numbers, 6);
    CHECK(span.indexOf(0) < 0);
    CHECK(span.indexOf(1) == 0);
    CHECK(span.indexOf(2) == 1);
    CHECK(span.indexOf(3) == 2);
    CHECK(span.indexOf(4) < 0);
}

TEST_CASE("Span_indexOf_2", "[span]")
{
    wchar_t characters[] { L'A', L'B', L'C', L'A', L'B', L'C' };
    const irbis::Span<wchar_t> span(characters, 6);
    CHECK(span.indexOf(0) < 0);
    CHECK(span.indexOf(L'A') == 0);
    CHECK(span.indexOf(L'B') == 1);
    CHECK(span.indexOf(L'C') == 2);
    CHECK(span.indexOf('A') == 0);
    CHECK(span.indexOf('B') == 1);
    CHECK(span.indexOf('C') == 2);
    CHECK(span.indexOf(L'D') < 0);
    CHECK(span.indexOf(L'a') < 0);
    CHECK(span.indexOf(L'b') < 0);
    CHECK(span.indexOf(L'c') < 0);
}

TEST_CASE("Span_lastIndexOf_1", "[span]")
{
    int numbers[] { 1, 2, 3, 1, 2, 3 };
    const irbis::Span<int> span(numbers, 6);
    CHECK(span.lastIndexOf(0) < 0);
    CHECK(span.lastIndexOf(1) == 3);
    CHECK(span.lastIndexOf(2) == 4);
    CHECK(span.lastIndexOf(3) == 5);
    CHECK(span.lastIndexOf(4) < 0);
}

TEST_CASE("Span_toVector_1", "[span]")
{
    const std::vector<int> vec1 { 1, 2, 3 };
    const irbis::Span<int> span(vec1);
    const std::vector<int> vec2 = span.toVector();
    CHECK(vec1 == vec2);
}

TEST_CASE("Span_toString_1", "[span]")
{
    char chars[] { 'H', 'e', 'l', 'l', 'o' };
    const irbis::Span<char> span(chars, sizeof(chars));
    const std::string text = span.toString();
    CHECK(text == "Hello");
}

TEST_CASE("Span_slice_1", "[span]")
{
    const std::vector<int> vec1 { 1, 2, 3, 4, 5 };
    const irbis::Span<int> span1 (vec1);
    const irbis::Span<int> span2 = span1.slice(0, 3);
    CHECK(span2.size() == 3);
    CHECK(span2[0] == 1);
}

TEST_CASE("Span_slice_2", "[span]")
{
    const std::vector<int> vec1 { 1, 2, 3, 4, 5 };
    const irbis::Span<int> span1 (vec1);
    const irbis::Span<int> span2 = span1.slice(2);
    CHECK(span2.size() == 3);
    CHECK(span2[0] == 3);
}

TEST_CASE("Span_slice_3", "[span]")
{
    const std::vector<int> vec1 { 1, 2, 3, 4, 5 };
    const irbis::Span<int> span1 (vec1);
    const irbis::Span<int> span2 = span1.slice(2, 0);
    CHECK(span2.size() == 0);  // NOLINT(readability-container-size-empty)
    CHECK(span2.empty());
}

TEST_CASE("Span_startsWith_1", "[span]")
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3 };
    const irbis::Span<int> span(numbers1, 6);
    const irbis::Span<int> prefix(numbers2, 3);
    CHECK(span.startsWith(prefix));
}

TEST_CASE("Span_startsWith_2", "[span]")
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 7, 8, 9 };
    const irbis::Span<int> span(numbers1, 6);
    const irbis::Span<int> prefix(numbers2, 3);
    CHECK_FALSE(span.startsWith(prefix));
}

TEST_CASE("Span_endsWith_1", "[span]")
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 4, 5, 6 };
    const irbis::Span<int> span(numbers1, 6);
    const irbis::Span<int> suffix(numbers2, 3);
    CHECK(span.endsWith(suffix));
}

TEST_CASE("Span_endsWith_2", "[span]")
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 7, 8, 9 };
    const irbis::Span<int> span(numbers1, 6);
    const irbis::Span<int> suffix(numbers2, 3);
    CHECK_FALSE(span.endsWith(suffix));
}

TEST_CASE("Span_compare_1", "[span]")
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5, 6 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 6);
    CHECK(first.compare(second) == 0);
}

TEST_CASE("Span_compare_2", "[span]")
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5, 7 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 6);
    CHECK(first.compare(second) < 0);
}

TEST_CASE("Span_compare_3", "[span]")
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5, 5 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 6);
    CHECK(first.compare(second) > 0);
}

TEST_CASE("Span_compare_4", "[span]")
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 5);
    CHECK(first.compare(second) > 0);
}

TEST_CASE("Span_compare_5", "[span]")
{
    int numbers1[] { 1, 2, 3, 4, 5, 6 };
    int numbers2[] { 1, 2, 3, 4, 5, 6, 7 };
    const irbis::Span<int> first(numbers1, 6);
    const irbis::Span<int> second(numbers2, 7);
    CHECK(first.compare(second) < 0);
}

TEST_CASE("Span_trimStart_1", "[span]")
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("  Hello!");
    const irbis::CharSpan trimmed = irbis::trimStart(span);
    CHECK(trimmed[0] == 'H');
    CHECK(trimmed.length == 6);
}

TEST_CASE("Span_trimStart_2", "[span]")
{
    const irbis::WideSpan span = irbis::WideSpan::fromString(L"  Hello!");
    const irbis::WideSpan trimmed = irbis::trimStart(span);
    CHECK(trimmed[0] == L'H');
    CHECK(trimmed.length == 6);
}

TEST_CASE("Span_trimEnd_1", "[span]")
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("Hello!  ");
    const irbis::CharSpan trimmed = irbis::trimEnd(span);
    CHECK(trimmed.length == 6);
}

TEST_CASE("Span_trimEnd_2", "[span]")
{
    const irbis::WideSpan span = irbis::WideSpan::fromString(L"Hello!  ");
    const irbis::WideSpan trimmed = irbis::trimEnd(span);
    CHECK(trimmed.length == 6);
}

TEST_CASE("Span_trim_1", "[span]")
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("  Hello!  ");
    const irbis::CharSpan trimmed = trim(span);
    CHECK(trimmed[0] == 'H');
    CHECK(trimmed.length == 6);
}

TEST_CASE("Span_trim_2", "[span]")
{
    const irbis::WideSpan span = irbis::WideSpan::fromString(L"  Hello!  ");
    const irbis::WideSpan trimmed = irbis::trim(span);
    CHECK(trimmed[0] == L'H');
    CHECK(trimmed.length == 6);
}

TEST_CASE("Span_toupper_1", "[span]")
{
    char chars[] { 'H', 'e', 'l', 'l', 'o' };
    const irbis::CharSpan span(chars, sizeof(chars)/sizeof(chars[0]));
    irbis::toupper(span);
    CHECK(chars[0] == 'H');
    CHECK(chars[1] == 'E');
    CHECK(chars[2] == 'L');
    CHECK(chars[3] == 'L');
    CHECK(chars[4] == 'O');
}

TEST_CASE("Span_toupper_2", "[span]")
{
    irbis::Char chars[] { L'H', L'e', L'l', L'l', L'o' };
    const irbis::WideSpan span(chars, sizeof(chars)/sizeof(chars[0]));
    irbis::toupper(span);
    CHECK(chars[0] == L'H');
    CHECK(chars[1] == L'E');
    CHECK(chars[2] == L'L');
    CHECK(chars[3] == L'L');
    CHECK(chars[4] == L'O');
}

TEST_CASE("Span_tolower_1", "[span]")
{
    char chars[] { 'H', 'e', 'l', 'l', 'o' };
    const irbis::CharSpan span(chars, sizeof(chars)/sizeof(chars[0]));
    irbis::tolower(span);
    CHECK(chars[0] == 'h');
    CHECK(chars[1] == 'e');
    CHECK(chars[2] == 'l');
    CHECK(chars[3] == 'l');
    CHECK(chars[4] == 'o');
}

TEST_CASE("Span_tolower_2", "[span]")
{
    irbis::Char chars[] { L'H', L'e', L'l', L'l', L'o' };
    const irbis::WideSpan span(chars, sizeof(chars)/sizeof(chars[0]));
    irbis::tolower(span);
    CHECK(chars[0] == L'h');
    CHECK(chars[1] == L'e');
    CHECK(chars[2] == L'l');
    CHECK(chars[3] == L'l');
    CHECK(chars[4] == L'o');
}

TEST_CASE("Span_split_1", "[span]")
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("Hello,world,here");
    std::vector<irbis::CharSpan> parts = span.split(',');
    CHECK(parts.size() == 3);
    CHECK(parts[0].toString() == "Hello");
    CHECK(parts[1].toString() == "world");
    CHECK(parts[2].toString() == "here");
}

TEST_CASE("Span_split_2", "[span]")
{
    const irbis::WideSpan span = irbis::WideSpan::fromString(L"Hello,world,here");
    std::vector<irbis::WideSpan> parts = span.split(L',');
    CHECK(parts.size() == 3);
    CHECK(parts[0].toString() == L"Hello");
    CHECK(parts[1].toString() == L"world");
    CHECK(parts[2].toString() == L"here");
}

TEST_CASE("Span_split_3", "[span]")
{
    const irbis::CharSpan span = irbis::CharSpan::fromString("Hello,world,here");
    std::vector<irbis::CharSpan> parts = span.split(',', 2);
    CHECK(parts.size() == 2);
    CHECK(parts[0].toString() == "Hello");
    CHECK(parts[1].toString() == "world,here");
}

TEST_CASE("Span_parseInt32_1", "[span]")
{
    CHECK(irbis::CharSpan::fromString("").parseInt32() == 0);
    CHECK(irbis::CharSpan::fromString("0").parseInt32() == 0);
    CHECK(irbis::CharSpan::fromString("1").parseInt32() == 1);
    CHECK(irbis::CharSpan::fromString("12").parseInt32() == 12);
    CHECK(irbis::CharSpan::fromString("123").parseInt32() == 123);
    CHECK(irbis::CharSpan::fromString("1234").parseInt32() == 1234);
    CHECK(irbis::CharSpan::fromString("12345").parseInt32() == 12345);
    CHECK(irbis::CharSpan::fromString("123456").parseInt32() == 123456);
}

TEST_CASE("Span_parseInt32_2", "[span]")
{
    CHECK(irbis::WideSpan::fromString(L"").parseInt32() == 0);
    CHECK(irbis::WideSpan::fromString(L"0").parseInt32() == 0);
    CHECK(irbis::WideSpan::fromString(L"1").parseInt32() == 1);
    CHECK(irbis::WideSpan::fromString(L"12").parseInt32() == 12);
    CHECK(irbis::WideSpan::fromString(L"123").parseInt32() == 123);
    CHECK(irbis::WideSpan::fromString(L"1234").parseInt32() == 1234);
    CHECK(irbis::WideSpan::fromString(L"12345").parseInt32() == 12345);
    CHECK(irbis::WideSpan::fromString(L"123456").parseInt32() == 123456);
}

TEST_CASE("Span_parseInt64_1", "[span]")
{
    CHECK(irbis::CharSpan::fromString("").parseInt64() == 0);
    CHECK(irbis::CharSpan::fromString("0").parseInt64() == 0);
    CHECK(irbis::CharSpan::fromString("1").parseInt64() == 1);
    CHECK(irbis::CharSpan::fromString("12").parseInt64() == 12);
    CHECK(irbis::CharSpan::fromString("123").parseInt64() == 123);
    CHECK(irbis::CharSpan::fromString("1234").parseInt64() == 1234);
    CHECK(irbis::CharSpan::fromString("12345").parseInt64() == 12345);
    CHECK(irbis::CharSpan::fromString("123456").parseInt64() == 123456);
}

TEST_CASE("Span_parseInt64_2", "[span]")
{
    CHECK(irbis::WideSpan::fromString(L"").parseInt64() == 0);
    CHECK(irbis::WideSpan::fromString(L"0").parseInt64() == 0);
    CHECK(irbis::WideSpan::fromString(L"1").parseInt64() == 1);
    CHECK(irbis::WideSpan::fromString(L"12").parseInt64() == 12);
    CHECK(irbis::WideSpan::fromString(L"123").parseInt64() == 123);
    CHECK(irbis::WideSpan::fromString(L"1234").parseInt64() == 1234);
    CHECK(irbis::WideSpan::fromString(L"12345").parseInt64() == 12345);
    CHECK(irbis::WideSpan::fromString(L"123456").parseInt64() == 123456);
}
