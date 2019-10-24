#ifndef RECYCLE_SRC_PROCESS_TESTS_H_
#define RECYCLE_SRC_PROCESS_TESTS_H_

#include <gtest/gtest.h>

#include "pyre.h"
#include "context.h"
#include "facility_tests.h"
#include "agent_tests.h"
#include <sstream>
#include "cyclus.h"

namespace recycle {

class ProcessTests : public ::testing::Test {
    protected:
        recycle::Pyre* src_facility;
        std::string simple_config;
    };

} // namespace recycle
#endif // RECYCLE_SRC_PROCESS_TESTS_H_
