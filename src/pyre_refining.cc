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
  temp = refine_temp;
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
  double sepeff = Efficiency(temp, pressure, rotation);

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
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
}

double Refine::Efficiency(double temp, double pressure, double rotation) {
  double agitation;
  double thermal = 4.7369E-9*pow(temp,3) - 1.08337E-5*pow(temp,2)
    +0.008069*temp-0.9726;
  double pres_eff = -4.6667*pow(pressure,-5) + 1.002;
  if (rotation <= 1) {
    agitation = 0.032*rotation + 0.72;
  } else {
    agitation = 0.0338396*log(rotation)+0.836671495;
    if (agitation > 1) {
      throw ValueError("Rotation efficiency cannot exceed 1");
    }
  }
  double refine_eff = thermal * pres_eff * agitation;
  std::cout << "Refine Eff = " << pres_eff << std::endl;
  return refine_eff;
}

double Refine::Throughput(double batch_size, double reprocess_time) {
  double refine_through = batch_size / reprocess_time;
  return refine_through;
};
}
