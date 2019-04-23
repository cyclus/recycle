#ifndef RECYCLE_SRC_DIVERTER_H_
#define RECYCLE_SRC_DIVERTER_H_

#include "cyclus.h"

namespace recycle {

class Diverter {

public:

Diverter();

Diverter(cyclus::Context* ctx, std::vector<Process*> components, int frequency,
    double siphon);

Divert(cyclus::Context* ctx, int frequency);

}

}