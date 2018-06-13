#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

cyclus::Material::Ptr SepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

class Refining
  : public cyclus::Facility {
#pragma cyclus note { \
  "niche": "voloxidation", \
  "doc": \
  "Voloxidation processes out actinides and noble gasses in the" \
  "pyroprocessing process. Waste from this stage is sent to storage" \
  "and yellowcake product is sent to reduction." \
}
public:
  Refining(cyclus::Context* ctx);
  virtual ~Refining(){};

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
    "doc": "Anode rotation speed", \
    "default": 0, \
    "tooltip": "Anode rotation", \
    "units": "rpm", \
    "uilabel": "Rotation Speed" \
  }
  double rotation;

  #pragma cyclus var { \
    "doc": "Center Stirrer Speed", \
    "default": 0, \
    "tooltip": "Stirrer Speed", \
    "units": "rpm", \
    "uilabel": "Stirrer Speed" \
  }
  double stirrer;

  #pragma cyclus var { \
    "doc": "Weight percent of lithium oxide added as catalyst", \
    "tooltip": "Weight percent of lithium oxide", \
    "units": "percent", \
    "uilabel": "Lithium Oxide" \
  }
  double lithium_oxide;

  #pragma cyclus var { \
    "doc": "Pressure in the electrorefining process", \
    "tooltip": "Pressure in the electrorefining process", \
    "units": "mTorr", \
    "uilabel": "Refiner Pressure" \
  }
  double pressure;

  #pragma cyclus var { \
    "doc": "Temperature of the subprocess", \
    "tooltip": "Temperature of the subprocess", \
    "units": "K", \
    "uilabel": "Temperature" \
  }
  double temperature;


  }
  	
} // namespace recycle

#endif // RECYCLE_SRC_PYRE_REFINING_H_
