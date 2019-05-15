#include "cyclus.h"
#include "pyre.h"
#include "process.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

namespace recycle {

Process::Process() {}

// KDH make sure the docs state the units.
void Process::temp(double new_temp) {
    subcomponents["temp"].push_back(new_temp);
}

void Process::current(double new_current){
    subcomponents["current"].push_back(new_current);
}

void Process::rotation(double new_rotate){
    subcomponents["rotation"].push_back(new_rotate);
}

void Process::pressure(double new_press){
    subcomponents["pressure"].push_back(new_press);
}

void Process::lithium(double new_lithium){
    subcomponents["lithium"].push_back(new_lithium);
}

void Process::Rtime(double new_time){
    subcomponents["reprocess time"].push_back(new_time);
}

void Process::flowrate(double new_flow){
    subcomponents["flowrate"].push_back(new_flow);
}

void Process::b_size(double new_size){
    subcomponents["batch size"].push_back(new_size);
}

void Process::volume(double new_volume) {
    subcomponents["volume"].push_back(new_volume);
}

double Process::temp() {
    return subcomponents["temp"].back();
}

double Process::current() {
    return subcomponents["current"].back();
}

double Process::rotation() {
    return subcomponents["rotation"].back();
}

double Process::pressure() {
    return subcomponents["pressure"].back();
}

double Process::lithium() {
    return subcomponents["lithium"].back();
}

double Process::Rtime() {
    return subcomponents["reprocess time"].back();
}

double Process::flowrate() {
    return subcomponents["flowrate"].back();
}

double Process::b_size() {
    return subcomponents["batch size"].back();
}

double Process::volume() {
    return subcomponents["volume"].back();
}

Material::Ptr Process::SepMaterial(std::map<int, double> effs, Material::Ptr mat) {
  CompMap cm = mat->comp()->mass();
  cyclus::compmath::Normalize(&cm, mat->quantity());
  double tot_qty = 0;
  CompMap sepcomp;

  CompMap::iterator it;
  for (it = cm.begin(); it != cm.end(); ++it) {
    int nuc = it->first;
    int elem = (nuc / 10000000) * 10000000;
    double eff = 0;
    if (effs.count(nuc) > 0) {
      eff = effs[nuc];
    } else if (effs.count(elem) > 0) {
      eff = effs[elem];
    } else {
      continue;
    }

    double qty = it->second;
    double sepqty = qty * eff * Efficiency();
    sepcomp[nuc] = sepqty;
    tot_qty += sepqty;
  }

  Composition::Ptr c = Composition::CreateFromMass(sepcomp);
  return Material::CreateUntracked(tot_qty, c);
}

void Process::DivertMat(std::string type, std::pair<std::string, std::string> location,
  double siphon) {};

double Process::Efficiency() {
    return 1;
};
}
