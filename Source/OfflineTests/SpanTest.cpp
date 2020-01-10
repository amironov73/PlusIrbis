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
            Assert::IsTrue(0 == span.size());
        }

        TEST_METHOD(constructor_2)
        {
            int numbers[] { 1, 2, 3 };
            const Span<int> span(numbers, 3 );
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

        TEST_METHOD(fromString_1)
        {
            Span<char> span = Span<char>::fromString("Hello");
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
            std::string text("Hello");
            Span<char> span(text);
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
            std::string text = span.toString();
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
            Assert::IsTrue(span2.size() == 0);
            Assert::IsTrue(span2.empty());
        }
    };
}

