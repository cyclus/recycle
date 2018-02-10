// Implements the GrowthRegion class
#include "growth_region.h"

namespace recycle {

GrowthRegion::GrowthRegion(cyclus::Context* ctx) : cyclus::Region(ctx) { }

GrowthRegion::~GrowthRegion() {}

void GrowthRegion::AddCommodityDemand_(std::string commod,
                                       Demand& demand) {
  
  
  cyclus::toolkit::PiecewiseFunctionFactory pff;
  cyclus::toolkit::BasicFunctionFactory bff;
  bool continuous = false;
  int time;
  std::string type, params;
  Demand::const_iterator it;
  for (it = demand.begin(); it != demand.end(); it++) {
    time = it->first;
    type = it->second.first;
    params = it->second.second;
    pff.AddFunction(bff.GetFunctionPtr(type, params), time, continuous);
    continuous = true; // only the first entry is not continuous
  }

  // register the commodity and demand
  cyclus::toolkit::Commodity c(commod);
  sdmanager_.RegisterCommodity(c, pff.GetFunctionPtr());
}

void GrowthRegion::EnterNotify() {
  cyclus::Region::EnterNotify();
  Register_(this);

  std::map<std::string, Demand>::iterator it;
  for (it = commodity_demand.begin(); it != commodity_demand.end(); ++it) {
    LOG(cyclus::LEV_INFO3, "greg") << "Adding demand for commodity "
                                   << it->first;
    AddCommodityDemand_(it->first, it->second);
  }
}

void GrowthRegion::DecomNotify(Agent* a) {
  Unregister_(a);
}



void GrowthRegion::Register_(cyclus::Agent* agent) {
  using cyclus::toolkit::CommodityProducerManager;
  using cyclus::toolkit::CommodityProducer;
  using cyclus::toolkit::Builder;

  CommodityProducerManager* cpm_cast = dynamic_cast<CommodityProducerManager*>(agent);
  if (cpm_cast != NULL){
    LOG(cyclus::LEV_INFO3, "mani") << "Registering agent "
                                   << agent->prototype() << agent->id()
                                   << " as a commodity producer Manager.";
    sdmanager_.RegisterProducerManager(cpm_cast);
  }

  CommodityProducer* cp_cast = dynamic_cast<CommodityProducer*>(agent);
  if (cp_cast != NULL){
    LOG(cyclus::LEV_INFO3, "mani") << "Registering agent "
                                   << agent->prototype() << agent->id()
                                   << " as a commodity producer.";
    CommodityProducerManager::Register(cp_cast);
  }

  Builder* b_cast = dynamic_cast<Builder*>(agent);
  if (b_cast != NULL) {
    LOG(cyclus::LEV_INFO3, "greg") << "Registering agent "
                                   << agent->prototype() << agent->id()
                                   << " as a builder.";
    buildmanager_.Register(b_cast);
  }
}

void GrowthRegion::Unregister_(cyclus::Agent* agent) {
  using cyclus::toolkit::CommodityProducerManager;
  using cyclus::toolkit::CommodityProducer;
  using cyclus::toolkit::Builder;

  CommodityProducerManager* cpm_cast = dynamic_cast<CommodityProducerManager*>(agent);
  if (cpm_cast != NULL){
    sdmanager_.UnregisterProducerManager(cpm_cast);
  }

  CommodityProducer* cp_cast = dynamic_cast<CommodityProducer*>(agent);
  if (cp_cast != NULL){
    CommodityProducerManager::Unregister(cp_cast);
  }

  Builder* b_cast = dynamic_cast<Builder*>(agent);
  if (b_cast != NULL)
    buildmanager_.Unregister(b_cast);
}

void GrowthRegion::Tock(){
  Recursive_Unregister(this);
}



void GrowthRegion::Recursive_Unregister(cyclus::Agent* a){
  // Unregisters the descendants of this GrowthRegion recursively.
  std::set<cyclus::Agent*>::iterator ait;
  if  (a->children().begin() == a->children().end()){
    return;
  }
  else{
    for (ait = a->children().begin();
         ait != a->children().end();
         ++ait){
      Agent* child = *ait;
      if (child->exit_time() == context()->time()){
        std::cout << "Agent exiting now will be unregistered: " << child->prototype() << std::endl;
        Unregister_(child);
      }
      Recursive_Unregister(child);
    }
  }
}

void GrowthRegion::Recursive_Register(cyclus::Agent* a){
  // Registers the descendants of this GrowthRegion recursively.
  std::set<cyclus::Agent*>::iterator ait;
  if  (a->children().begin() == a->children().end()){
    return;
  }
  else{
    for (ait = a->children().begin();
         ait != a->children().end();
         ++ait){
      Agent* child = *ait;
      if (child->enter_time() == context()->time()){
        std::cout << "Agent entering now will be registered: " << child->prototype() << std::endl;
        Register_(child);
      }
      Recursive_Register(child);
    }
  }
}

void GrowthRegion::Tick() {
  // registers agents in the region as commodity supplier
  Recursive_Register(this);

  double demand, supply, unmetdemand;
  cyclus::toolkit::Commodity commod;
  int time = context()->time();
  std::map<std::string, Demand>::iterator it;
  for (it = commodity_demand.begin(); it != commodity_demand.end(); ++it) {
    commod = cyclus::toolkit::Commodity(it->first);
    demand = sdmanager_.Demand(commod, time);
    supply = sdmanager_.Supply(commod);
    unmetdemand = demand - supply;

    LOG(cyclus::LEV_INFO3, "greg") << "GrowthRegion: " << prototype()
                                   << " at time: " << time
                                   << " has the following values regarding "
                                   << " commodity: " << commod.name();
    LOG(cyclus::LEV_INFO3, "greg") << "  * demand = " << demand;
    LOG(cyclus::LEV_INFO3, "greg") << "  * supply = " << supply;
    LOG(cyclus::LEV_INFO3, "greg") << "  * unmet demand = " << unmetdemand;
    
    if (unmetdemand > 0) {
      OrderBuilds(commod, unmetdemand);
    }
  }
  cyclus::Region::Tick();
}

void GrowthRegion::OrderBuilds(cyclus::toolkit::Commodity& commodity,
                               double unmetdemand) {
  using std::vector;
  vector<cyclus::toolkit::BuildOrder> orders =
    buildmanager_.MakeBuildDecision(commodity, unmetdemand);

  LOG(cyclus::LEV_INFO3, "greg")
      << "The build orders have been determined. "
      << orders.size()
      << " different type(s) of prototypes will be built.";

  cyclus::toolkit::BuildOrder* order;
  cyclus::Institution* instcast;
  cyclus::Agent* agentcast;
  for (int i = 0; i < orders.size(); i++) {
    order = &orders.at(i);
    instcast = dynamic_cast<cyclus::Institution*>(order->builder);
    agentcast = dynamic_cast<cyclus::Agent*>(order->producer);
    if (!instcast || !agentcast) {
      throw cyclus::CastError("growth_region has tried to incorrectly "
                              "cast an already known entity.");
    }

    LOG(cyclus::LEV_INFO3, "greg")
        << "A build order for " << order->number
        << " prototype(s) of type "
        << dynamic_cast<cyclus::Agent*>(agentcast)->prototype()
        << " from builder " << instcast->prototype()
        << " is being placed.";

    for (int j = 0; j < order->number; j++) {
      LOG(cyclus::LEV_DEBUG2, "greg") << "Ordering build number: " << j + 1;
      context()->SchedBuild(instcast, agentcast->prototype());
    }
  }
}

extern "C" cyclus::Agent* ConstructGrowthRegion(cyclus::Context* ctx) {
  return new GrowthRegion(ctx);
  }

} // namespace recycle