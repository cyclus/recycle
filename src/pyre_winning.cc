#include "pyre.h"
#include "process.h"
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

Winning::Winning(double new_current = 8, 
                 double new_time = 1, 
                 double new_flowrate = 3, 
                 double new_volume = 1
            ) 
            {
  set_coeff();
  current(new_current);
  Rtime(new_time);
  flowrate(new_flowrate);
  volume(new_volume);
}

void Winning::set_coeff() {
  c0 = -0.00685;
  c1 = 0.20413;
  c2 = -2.273;
  c3 = 11.2046;
  c4 = -19.7493;
  t0 = 0.2903;
  t1 = -1.696;
  r0 = 0.12435;
  r1 = 0.7985;
}

double Winning::Efficiency() {
  return Coulombic(c0,c1,c2,c3,c4) * Temporal(t0,t1) * RateEff(r0,r1);
}

double Winning::Coulombic(double c0 = -0.00685,
                          double c1 = 0.20413,
                          double c2 = -2.273,
                          double c3 = 11.2046,
                          double c4 = -19.7493
) {
  return c0*pow(current(), 4) + c1*pow(current(), 3) + c2*pow(current(), 2)
          + c3*current() + c4;
}

double Winning::Temporal(double c0 = 0.2903,
                         double c1 = -1.696
) {
  return c0 * log(Rtime()*3600) + c1;
}

double Winning::RateEff(double c0 = 0.12435,
                        double c1 = 0.7985
) {
  return c0*log(flowrate()) + c1;
}

double Winning::Throughput() {
  return volume() / Rtime();
};
}
