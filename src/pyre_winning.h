#ifndef RECYCLE_SRC_PYRE_WINNING_H_
#define RECYCLE_SRC_PYRE_WINNING_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

/// @param feed feed yellowcake from voloxidation
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to fuel fabrication
cyclus::Material::Ptr WinningSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

class Winning
  : public cyclus::Facility {
#pragma cyclus note { \
  "niche": "electrowinning", \
  "doc": \
  "Voloxidation processes out actinides and noble gasses in the" \
  "pyroprocessing process. Waste from this stage is sent to storage" \
  "and yellowcake product is sent to reduction." \
}
public:
  Winning(cyclus::Context* ctx);
  virtual ~Winning(){};

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

#endif // RECYCLE_SRC_PYRE_WINNING_H_
