#include "pyre_reduction.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::CompMap;

namespace pyro {

Reduct::Reduct() {
  SetCoeff();
  current(5);
  lithium(2);
  volume(10);
  Rtime(1);
}

Reduct::Reduct(double new_current, 
               double new_lithium, 
               double new_volume, 
               double new_Rtime
            ) 
            {
              SetCoeff();
              current(new_current);
              lithium(new_lithium);
              volume(new_volume);
              Rtime(new_Rtime);
}

void Reduct::SetCoeff() {
  coul.push_back(-0.00685);
  coul.push_back(0.20413);
  coul.push_back(-2.273);
  coul.push_back(11.2046);
  coul.push_back(-19.7493);
  lith.push_back(0.075);
  lith.push_back(0.775);
}

double Reduct::Efficiency() { 
  return Coulombic() * Catalyst();
}

double Reduct::Coulombic() {
  return coul[0]*pow(current(), 4) + coul[1]*pow(current(), 3) + coul[2]*pow(current(), 2)
          + coul[3]*current() + coul[4];
}

double Reduct::Catalyst() {
  return lith[0]*lithium() + lith[1];
}

double Reduct::Throughput() {
  return volume() / Rtime();
};
} // namespace pyro
