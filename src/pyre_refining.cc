#include "pyre.h"
#include "pyre_refining.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

namespace recycle {

Refine::Refine() {
  double temperature = 900;
  double pressure = 760;
  double rotation = 0;
  double batch_size = 20;
  double reprocess_time = 1;
}

Refine::Refine(refine_temp,refine_press,refine_rotation,refine_batch_size,refine_time) {
  double temperature = refine_temp;
  double pressure = refine_press;
  double rotation = refine_rotation;
  double batch_size = refine_batch_size;
  double reprocess_time = refine_time
}

// Note that this returns an untracked material that should just be used for
// its composition and qty - not in any real inventories, etc.
Material::Ptr RefineSepMaterial(std::map<int, double> effs, Material::Ptr mat) {
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

    double qty = it->second;
    double sepqty = qty * eff * Refine::Efficiency(temperature,pressure,rotation);
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
};

double Efficiency(temperature,pressure,rotation) {
  double thermal = (8.8333E-7*pow(temperature,3) - 0.00255*(temperature,2)+2.4572*temperature-691.1) / 100;
  double pres_eff = -0.0055128 * pressure + 100.5;
  if (rotation <= 1) {
    double agitation = 0.032*rotation + 0.72;
  } else {
    double agitation = 0.0369924675*log(rotation)+0.829777331;
    if (agitation > 1) {
      agitation = 1;
    }
  }
  double refine_eff = thermal * pres_eff * agitation;
  return refine_eff;
};

double Throughput(batch_size, reprocess_time) {
  double refine_through = refine_batch_size / reprocess_time;
  return refine_through;
};
}
