#include "pyre.h"
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

Reduct::Reduct(double current = 5, 
               double lithium = 2, 
               double volume = 10, 
               double Rtime = 1
            ) 
            {
              current(current);
              lithium(lithium);
              volume(volume);
              Rtime(Rtime);
}

double Reduct::Efficiency() {
  return coulombic_eff * catalyst_eff;
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

