#include "pyre.h"
#include "pyre_refining.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

namespace recycle {

Refine::Refine() {
  temp(0);
  pressure(0);
  rotation(0);
  b_size(0);
  Rtime(0);
}

Refine::Refine(double temp = 900, 
               double press = 760, 
               double rotation = 0, 
               double batch_size = 20,
               double r_time = 1
            ) 
            {
  temp(refine_temp);
  pressure(refine_press);
  rotation(refine_rotation);
  b_size(refine_batch_size);
  Rtime(r_time);
}

void Refine::DivertMat(std::string type, std::pair<std::string, std::string> location,
  double siphon) {
    if (type == "Operator") {
      OpDivertMat(location, siphon);
    } else {
      NefDivertMat(siphon);
    }
}

void Refine::NefDivertMat(double siphon) {

}

void Refine::OpDivertMat(std::pair<std::string, std::string> location, double siphon) {
  std::string subcomponent = location->second;
  if (subcomponent == "temp"){
    double newEff = Efficiency() * siphon;
    ThermalFunc = boost::bind(recycle::Refine::Thermal,this,_1,newEff,4.7369E-9,-1.08337E-5,0.008069,-0.9726);
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
  return Thermal()*PressureEff()*Agitation();
}

double Refine::Thermal(double temp = temp(),
                       double newEff = 0,
                       double c0 = 4.7369E-9,
                       double c1 = -1.08337E-5,
                       double c2 = 0.008069,
                       double c3 = -0.9726
) {
  return c0*pow(temp,3) + c1*pow(temp,2) + c2*temp + c3 - newEff;
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
    double agitation = c0*rotation() + c1;
  } else {
    double agitation = c2*log(rotation()) + c3;
    if (agitation > 1) {
      throw ValueError("Rotation efficiency cannot exceed 1");
    }
  }
  return agitation;
}

double Refine::Throughput() {
  return b_size() / Rtime();
};
}
