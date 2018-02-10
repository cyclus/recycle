#ifndef RECYCLE_SRC_SOURCE_TESTS_H_
#define RECYCLE_SRC_SOURCE_TESTS_H_
#include "source.h"

#include <gtest/gtest.h>

#include <boost/shared_ptr.hpp>

#include "agent_tests.h"
#include "context.h"
#include "exchange_context.h"
#include "facility_tests.h"
#include "material.h"

namespace recycle {

class SourceTest : public ::testing::Test {
 public:
  cyclus::TestContext tc;
  TestFacility* trader;
  recycle::Source* src_facility;
  std::string commod, recipe_name;
  double capacity;
  cyclus::Composition::Ptr recipe;

  virtual void SetUp();
  virtual void TearDown();
  void InitParameters();
  void SetUpSource();

  std::string outrecipe(recycle::Source* s) { return s->outrecipe; }
  std::string outcommod(recycle::Source* s) { return s->outcommod; }
  double throughput(recycle::Source* s) { return s->throughput; }

  void outrecipe(recycle::Source* s, std::string recipe) {
    s->outrecipe = recipe;
  }
  void outcommod(recycle::Source* s, std::string commod) {
    s->outcommod = commod;
  }
  void throughput(recycle::Source* s, double val) { s->throughput = val; }

  boost::shared_ptr<cyclus::ExchangeContext<cyclus::Material> > GetContext(
      int nreqs, std::string commodity);
};

} // namespace recycle

#endif  // RECYCLE_SRC_SOURCE_TESTS_H_
