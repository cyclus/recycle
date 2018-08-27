#include "pyre_winning.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

namespace recycle {

Winning::Winning() {}

Winning::Winning(double winning_current, double winning_time, 
                 double winning_flowrate, double winning_volume) {
  current = winning_current;
  reprocess_time = winning_time;
  flowrate = winning_flowrate;
  volume = winning_volume;
}

// Note that this returns an untracked material that should just be used for
// its composition and qty - not in any real inventories, etc.
Material::Ptr Winning::WinningSepMaterial(std::map<int, double> effs, Material::Ptr mat) {
  CompMap cm = mat->comp()->mass();
  cyclus::compmath::Normalize(&cm, mat->quantity());
  double tot_qty = 0;
  CompMap sepcomp;
  double sepeff = Efficiency(current,reprocess_time,flowrate);

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
    std::cout << "Nuc Id qty: " << sepqty << std::endl;
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  std::cout << "blah" << std::endl;
  return Material::CreateUntracked(tot_qty, c);
}

double Winning::Efficiency(double current, double reprocess_time, double flowrate) {
  double coulombic_eff = -0.00685*pow(current,4) + 0.20413*pow(current,3) 
                         - 2.273*pow(current,2) + 11.2046*current - 19.7493;
  double temporal = 0.2903 * log(reprocess_time*3600) - 1.696;
  double rate = 0.12435 * log(flowrate) + 0.7985;
  std::cout << "Thermal = " << coulombic_eff << std::endl;
  std::cout << "Temporal = " << temporal << std::endl;
  std::cout << "rate = " << rate << std::endl;
  double winning_eff = coulombic_eff * temporal * rate;
  return winning_eff;
}

double Winning::Throughput(double reprocess_time, double volume) {
  double winning_through = volume / reprocess_time;
  return winning_through;
};
}
