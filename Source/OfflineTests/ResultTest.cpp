#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(ResultTest)
    {
    public:

        TEST_METHOD(success_1)
        {
            auto r1 = Result<int>::Success(123);
            Assert::IsTrue(r1.success);
            Assert::AreEqual(123, r1.result);
        }

        TEST_METHOD(failure_1)
        {
            auto r1 = Result<int>::Failure(L"Can't do it");
            Assert::IsFalse(r1.success);
            Assert::AreEqual(std::wstring(L"Can't do it"), r1.errorMessage);
        }

        TEST_METHOD(operator_1)
        {
            auto r1 = Result<int>::Success(123);
            if (!r1)
            {
                Assert::Fail(L"Can't be!");
            }
            const int n1 = r1;
            Assert::AreEqual(123, n1);
        }
    };
}
