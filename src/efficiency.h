#ifndef _EFFICIENCY_H
#define _EFFICIENCY_H


class Efficiency {

public:
/// @return efficiency total efficiency throughout voloxidation
double Efficiency::VoloxEff(temperature, reprocess_time, flowrate)

/// @return efficiency total efficiency throughout electroreduction
double Efficiency::ReductEff(porosity, current, batch_size, li2o)

/// @return efficiency total efficiency throughout electrorefining
double Efficiency::RefineEff(temperature, pressure, rotation, volume)

/// @return efficiency total efficiency throughout electrowinning
double Efficiency::WinningEff(porosity, current, reprocess_time, flowrate)
};
