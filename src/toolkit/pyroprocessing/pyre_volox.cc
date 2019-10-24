#include "pyre_volox.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::CompMap;

namespace pyro {

Volox::Volox() {
  temp(0);
  Rtime(0);
  flowrate(0);
  volume(0);
}

Volox::Volox(double new_temp, 
             double new_Rtime, 
             double new_flowrate, 
             double new_volume
        ) 
        {
          temp(new_temp);
          Rtime(new_Rtime);
          flowrate(new_flowrate);
          volume(new_volume); 
}

double Volox::Efficiency() {
  return Thermal()*Temporal()*RateEff();
}

double Volox::Thermal() {
  return th[0]*pow(temp(), 3) + th[1]*pow(temp(),2) + th[2]*temp() + th[3];
}

double Volox::Temporal() {
  return ti[0] * log(Rtime()*3600) + ti[1];
}

double Volox::RateEff() {
  return r[0]*log(flowrate()) + r[1];
}

double Volox::Throughput() {
  return volume() / flowrate()*Rtime();
}
} // namespace pyro
