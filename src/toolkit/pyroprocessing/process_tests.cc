#include "process_tests.h"

using pyne::nucname::id;
using cyclus::Composition;
using cyclus::CompMap;
using cyclus::Material;
using cyclus::QueryResult;
using cyclus::Cond;
using cyclus::toolkit::MatQuery;

namespace recycle{

TEST_F(ProcessTests, SepMixElemAndNuclide) {
  std::string config = 
    "<streams>"
      "    <item>"
      "        <commod>refine_test</commod>"
      "        <info>"
      "            <buf_size>-1</buf_size>"
      "            <efficiencies>"
      "                <item><comp>U</comp> <eff>0.6</eff></item>"
      "                <item><comp>Pu239</comp> <eff>.7</eff></item>"
      "            </efficiencies>"
      "        </info>"
      "    </item>"
      "</streams>"
      ""
      "<leftover_commod>waste</leftover_commod>"
      "<throughput>100</throughput>"
      "<feedbuf_size>100</feedbuf_size>"
      "<feed_commods> <val>feed</val> </feed_commods>";

  CompMap m;
  m[id("u235")] = 0.08;
  m[id("u238")] = 0.9;
  m[id("Pu239")] = .01;
  m[id("Pu240")] = .01;
  Composition::Ptr c = Composition::CreateFromMass(m);

  int simdur = 2;
  cyclus::MockSim sim(cyclus::AgentSpec(":recycle:Pyre"), config, simdur);
  sim.AddSource("feed").recipe("recipe1").Finalize();
  sim.AddSink("refine_test").capacity(100).Finalize();
  sim.AddRecipe("recipe1", c);
  int id = sim.Run();

  std::vector<Cond> conds;
  conds.push_back(Cond("SenderId", "==", id));
  int resid = sim.db().Query("Transactions", &conds).GetVal<int>("ResourceId");
  MatQuery mq (sim.GetMaterial(resid));
  // default_efficiency is the Electrorefiner's separation efficiency given Pyre's default
  // values for temperature, pressure, and rotation.
  double default_efficiency = 0.6778044172175635;
  EXPECT_DOUBLE_EQ(m[922350000]*0.6*default_efficiency*100, mq.mass("U235"));
  EXPECT_DOUBLE_EQ(m[922380000]*0.6*default_efficiency*100, mq.mass("U238"));
  EXPECT_DOUBLE_EQ(m[942390000]*0.7*default_efficiency*100, mq.mass("Pu239"));
  EXPECT_DOUBLE_EQ(0, mq.mass("Pu240"));
}

} //namespace recycle
