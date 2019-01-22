#ifndef RECYCLE_SRC_PYRE_TESTS_H_
#define RECYCLE_SRC_PYRE_TESTS_H_

#include "pyre.h"
#include "context.h"
#include "facility_tests.h"
#include "agent_tests.h"

namespace recycle {

class PyreTests : public ::testing::TestWithParam < std::string > {
    protected:
        cyclus::TestContext tc_;
        Pyre* src_facility_;

        virtual void SetUp();
        virtual void TearDown();
        void InitParameters();
        double default_efficiency;
        std::string config, simple_config;
        int simdur, life;
};

} // namespace recycle
#endif // RECYCLE_SRC_PYRE_TESTS_H_
