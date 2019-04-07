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

Winning::Winning(double winning_current = 8, 
                 double winning_time = 1, 
                 double winning_flowrate = 3, 
                 double winning_volume = 1
            ) 
            {
  current.push(winning_current);
  reprocess_time.push(winning_time);
  flowrate.push(winning_flowrate);
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
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
}

double Winning::Efficiency(std::vector<double> current, 
  std::vector<double> reprocess_time, std::vector<double> flowrate) {
  double curr = current.back();
  double rep_time = reprocess_time.back();
  double flow = flowrate.back();

  double coulombic_eff = -0.00685*pow(curr,4) + 0.20413*pow(curr,3) 
                         - 2.273*pow(curr,2) + 11.2046*curr - 19.7493;
  double temporal = 0.2903 * log(rep_time*3600) - 1.696;
  double rate = 0.12435 * log(flow) + 0.7985;
  double winning_eff = coulombic_eff * temporal * rate;
  return winning_eff;
}

double Winning::Throughput(std::vector<double> reprocess_time, double volume) {
  double winning_through = volume / reprocess_time.back();
  return winning_through;
};
}
