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

Volox::Volox(double volox_temp, 
             double volox_time, 
             double volox_flowrate, 
             double volox_volume
        ) 
        {
          temp.push(volox_temp);
          reprocess_time.push(volox_time);
          flowrate.push(volox_flowrate);
          volume.push(volox_volume); 
}

// This returns an untracked material that should just be used for
// its composition and qty - not in any real inventories, etc.
Material::Ptr Volox::VoloxSepMaterial(std::map<int, double> effs, Material::Ptr mat) {
  CompMap cm = mat->comp()->mass();
  cyclus::compmath::Normalize(&cm, mat->quantity());
  double tot_qty = 0;
  CompMap sepcomp;
  double sepeff = Efficiency(temp, reprocess_time, flowrate);

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

double Volox::Efficiency(std::vector<double> temp, 
  std::vector<double> reprocess_time, std::vector<double> flowrate) {
  double tmp = temp.back();
  double rep_time = reprocess_time.back();
  double flow = flowrate.back();

  double thermal = 4.7369E-9*pow(tmp,3) - 1.08337E-5*pow(tmp,2)+0.008069*tmp-0.9726;
  double temporal = 0.2903 * log(rep_time*3600) - 1.696;
  double rate = 0.12435 * log(flow) + 0.7985;
  double volox_eff = thermal * temporal * rate;
  return volox_eff;
}

double Volox::Throughput(std::vector<double> flowrate, 
  std::vector<double> reprocess_time, double volume) {
  // placeholder calculation
  double volox_through = volume / flowrate.back()*reprocess_time.back();
  return volox_through;
}
}
