#include "pyre.h"
#include "pyre_volox.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

namespace recycle {

Volox::Volox() {}

Volox::Volox(double volox_temp = 900, double volox_time = 1, 
             double volox_flowrate = 3, double volox_volume = 1){
  temp = volox_temp;
  reprocess_time = volox_time;
  flowrate = volox_flowrate;
  volume = volox_volume;
}

// This returns an untracked material that should just be used for
// its composition and qty - not in any real inventories, etc.
Material::Ptr Volox::VoloxSepMaterial(std::map<int, double> effs, Material::Ptr mat) {
  CompMap cm = mat->comp()->mass();
  cyclus::compmath::Normalize(&cm, mat->quantity());
  double tot_qty = 0;
  CompMap sepcomp;

  CompMap::iterator it;
  for (it = cm.begin(); it != cm.end(); ++it) {
    int nuc = it->first;
    int elem = nuc;
    double eff = 0;
    if (effs.count(nuc) > 0) {
      eff = effs[nuc];
    } else if (effs.count(elem) > 0) {
      eff = effs[elem];
    } else {
      continue;
    }

    double qty = it->second;
    double sepqty = qty * eff * Volox::Efficiency(temp, reprocess_time, flowrate);
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
}

double Volox::Efficiency(double temp, double reprocess_time, double flowrate) {
  double thermal = (8.8333E-7*pow(temp,3) - 0.001755*(temp,2)+1.166*temp-159.6) / 100;
  double temporal = 0.2903 * log(reprocess_time*3600) - 1.696;
  double rate = 0.12435 * log(flowrate) + 0.7985;
  double volox_eff = thermal * temporal * rate;
  return volox_eff;
}

double Volox::Throughput(double flowrate, double reprocess_time, double volume) {
  // placeholder calculation
  double volox_through = volume / flowrate*reprocess_time;
  return volox_through;
}
}
