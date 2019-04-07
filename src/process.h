#ifndef RECYCLE_SRC_PROCESS_H_
#define RECYCLE_SRC_PROCESS_H_

#include "pyre.h"
#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

class Process {

public:

std::vector<double> temp;
std::vector<double> current;
std::vector<double> rotation;
std::vector<double> pressure;
std::vector<double> lithium;
std::vector<double> reprocess_time;
std::vector<double> flowrate;
std::vector<double> size;
double volume;

Process();

protected:

void set_temp(double input);

void set_current(double input);

void set_rotation(double input);

void set_pressure(double input);

void set_lithium(double input);

void set_time(double input);

void set_flowrate(double input);

void set_size(double input);
};
}
#endif // RECYCLE_SRC_PROCESS_H_
