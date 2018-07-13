#ifndef RECYCLE_SRC_PYRE_REDUCTION_H_
#define RECYCLE_SRC_PYRE_REDUCTION_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

/// @param feed feed yellowcake from voloxidation
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to refining
cyclus::Material::Ptr ReductionSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

class Reduction
  : public cyclus::Facility {
#pragma cyclus note { \
  "niche": "electroreduction", \
  "doc": \
  "Reduction primarily separates Cesium and Strontium waste from " \
  "the yellowcake feed. Waste from this procees is sent to storage " \
  "and the metal fuel and salt mixture is sent to the refining stage." \
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
  
  }
  	
} // namespace recycle

#endif // RECYCLE_SRC_PYRE_REDUCTION_H_
