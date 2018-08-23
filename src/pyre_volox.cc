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
        ) : 
        temp( volox_temp ),
        reprocess_time( volox_time ),
        flowrate( volox_flowrate ),
        volume( volox_volume ) 
        {
          std::cout << "Volox Temp = " << temp << std::endl;
        }

// This returns an untracked material that should just be used for
// its composition and qty - not in any real inventories, etc.
Material::Ptr Volox::VoloxSepMaterial(std::map<int, double> effs, Material::Ptr mat) {
  std::cout << "Temp = " << temp << std::endl;
  CompMap cm = mat->comp()->mass();
  cyclus::compmath::Normalize(&cm, mat->quantity());
  double tot_qty = 0;
  CompMap sepcomp;

  CompMap::iterator it;
  for (it = cm.begin(); it != cm.end(); ++it) {
    int nuc = it->first;
    int elem = (nuc / 10000000) * 10000000;
    double eff = 0;
    std::cout << nuc << std::endl;
    if (effs.count(nuc) > 0) {
      eff = effs[nuc];
    } else if (effs.count(elem) > 0) {
      eff = effs[elem];
    } else {
      continue;
    }
    double qty = it->second;
    std::cout << "hi" << std::endl;
    double sepqty = qty * eff * Efficiency(temp, reprocess_time, flowrate);
    std::cout << "hi2" << std::endl;
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }
  std::cout << "CreateFromMass" << std::endl;
  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
}

double Volox::Efficiency(double temp, double reprocess_time, double flowrate) {
  double thermal = 4.7369E-9*pow(temp,3) - 1.08337E-5*pow(temp,2)+0.008069*temp-0.9726;
  double temporal = 0.2903 * log(reprocess_time*3600) - 1.696;
  double rate = 0.12435 * log(flowrate) + 0.7985;
  double volox_eff = thermal * temporal * rate;
  std::cout << "Thermal = " << temp << std::endl;
  std::cout << "Temporal = " << reprocess_time << std::endl;
  std::cout << "rate = " << flowrate << std::endl;
  return volox_eff;
}

double Volox::Throughput(double flowrate, double reprocess_time, double volume) {
  // placeholder calculation
  double volox_through = volume / flowrate*reprocess_time;
  return volox_through;
}
}
