#include "tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace irbis;

namespace OfflineTests
{
    TEST_CLASS(MarcRecordTest)
    {
    public:

        TEST_METHOD(constructor_1)
        {
            const MarcRecord record;
            Assert::AreEqual(record.mfn, 0u);
            Assert::AreEqual(record.status, 0u);
            Assert::AreEqual(record.version, 0u);
            Assert::IsTrue(record.database.empty());
            Assert::IsTrue(record.fields.empty());
        }

    };
}
