#include "pyre.h"
#include "cyclus.h"
#include "process.h"
#include "diverter.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::CompMap;

namespace recycle {

Diverter::Diverter() {

}

Diverter::Diverter(cyclus::Context* ctx, std::pair<std::string,std::string> location_, 
    int frequency_, double siphon_, int divert_num_) {
    location(location_);
    frequency(frequency_);
    siphon(siphon_);
    divert_num(divert_num_);
    divert_time(0);
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

void divert_num(int divert_num_) {
    divert_num = divert_num_;
}

int divert_num() {
    return divert_num;
}

void divert_time(int divert_time_) {
    divert_time = divert_time_;
}

int divert_time() {
    return divert_time;
}

bool Diverter::Divert(cyclus::Context* ctx, std::map<std::string, Process*>) {
    if (ctx->time() % frequency == 0) {
        if (divert_time() < divert_num()) {
            divert_time(divert_time()+1);
            Process* x = components[location()->first];
            x.DivertMat(location(), siphon());
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

Material::Ptr Diverter::DivertStream(std::map<std::string, Material::Ptr> sepstreams) {
    Material::Ptr m = sepstreams[location()->first];
    return m->ExtractComp(m->quantity() * siphon(), m->comp());
} 
}
