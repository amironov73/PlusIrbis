#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(SubFieldListTest)
    {
    public:

        TEST_METHOD(add_1)
        {
            SubFieldList list;
            list.add('a', L"SubA");
            list.add('b', L"SubB");
            Assert::AreEqual(size_t(2), list.size());
            //std::wstring s;
            //for (const SubField &item : list)
            //{
            //    s += item.wstr();
            //}
            //Assert::AreEqual(std::wstring(L"^aSubA^bSubB"), s);
        }

    };
}
