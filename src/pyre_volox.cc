#include "pyre.h"
#include "process.h"
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
          set_coeff();
          temp(new_temp);
          Rtime(new_Rtime);
          flowrate(new_flowrate);
          volume(new_volume); 
}

void Volox::set_coeff() {
  th0 = 4.369E-9;
  th1 = -1.0833E-5;
  th2 = 0.008069;
  th3 = -0.9726;
  t0 = 0.2903;
  t1 = -1.696;
  r0 = 0.12435;
  r1 = 0.7985;
}

double Volox::Efficiency() {
  return Thermal(th0,th1,th2,th3)*Temporal(t0,t1)*RateEff(r0,r1);
}

double Volox::Thermal(double c0 = 4.369E-9,
                      double c1 = -1.0833E-5, 
                      double c2 = 0.008069,
                      double c3 = -0.9726
) {
  return c0*pow(temp(), 3) + c1*pow(temp(),2) + c2*temp() + c3;
}

double Volox::Temporal(double c0 = 0.2903,
                       double c1 = -1.696
) {
  return c0 * log(Rtime()*3600) + c1;
}

double Volox::RateEff(double c0 = 0.12435,
                      double c1 = 0.7985
) {
  return c0*log(flowrate()) + c1;
}

double Volox::Throughput() {
  return volume() / flowrate()*Rtime();
}
}
