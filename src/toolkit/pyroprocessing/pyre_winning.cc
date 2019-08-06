#include "pyre_winning.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::CompMap;

namespace pyro {

Winning::Winning() {
  SetCoeff();
  current(0);
  Rtime(0);
  flowrate(0);
  volume(0);
}

Winning::Winning(double new_current = 8, 
                 double new_time = 1, 
                 double new_flowrate = 3, 
                 double new_volume = 1
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
  return Coulombic(coul[0],coul[1],coul[2],coul[3],coul[4]) * Temporal(ti[0],ti[1]) * RateEff(r[0],r[1]);
}

double Winning::Coulombic(double c0,
                          double c1,
                          double c2,
                          double c3,
                          double c4
) {
  return c0*pow(current(), 4) + c1*pow(current(), 3) + c2*pow(current(), 2)
          + c3*current() + c4;
}

double Winning::Temporal(double c0,
                         double c1
) {
  return c0 * log(Rtime()*3600) + c1;
}

double Winning::RateEff(double c0,
                        double c1
) {
  return c0*log(flowrate()) + c1;
}

double Winning::Throughput() {
  return volume() / Rtime();
};
} // namespace pyro
