#include "pyre.h"
#include "process.h"
#include "diverter.h"

namespace recycle {

Diverter::Diverter() {

}

Diverter::Diverter(cyclus::Context* ctx, std::pair<std::string,std::string> location_, 
    int frequency_, double siphon) {
    location(location_);
    frequency(frequency_);
    siphon(siphon_);
}

void location(std::pair<std::string,std::string> location_) {
    location = location_;
}

std::pair<std::string,std::string> location() {
    return location;
}

void frequency(int frequency_) {
    frequency = frequency_;
}

int frequency() {
    return frequency;
}

void siphon(double siphon_) {
    siphon = siphon_;
}

double siphon() {
    return siphon;
}

void Diverter::Divert(cyclus::Context* ctx, std::map<std::string, Process*>) {
    if (ctx->time() % frequency == 0) {
        Process* x = components[location->first];
        x.DivertMat(location(), siphon());
    }
}
}
