#include "pyre.h"
#include "pyre_reduction.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

namespace recycle {

Reduct::Reduct() {}

Reduct::Reduct(double reduct_current = 5, double reduct_lithium_oxide = 2, 
               double reduct_volume = 10, double reduct_time = 1) {
  current = reduct_current;
  lithium_oxide = reduct_lithium_oxide;
  volume = reduct_volume;
  reprocess_time = reduct_time;
}

// This returns an untracked material that should just be used for
// its composition and qty - not in any real inventories, etc.
Material::Ptr Reduct::ReductSepMaterial(std::map<int, double> effs, 
  Material::Ptr mat) {
  CompMap cm = mat->comp()->mass();
  cyclus::compmath::Normalize(&cm, mat->quantity());
  double tot_qty = 0;
  CompMap sepcomp;
  double sepeff = Efficiency(current, lithium_oxide);

  CompMap::iterator it;
  for (it = cm.begin(); it != cm.end(); ++it) {
    int nuc = it->first;
    int elem = (nuc / 10000000) * 10000000;
    double eff = 0;
    if (effs.count(nuc) > 0) {
      eff = effs[nuc];
    } else if (effs.count(elem) > 0) {
      eff = effs[elem];
    } else {
      continue;
    }

    double qty = it->second;
    double sepqty = qty * eff * sepeff;
    std::cout << nuc << std::endl;
    std::cout << "Nuc Id qty: " << sepqty << std::endl;
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
}

double Reduct::Efficiency(double current, double lithium_oxide) {
  double coulombic_eff = -0.00685*pow(current,4) + 0.20413*pow(current,3) 
                         - 2.273*pow(current,2) + 11.2046*current - 19.7493;
  double catalyst_eff = 0.075 * lithium_oxide + 0.775;
  std::cout << "Coulombic = " << coulombic_eff << std::endl;
  std::cout << "Lithim = " << catalyst_eff << std::endl;
  double reduct_eff = coulombic_eff * catalyst_eff;
  return reduct_eff;
}

double Reduct::Throughput(double volume, double reprocess_time) {
  double reduct_through = volume / reprocess_time;
  return reduct_through;
};
}
