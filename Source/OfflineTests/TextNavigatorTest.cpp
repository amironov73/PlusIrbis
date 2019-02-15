#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(TextNavigatorTest)
    {
    public:

        TEST_METHOD(constructor_1)
        {
            std::wstring hello = L"Hello, world!";
            TextNavigator navigator(hello);
            Assert::IsFalse(navigator.eot());
            Assert::AreEqual(navigator.line(), size_t(1));
            Assert::AreEqual(navigator.column(), size_t(1));
            Assert::AreEqual(navigator.position(), size_t(0));
            Assert::AreEqual(navigator.length(), size_t(13));
        }

        TEST_METHOD(constructor_2)
        {
            std::wstring hello = L"Hello, world!";
            TextNavigator navigator1(hello);
            TextNavigator navigator2(navigator1);
            Assert::AreEqual(navigator2.line(), size_t(1));
            Assert::AreEqual(navigator2.column(), size_t(1));
            Assert::AreEqual(navigator2.position(), size_t(0));
            Assert::AreEqual(navigator2.length(), size_t(13));
        }

        TEST_METHOD(at_1)
        {
            std::wstring hello = L"Hello, world!";
            TextNavigator navigator(hello);
            Assert::AreEqual(navigator.at(0), L'H');
            Assert::AreEqual(navigator.at(-1), TextNavigator::EOT);
            Assert::AreEqual(navigator.at(100), TextNavigator::EOT);
            Assert::AreEqual(navigator.at(1), L'e');
            Assert::AreEqual(navigator.at(12), L'!');
        }


    };

}
