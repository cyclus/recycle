#include "pyre.h"
#include "pyre_volox.h"
#include "efficiency.h"

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
Material::Ptr Volox::VoloxSepMaterial(std::map<int, double> effs, Material::Ptr mat) {
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

    volox_eff = e->volox_eff;

    double qty = it->second;
    double sepqty = qty * eff * volox_eff;
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
};

