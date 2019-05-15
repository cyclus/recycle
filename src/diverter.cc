#include "cyclus.h"
#include "pyre.h"
#include "process.h"
#include "diverter.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::CompMap;

namespace recycle {

Diverter::Diverter() {}

Diverter::Diverter(cyclus::Context* ctx, std::pair<std::string,std::string> location, 
    int frequency = 1E299, double quantity = 0.01, int divert_number = 1, std::string type_ = "operator") {
    tc = ctx;
    locate(location);
    freq(frequency);
    siphon(quantity);
    divert_num(divert_number);
    divert_time(0);
    type = type_;
}

void Diverter::locate(std::pair<std::string,std::string> new_local) {
    location = new_local;
}

std::pair<std::string,std::string> Diverter::locate() {
    return location;
}

void Diverter::freq(int new_freq) {
    frequency = new_freq;
}

int Diverter::freq() {
    return frequency;
}

void Diverter::siphon(double new_siphon) {
    quantity = new_siphon;
}

double Diverter::siphon() {
    return quantity;
}

void Diverter::divert_num(int new_num) {
    divert_number = new_num;
}

int Diverter::divert_num() {
    return divert_number;
}

void Diverter::divert_time(int new_time) {
    divert_times = new_time;
}

int Diverter::divert_time() {
    return divert_times;
}

bool Diverter::Divert(std::map<std::string, Process> components) {
    if (tc->time() % freq() == 0) {
        if (divert_time() < divert_num()) {
            divert_time(divert_time()+1);
            Process x = components[locate().first];
            x.DivertMat(type, locate(), siphon());
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

Material::Ptr Diverter::DivertStream(std::map<std::string, Material::Ptr> sepstreams) {
    Material::Ptr m = sepstreams[locate().first];
    return m->ExtractComp(m->quantity() * siphon(), m->comp());
} 
}
