#include "pyre_reduction.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;


// Note that this returns an untracked material that should just be used for
// its composition and qty - not in any real inventories, etc.
Material::Ptr Reduct::ReductionSepMaterial(std::map<int, double> effs, Material::Ptr mat) {
  CompMap cm = mat->comp()->mass();
  cyclus::compmath::Normalize(&cm, mat->quantity());
  double tot_qty = 0;
  CompMap sepcomp;

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

    reduct_eff = e->reduct_eff;

    double qty = it->second;
    double sepqty = qty * eff * reduct_eff;
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
};

Reduct::Efficiency(reduct_current, reduct_li2o) {
  double coulombic_eff = -0.00685*pow(current,4) + 0.20413*pow(current,3) - 2.273*pow(current,2) + 11.2046*current - 19.7493;
  double catalyst_eff = 0.075 * lithium_oxide + 0.775;
  double reduct_eff = coulombic_eff * catalyst_eff;
  return reduct_eff;
};

Reduct::Throughput(reduct_volume, reduct_time) {
  double reduct_through = reduct_volume / reduct_time;
  return reduct_through;
}
