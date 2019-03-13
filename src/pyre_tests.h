#ifndef RECYCLE_SRC_PYRE_TESTS_H_
#define RECYCLE_SRC_PYRE_TESTS_H_

#include <gtest/gtest.h>

#include "pyre.h"
#include "context.h"
#include "facility_tests.h"
#include "agent_tests.h"

namespace recycle {

class PyreTests : public ::testing::Test {
    protected:
        cyclus::TestContext tc_;
        recycle::Pyre* src_facility;
        std::string simple_config;
        virtual void SetUp();
        virtual void TearDown();
        void InitParameters();
        void SetupPyre();
    };

} // namespace recycle
#endif // RECYCLE_SRC_PYRE_TESTS_H_
