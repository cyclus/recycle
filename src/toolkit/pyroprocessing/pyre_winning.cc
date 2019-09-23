#include "pyre_winning.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::CompMap;

namespace pyro {

Winning::Winning() {
  SetCoeff();
  current(8);
  Rtime(1);
  flowrate(3);
  volume(1);
}

Winning::Winning(double new_current, 
                 double new_time, 
                 double new_flowrate, 
                 double new_volume
            ) 
            {
  SetCoeff();
  current(new_current);
  Rtime(new_time);
  flowrate(new_flowrate);
  volume(new_volume);
}

void Winning::SetCoeff() {
  coul.push_back(-0.00685);
  coul.push_back(0.20413);
  coul.push_back(-2.273);
  coul.push_back(11.2046);
  coul.push_back(-19.7493);
  ti.push_back(0.2903);
  ti.push_back(-1.696);
  r.push_back(0.12435);
  r.push_back(0.7985);
}

double Winning::Efficiency() {
  return Coulombic() * Temporal() * RateEff();
}

double Winning::Coulombic() {
  return coul[0]*pow(current(), 4) + coul[1]*pow(current(), 3) + coul[2]*pow(current(), 2)
          + coul[3]*current() + coul[4];
}

double Winning::Temporal() {
  return ti[0] * log(Rtime()*3600) + ti[1];
}

double Winning::RateEff() {
  return r[0]*log(flowrate()) + r[1];
}

double Winning::Throughput() {
  return volume() / Rtime();
};
} // namespace pyro
