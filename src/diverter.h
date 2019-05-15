#ifndef RECYCLE_SRC_DIVERTER_H_
#define RECYCLE_SRC_DIVERTER_H_

#include "cyclus.h"
#include "pyre.h"
#include "process.h"

namespace recycle {

class Diverter {

public:

std::pair<std::string, std::string> location;
int frequency;
double siphon;

Diverter();

Diverter(cyclus::Context* ctx, int frequency,
    double siphon);

void location(std::pair<std::string,std::string> location_);
std::pair<std::string,std::string> location();

void frequency(int frequency_);
int frequency();

void siphon(double siphon_);
double siphon();

bool Divert(cyclus::Context* ctx, std::map<std::string, Process*>);

Material::Ptr DivertStream(std::map<std::string, Material::Ptr> sepstreams);

}
}
