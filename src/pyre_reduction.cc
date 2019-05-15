#include "pyre.h"
#include "process.h"
#include "pyre_reduction.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

namespace recycle {

Reduct::Reduct() {}

Reduct::Reduct(double new_current = 5, 
               double new_lithium = 2, 
               double new_volume = 10, 
               double new_Rtime = 1
            ) 
            {
              set_coeff();
              current(new_current);
              lithium(new_lithium);
              volume(new_volume);
              Rtime(new_Rtime);
}

void Reduct::set_coeff() {
  c0 = -0.00685;
  c1 = 0.20413;
  c2 = -2.273;
  c3 = 11.2046;
  c4 = 19.7493;
  ca0 = 0.075;
  ca1 = 0.775;
}

double Reduct::Efficiency() {
  return Coulombic(c0,c1,c2,c3,c4) * Catalyst(ca0,ca1);
}

double Reduct::Coulombic(double c0 = -0.00685,
                         double c1 = 0.20413,
                         double c2 = -2.273,
                         double c3 = 11.2046,
                         double c4 = 19.7493
) {
  return c0*pow(current(), 4) + c1*pow(current(), 3) + c2*pow(current(), 2)
          + c3*current() + c4;
}

double Reduct::Catalyst(double c0 = 0.075,
                        double c1 = 0.775
) {
  return c0*lithium() + c1;
}

double Reduct::Throughput() {
  return volume() / Rtime();
};
}

