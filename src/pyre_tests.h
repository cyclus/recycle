#ifndef RECYCLE_SRC_PYRE_TESTS_H_
#define RECYCLE_SRC_PYRE_TESTS_H_

#include "pyre.h"
#include "context.h"
#include "facility_tests.h"
#include "agent_tests.h"

namespace recycle {

class PyreTests : public ::testing::TestWithParam < std::string > {
    protected:
        Pyre* src_facility_;

        virtual void SetUp();
        virtual void TearDown();
};

} // namespace recycle
#endif // RECYCLE_SRC_PYRE_TESTS_H_
