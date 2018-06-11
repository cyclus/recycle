#ifndef RECYCLE_SRC_PYRE_VOLOX_H_
#define RECYCLE_SRC_PYRE_VOLOX_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

cyclus::Material::Ptr SepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

class Volox
  : public cyclus::Facility {
#pragma cyclus note { \
  "niche": "voloxidation", \
  "doc": \
  "Voloxidation processes out actinides and noble gasses in the" \
  "pyroprocessing process. Waste from this stage is sent to storage" \
  "and yellowcake product is sent to reduction." \
}
public:
  Volox(cyclus::Context* ctx);
  virtual ~Volox(){};

  virtual std::string version() { return RECYCLE_VERSION; }

  virtual void Tick();
  virtual void Tock();

  #pragma cyclus clone
  #pragma cyclus initfromcopy
  #pragma cyclus infiletodb
  #pragma cyclus initfromdb
  #pragma cyclus schema
  #pragma cyclus annotations
  #pragma cyclus snapshot

  virtual cyclus::Inventories SnapshotInv();
  virtual void InitInv(cyclus::Inventories& inv);

 private:
  #pragma cyclus var { \
 	"doc": "Ordered list of commodities on which to request feed material to " \
 		   "separate. Order only matters for matching up with feed commodity " \
 		   "preferences if specified.", \
 	"uilabel": "Feed Commodity List", \
 	"uitype": ["oneormore", "incommodity"], \
  }
  std::vector<std::string> feed_commods;

  #pragma cyclus var { \
	"doc": "Volume of the subprocess container", \
	"tooltip": "Volume of the subprocess", \
	"units": "m^3", \
	"uilabel": "Volume" \
  }
  double volume;

  #pragma cyclus var { \
	"doc": "Amount of time spent in subprocess", \
	"tooltip": "Amount of time spent in subprocess", \
	"units": "hours", \
	"uilabel": "Reprocess Time" \
  }
  int reprocess_time;

  #pragma cyclus var { \
	"doc": "Flow rate of material through the voloxidation process.", \
	"uilabel": "Flow Rate", \
	"default": 100, \
	"units": "kgs-1", \
  }
  double flow_rate;

  #pragma cyclus var { \
    "doc": "Temperature of the subprocess", \
    "tooltip": "Temperature of the subprocess", \
    "units": "K", \
    "uilabel": "Temperature" \
  }
  double temperature;

  }
  	
} // namespace recycle

#endif // RECYCLE_SRC_PYRE_VOLOX_H_