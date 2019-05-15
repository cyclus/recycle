#include "pyre.h"
#include "process.h"
#include "pyre_refining.h"
#include "boost/bind.hpp"
#include <boost/math/tools/roots.hpp>

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

using boost::math::tools::bisect;

namespace recycle {

Refine::Refine() {
  temp(0);
  pressure(0);
  rotation(0);
  b_size(0);
  Rtime(0);
}

Refine::Refine(double new_temp = 900, 
               double new_press = 760, 
               double new_rotation = 0, 
               double new_batch_size = 20,
               double new_rtime = 1
            ) 
            {
  set_coeff();
  temp(new_temp);
  pressure(new_press);
  rotation(new_rotation);
  b_size(new_batch_size);
  Rtime(new_rtime);
}

void Refine::set_coeff() {
  t0 = 4.7369E-9;
  t1 = -1.08337E-5;
  t2 = 0.008069;
  t3 = -0.9726;
  p0 = -7.17631E-10;
  p1 = 4.04545E-07;
  p2 = -8.06336E-05;
  p3 = 1.002;
  a0 = 0.032;
  a1 = 0.72;
  a2 = 0.0338396;
  a3 = 0.83667;
}

struct Refine::TerminationCondition {
    bool operator() (double min, double max)  {
        return abs(min - max) <= 0.000001;
    }
};

void Refine::DivertMat(std::string type, std::pair<std::string, std::string> location,
  double siphon) {
    if (type == "Operator") {
      OpDivertMat(location, siphon);
    } 
}

void Refine::OpDivertMat(std::pair<std::string, std::string> location, double siphon) {
  std::string subcomponent = location.second;
  if (subcomponent == "temp"){
    double new_eff = Efficiency() * (1+siphon);
    ThermalFunc = boost::bind(&recycle::Refine::Thermal,this,_1,new_eff,
      4.7369E-9,-1.08337E-5,0.008069,-0.9726);
    std::pair<double, double> result = bisect(ThermalFunc, 400, 1200, TerminationCondition());
    double root = (result.first + result.second) / 2;
    temp(root);
  } else if (subcomponent == "pressure") {
  } else if (subcomponent == "rotation") {
  } else if (subcomponent == "batch size") {
  } else {
    //time
  }
}

double Refine::Efficiency() {
  return Thermal(temp(),0,t0,t1,t2,t3)*PressureEff(p0,p1,p2,p3)*Agitation(a0,a1,a2,a3);
}

double Refine::Thermal(double tmp,
                       double newEff = 0,
                       double c0 = 4.7369E-9,
                       double c1 = -1.08337E-5,
                       double c2 = 0.008069,
                       double c3 = -0.9726
) {
  return c0*pow(tmp,3) + c1*pow(tmp,2) + c2*tmp + c3 - newEff;
}

double Refine::PressureEff(double c0 = -7.17631E-10,
                           double c1 = 4.04545E-07,
                           double c2 = -8.06336E-05,
                           double c3 = 1.002
) {
  return c0*pow(pressure(), 3) + c1*pow(pressure(), 2) + c2*pressure() + c3;
}

double Refine::Agitation(double c0 = 0.032,
                         double c1 = 0.72,
                         double c2 = 0.0338396,
                         double c3 = 0.83667
) {
  if (rotation() <= 1) {
    double agi = c0*rotation() + c1;
  } else {
    double agi = c2*log(rotation()) + c3;
    if (agi > 1) {
      throw ValueError("Rotation efficiency cannot exceed 1");
    }
  }
  return agi;
}

double Refine::Throughput() {
  return b_size() / Rtime();
};
}
