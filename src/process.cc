#include "process.h"
#include <vector>

namespace recycle {

Process::Process() {}

void Process::set_temp(double input) {
    temp.push_back(input);
}

void Process::set_current(double input){
    current.push_back(input);
}

void Process::set_rotation(double input){
    rotation.push_back(input);
}

void Process::set_pressure(double input){
    pressure.push_back(input);
}

void Process::set_lithium(double input){
    lithium.push_back(input);
}

void Process::set_time(double input){
    reprocess_time.push_back(input);
}

void Process::set_flowrate(double input){
    flowrate.push_back(input);
}

void Process::set_size(double input){
    b_size.push_back(input);
}

double Process::get_temp() {
    return temp.back();
}

double Process::get_current() {
    return current.back();
}

double Process::get_rotation() {
    return rotation.back();
}

double Process::get_pressure() {
    return pressure.back();
}

double Process::get_lithium() {
    return lithium.back();
}

double Process::get_time() {
    return reprocess_time.back();
}

double Process::get_flowrate() {
    return flowrate.back();
}

double Process::get_size() {
    return b_size.back();
};
}
