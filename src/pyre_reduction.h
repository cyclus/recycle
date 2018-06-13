#ifndef RECYCLE_SRC_PYRE_REDUCTION_H_
#define RECYCLE_SRC_PYRE_REDUCTION_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

cyclus::Material::Ptr SepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

class Reduction
  : public cyclus::Facility {
#pragma cyclus note { \
  "niche": "voloxidation", \
  "doc": \
  "Voloxidation processes out actinides and noble gasses in the" \
  "pyroprocessing process. Waste from this stage is sent to storage" \
  "and yellowcake product is sent to reduction." \
}
public:
  Reduction(cyclus::Context* ctx);
  virtual ~Reduction(){};

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
	  "units": "m3", \
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
    "doc": "Batch size of the subprocess", \
    "tooltip": "Batch size of the subprocess", \
    "units": "kg", \
    "uilabel": "Batch Size" \
  }
  double batch_size;

  #pragma cyclus var { \
    "doc": "Weight percent of lithium oxide added as catalyst", \
    "tooltip": "Weight percent of lithium oxide", \
    "units": "percent", \
    "uilabel": "Lithium Oxide" \
  }
  double lithium_oxide;

  #pragma cyclus var { \
    "doc": "Current through the reduction subprocess", \
    "tooltip": "Current through the reduction subprocess", \
    "units": "mA", \
    "uilabel": "Current" \
  }
  double current;

  #pragma cyclus var { \
    "doc": "Porosity of the anode shroud material", \
    "tooltip": "Anode porosity", \
    "units": "", \
    "uilabel": "Current" \
  }
  double porosity;

  }
  	
} // namespace recycle

#endif // RECYCLE_SRC_PYRE_REDUCTION_H_
