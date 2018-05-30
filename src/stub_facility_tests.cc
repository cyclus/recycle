#include <gtest/gtest.h>

#include "stub_facility.h"

#include "agent_tests.h"
#include "context.h"
#include "facility_tests.h"

using recycle::StubFacility;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class StubFacilityTest : public ::testing::Test {
 protected:
  cyclus::TestContext tc;
  StubFacility* facility;

  virtual void SetUp() {
    facility = new StubFacility(tc.get());
  }

  virtual void TearDown() {
    delete facility;
  }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(StubFacilityTest, InitialState) {
  EXPECT_EQ(0, 0);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Do Not Touch! Below section required for connection with Cyclus
cyclus::Agent* StubFacilityConstructor(cyclus::Context* ctx) {
  return new StubFacility(ctx);
}
// Required to get functionality in cyclus agent unit tests library
#ifndef CYCLUS_AGENT_TESTS_CONNECTED
int ConnectAgentTests();
static int cyclus_agent_tests_connected = ConnectAgentTests();
#define CYCLUS_AGENT_TESTS_CONNECTED cyclus_agent_tests_connected
#endif  // CYCLUS_AGENT_TESTS_CONNECTED
INSTANTIATE_TEST_CASE_P(StubFac, FacilityTests,
                        ::testing::Values(&StubFacilityConstructor));
INSTANTIATE_TEST_CASE_P(StubFac, AgentTests,
                        ::testing::Values(&StubFacilityConstructor));
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
