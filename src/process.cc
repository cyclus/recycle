#include "pyre.h"
#include "process.h"

namespace recycle {

Process::Process() {
    set_temp(0);
    set_current(0);
    set_rotation(0);
    set_pressure(0);
    set_lithium(0);
    set_time(0);
    set_size(0);
}

void set_temp(double input) {
    temp.push(input);
}

void set_current(double input){
    current.push(input);
}

void set_rotation(double input){
    rotation.push(input);
}

void set_pressure(double input){
    pressure.push(input);
}

void set_lithium(double input){
    lithium.push(input);
}

void set_time(double input){
    reprocess_time.push(input);
}

void set_flowrate(double input){
    flowrate.push(input);
}

void set_size(double input){
    size.push(input);
}

double get_temp() {
    return temp.back();
}

double get_current() {
    return current.back();
}

double get_rotation() {
    return rotation.back();
}

double get_pressure() {
    return pressure.back();
}

double get_lithium() {
    return lithium.back();
}

double get_time() {
    return reprocess_time.back();
}

double get_flowrate() {
    return flowrate.back();
}

double get_size() {
    return size.back();
}
}
