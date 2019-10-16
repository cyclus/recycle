#include "pyre_reduction.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::CompMap;

namespace pyro {

Reduct::Reduct() {
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
              current(new_current);
              lithium(new_lithium);
              volume(new_volume);
              Rtime(new_Rtime);
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
