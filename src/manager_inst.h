#ifndef RECYCLE_SRC_MANAGER_INST_H_
#define RECYCLE_SRC_MANAGER_INST_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

/// @class ManagerInst
/// @section introduction Introduction
/// @section detailedBehavior Detailed Behavior
/// @warning The ManagerInst is experimental
class ManagerInst
    : public cyclus::Institution,
      public cyclus::toolkit::CommodityProducerManager,
      public cyclus::toolkit::Builder {
 public:
  /// Default constructor
  ManagerInst(cyclus::Context* ctx);

  /// Default destructor
  virtual ~ManagerInst();

  virtual std::string version() { return RECYCLE_VERSION; }

  #pragma cyclus

  #pragma cyclus note {"doc": "An institution that owns and operates a " \
                              "manually entered list of facilities in " \
                              "the input file"}

  /// enter the simulation and register any children present
  virtual void EnterNotify();

  /// write information about a commodity producer to a stream
  /// @param producer the producer
  void WriteProducerInformation(cyclus::toolkit::CommodityProducer*
                                producer);

 private:
  #pragma cyclus var { \
    "tooltip": "producer facility prototypes",                          \
    "uilabel": "Producer Prototype List",                               \
    "uitype": ["oneormore", "prototype"],                                    \
    "doc": "A set of facility prototypes that this institution can build. " \
    "All prototypes in this list must be based on an archetype that "   \
    "implements the cyclus::toolkit::CommodityProducer interface",      \
    }
  std::vector<std::string> prototypes;
};

}  // namespace recycle

#endif  // RECYCLE_SRC_MANAGER_INST_H_
