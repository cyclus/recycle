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

Volox::Volox() {
  SetCoeff();
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
          SetCoeff();
          temp(new_temp);
          Rtime(new_Rtime);
          flowrate(new_flowrate);
          volume(new_volume); 
}

void Volox::SetCoeff() {
  th.push_back(4.369E-9);
  th.push_back(-1.0833E-5);
  th.push_back(0.008069);
  th.push_back(-0.9726);
  ti.push_back(0.2903);
  ti.push_back(-1.696);
  r.push_back(0.12435);
  r.push_back(0.7985);
}

double Volox::Efficiency() {
  return Thermal(th[0],th[1],th[2],th[3])*Temporal(ti[0],ti[1])*RateEff(r[0],r[1]);
}

double Volox::Thermal(double c0,
                      double c1, 
                      double c2,
                      double c3
) {
  return c0*pow(temp(), 3) + c1*pow(temp(),2) + c2*temp() + c3;
}

double Volox::Temporal(double c0,
                       double c1
) {
  return c0 * log(Rtime()*3600) + c1;
}

double Volox::RateEff(double c0,
                      double c1
) {
  return c0*log(flowrate()) + c1;
}

double Volox::Throughput() {
  return volume() / flowrate()*Rtime();
}
} // namespace recycle
