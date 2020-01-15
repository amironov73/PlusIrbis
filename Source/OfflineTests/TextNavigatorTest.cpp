#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(TextNavigatorTest)
    {
    public:

        TEST_METHOD(constructor_1)
        {
            const String hello = L"Hello, world!";
            const TextNavigator navigator(hello);
            Assert::IsFalse(navigator.eot());
            Assert::AreEqual(navigator.line(), size_t(1));
            Assert::AreEqual(navigator.column(), size_t(1));
            Assert::AreEqual(navigator.position(), size_t(0));
            Assert::AreEqual(navigator.length(), size_t(13));
        }

        TEST_METHOD(constructor_2)
        {
            const String hello = L"Hello, world!";
            const TextNavigator navigator1(hello);
            const TextNavigator navigator2(navigator1);  // NOLINT(performance-unnecessary-copy-initialization)
            Assert::AreEqual(navigator2.line(), size_t(1));
            Assert::AreEqual(navigator2.column(), size_t(1));
            Assert::AreEqual(navigator2.position(), size_t(0));
            Assert::AreEqual(navigator2.length(), size_t(13));
        }

        TEST_METHOD(at_1)
        {
            const String hello = L"Hello, world!";
            const TextNavigator navigator(hello);
            Assert::AreEqual(navigator.at(0), L'H');
            Assert::AreEqual(navigator.at(-1), TextNavigator::EOT);
            Assert::AreEqual(navigator.at(100), TextNavigator::EOT);
            Assert::AreEqual(navigator.at(1), L'e');
            Assert::AreEqual(navigator.at(12), L'!');
        }

        TEST_METHOD(front_1)
        {
            const String hello = L"Hello, world!";
            const TextNavigator navigator(hello);
            Assert::IsTrue(navigator.front() == L'H');
        }

        TEST_METHOD(front_2)
        {
            const String hello;
            const TextNavigator navigator(hello);
            Assert::IsTrue(navigator.front() == TextNavigator::EOT);
        }

        TEST_METHOD(back_1)
        {
            const String hello = L"Hello, world!";
            const TextNavigator navigator(hello);
            Assert::IsTrue(navigator.back() == L'!');
        }

        TEST_METHOD(back_2)
        {
            const String hello;
            const TextNavigator navigator(hello);
            Assert::IsTrue(navigator.back() == TextNavigator::EOT);
        }

        TEST_METHOD(lookAhead_1)
        {
            const String hello = L"Hello, world!";
            const TextNavigator navigator(hello);
            Assert::IsTrue(navigator.lookAhead() == L'e');
            Assert::IsTrue(navigator.lookAhead(1) == L'e');
            Assert::IsTrue(navigator.lookAhead(2) == L'l');
            Assert::IsTrue(navigator.lookAhead(3) == L'l');
            Assert::IsTrue(navigator.lookAhead(4) == L'o');
        }

        TEST_METHOD(lookBehind_1)
        {
            const String hello = L"Hello, world!";
            TextNavigator navigator(hello);
            navigator.move(13);
            Assert::IsTrue(navigator.lookBehind() == L'!');
            Assert::IsTrue(navigator.lookBehind(1) == L'!');
            Assert::IsTrue(navigator.lookBehind(2) == L'd');
            Assert::IsTrue(navigator.lookBehind(3) == L'l');
            Assert::IsTrue(navigator.lookBehind(4) == L'r');
            Assert::IsTrue(navigator.lookBehind(5) == L'o');
            Assert::IsTrue(navigator.lookBehind(6) == L'w');
        }

        TEST_METHOD(move_1)
        {
            const String hello = L"Hello, world!";
            TextNavigator navigator(hello);
            navigator.move(10);
            Assert::IsTrue(navigator.position() == 10);
        }

        TEST_METHOD(peekChar_1)
        {
            const String hello = L"Hello, world!";
            TextNavigator navigator(hello);
            Assert::IsTrue(navigator.peekChar() == L'H');
            Assert::IsTrue(navigator.peekChar() == L'H');
            navigator.move(5);
            Assert::IsTrue(navigator.peekChar() == L',');
            Assert::IsTrue(navigator.peekChar() == L',');
            navigator.move(13);
            Assert::IsTrue(navigator.peekChar() == TextNavigator::EOT);
            Assert::IsTrue(navigator.peekChar() == TextNavigator::EOT);
        }

        TEST_METHOD(readChar_1)
        {
            const String hello = L"Hello, world!";
            TextNavigator navigator(hello);
            Assert::IsTrue(navigator.readChar() == L'H');
            Assert::IsTrue(navigator.readChar() == L'e');
            Assert::IsTrue(navigator.readChar() == L'l');
            Assert::IsTrue(navigator.readChar() == L'l');
            Assert::IsTrue(navigator.readChar() == L'o');
            Assert::IsTrue(navigator.readChar() == L',');
            Assert::IsTrue(navigator.readChar() == L' ');
            Assert::IsTrue(navigator.readChar() == L'w');
            Assert::IsTrue(navigator.readChar() == L'o');
            Assert::IsTrue(navigator.readChar() == L'r');
            Assert::IsTrue(navigator.readChar() == L'l');
            Assert::IsTrue(navigator.readChar() == L'd');
            Assert::IsTrue(navigator.readChar() == L'!');
            Assert::IsTrue(navigator.readChar() == TextNavigator::EOT);
            Assert::IsTrue(navigator.readChar() == TextNavigator::EOT);
        }

        TEST_METHOD(peekString_1)
        {
            const String hello = L"Hello\nworld";
            TextNavigator navigator(hello);
            Assert::IsTrue(navigator.peekString(5).toString() == L"Hello");
            Assert::IsTrue(navigator.peekString(5).toString() == L"Hello");
            navigator.move(1);
            Assert::IsTrue(navigator.peekString(5).toString() == L"ello");
            Assert::IsTrue(navigator.peekString(5).toString() == L"ello");
            navigator.move(5);
            Assert::IsTrue(navigator.peekString(5).toString() == L"world");
            Assert::IsTrue(navigator.peekString(5).toString() == L"world");
            navigator.move(1);
            Assert::IsTrue(navigator.peekString(5).toString() == L"orld");
            Assert::IsTrue(navigator.peekString(5).toString() == L"orld");
        }

        TEST_METHOD(readLine_1)
        {
            const String hello = L"Hello\nworld";
            TextNavigator navigator(hello);
            Assert::IsTrue(navigator.readLine().toString() == L"Hello");
            Assert::IsTrue(navigator.readLine().toString() == L"world");
            Assert::IsTrue(navigator.readLine().empty());
            Assert::IsTrue(navigator.eot());
        }
    };

}
