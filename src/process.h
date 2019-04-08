#ifndef RECYCLE_SRC_PROCESS_H_
#define RECYCLE_SRC_PROCESS_H_

#include <vector>

namespace recycle {

class Process {

public:

Process();

void set_temp(double input);

void set_current(double input);

void set_rotation(double input);

void set_pressure(double input);

void set_lithium(double input);

void set_time(double input);

void set_flowrate(double input);

void set_size(double input);

double get_temp();

double get_current();

double get_rotation();

double get_pressure();

double get_lithium();

double get_time();

double get_flowrate();

double get_size();

std::vector<double> temp;
std::vector<double> current;
std::vector<double> rotation;
std::vector<double> pressure;
std::vector<double> lithium;
std::vector<double> reprocess_time;
std::vector<double> flowrate;
std::vector<double> b_size;
double volume;

};
}
#endif // RECYCLE_SRC_PROCESS_H_
