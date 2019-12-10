#include "pyre_winning.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::CompMap;

namespace pyro {

Winning::Winning() {
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
  current(new_current);
  Rtime(new_time);
  flowrate(new_flowrate);
  volume(new_volume);
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
