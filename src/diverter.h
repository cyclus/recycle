#ifndef RECYCLE_SRC_DIVERTER_H_
#define RECYCLE_SRC_DIVERTER_H_

#include "cyclus.h"
#include "process.h"

namespace recycle {

class Diverter {

public:

cyclus::Context* tc;
std::pair<std::string, std::string> location;
int frequency;
int divert_number;
int divert_times;
double quantity;
std::string type;

Diverter();

Diverter(cyclus::Context* ctx, std::pair<std::string,std::string> location, 
    int frequency, double quantity, int divert_number, std::string type);

void locate(std::pair<std::string,std::string> new_local);
std::pair<std::string,std::string> locate();

void freq(int new_freq);
int freq();

void siphon(double new_siphon);
double siphon();

void divert_num(int new_num);
int divert_num();

void divert_time(int new_time);
int divert_time();

bool Divert(std::map<std::string, Process> components);

Material::Ptr DivertStream(std::map<std::string, Material::Ptr> sepstreams);

};
}
#endif // RECYCLE_SRC_DIVERTER_H_
