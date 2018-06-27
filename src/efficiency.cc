#include "efficiency.h"

using namespace std;

Efficiency::VoloxEff(temperature, time, flowrate) {
	eff1 = a * temperature + b;
	eff2 = a * time + b;
	eff3 = a * flowrate + b;
	volox_eff = eff1 * eff2 * eff3;
}

Efficiency::ReductEff(porosity, current, batch_size, li2o) {
	eff1 = a * porosity + b;
	eff2 = a * current + b;
	eff3 = a * batch_size + b;
	eff4 = a * li2o + b;
	reduct_eff = eff1 * eff2 * eff3 * eff4;
}

Efficiency::RefineEff(temperature, pressure, rotation, volume) {
	eff1 = a * temperature + b;
	eff2 = a * pressure + b;
	eff3 = a * rotation + b;
	eff4 = a * volume + b;
	refine_eff = eff1 * eff2 * eff3 * eff4;
}

Efficiency::WinningEff(porosity, currnet, time, flowrate) {
	eff1 = a * porosity + b;
	eff2 = a * current + b;
	eff3 = a * time + b;
	eff4 = a * flowrate + b;
	winning_eff = eff1 * eff2 * eff3 * eff4;
}
