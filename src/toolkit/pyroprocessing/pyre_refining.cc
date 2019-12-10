#include "pyre_refining.h"
#include "boost/bind.hpp"
#include <boost/math/tools/roots.hpp>


using cyclus::Material;
using cyclus::Composition;
using cyclus::CompMap;
using cyclus::ValueError;

using boost::math::tools::bisect;

namespace pyro {

Refine::Refine() {
  temp(900);
  pressure(760);
  rotation(0);
  b_size(20);
  Rtime(1);
}

Refine::Refine(double new_temp, 
               double new_press, 
               double new_rotation, 
               double new_batch_size,
               double new_rtime
            ) 
            {
  temp(new_temp);
  pressure(new_press);
  rotation(new_rotation);
  b_size(new_batch_size);
  Rtime(new_rtime);
}

struct Refine::TerminationCondition {
    bool operator() (double min, double max)  {
        return abs(min - max) <= 1e-5;
    }
};

void Refine::DivertMat(std::string type, std::pair<std::string, std::string> location,
  double siphon) {
    if (type == "operator") {
      OpDivertMat(location, siphon);
    } 
}

void Refine::OpDivertMat(std::pair<std::string, std::string> location, double siphon) {
  std::string param = location.second;
  double new_val;

  double paramVal = subcomponents[param].back();
  double lower_bound = paramVal-(paramVal*0.1);
  double upper_bound = paramVal+(paramVal*0.1);
  if (param == "temp"){
    double new_eff = Thermal() * (1+siphon);
    ThermalFunc = boost::bind(&pyro::Refine::Thermal,this,_1,new_eff);
    new_val = Bisector(ThermalFunc, lower_bound, upper_bound);
  } else if (param == "pressure") {
  } else if (param == "rotation") {
  } else if (param == "batch") {
  } else {
  }
  subcomponents[param].push_back(new_val);
}

double Refine::Bisector(boost::function<double(double)> SolveFunc, double lower, double upper) {
  std::pair<double, double> result = bisect(ThermalFunc, 
      temp()-100.0, temp()+100.0, TerminationCondition());
  return (result.first + result.second) / 2;
}

double Refine::Efficiency() {
  return Thermal()*PressureEff()*Agitation();
}

double Refine::Thermal() {
  return Thermal(temp(),0);
}

double Refine::Thermal(double tmp,
                       double new_eff
) {
  return therm[0]*pow(tmp,3) + therm[1]*pow(tmp,2) + therm[2]*tmp + therm[3] - new_eff;
}

double Refine::PressureEff() {
  return PressureEff(pressure(),0);
}

double Refine::PressureEff(double prs,
                           double new_eff
) {
  return pres[0]*pow(prs, 3) + pres[1]*pow(prs, 2) + pres[2]*prs + pres[3] - new_eff;
}

double Refine::Agitation() {
  return Agitation(rotation(),0);
}

double Refine::Agitation(double rot,
                         double new_eff
) {
  double agi;
  if (rot <= 1) {
    agi = agit[0]*rot + agit[1] - new_eff;
  } else {
    agi = agit[2]*log(rot) + agit[3] - new_eff;
    if (agi > 1) {
      throw ValueError("Rotation efficiency cannot exceed 1");
    }
  }
  return agi;
}

double Refine::Throughput() {
  return b_size() / Rtime();
};
} // namespace pyro
