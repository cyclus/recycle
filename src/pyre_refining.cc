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

Refine::Refine() {}

Refine::Refine(double refine_temp = 900, double refine_press = 760, 
               double refine_rotation = 0, double refine_batch_size = 20,
               double refine_time = 1) {
  temperature = refine_temp;
  pressure = refine_press;
  rotation = refine_rotation;
  batch_size = refine_batch_size;
  reprocess_time = refine_time;
}

// This returns an untracked material that should just be used for
// its composition and qty - not in any real inventories, etc.
Material::Ptr Refine::RefineSepMaterial(std::map<int, double> effs, Material::Ptr mat) {
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
    double sepqty = qty * eff * Refine::Efficiency(temperature, pressure, rotation);
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
}

double Refine::Efficiency(double temperature, double pressure, double rotation) {
  double agitation;
  double thermal = (8.8333E-7*pow(temperature,3) - 0.00255*(temperature,2)
                   +2.4572*temperature-691.1) / 100;
  double pres_eff = -0.0055128 * pressure + 100.5;
  if (rotation <= 1) {
    agitation = 0.032*rotation + 0.72;
  } else {
    agitation = 0.0369924675*log(rotation)+0.829777331;
    if (agitation > 1) {
      throw ValueError("Rotation efficiency cannot exceed 1");
    }
  }
  double refine_eff = thermal * pres_eff * agitation;
  return refine_eff;
}

double Refine::Throughput(double batch_size, double reprocess_time) {
  double refine_through = batch_size / reprocess_time;
  return refine_through;
};
}
