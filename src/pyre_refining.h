#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

cyclus::Material::Ptr RefineSepMaterial(std::map<int, double> effs,
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

  }
  	
} // namespace recycle

#endif // RECYCLE_SRC_PYRE_REFINING_H_
