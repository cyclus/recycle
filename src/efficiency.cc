#include "efficiency.h"

using namespace std;
// a and b are placeholders (will not build - these will be numbers when finalized)
Efficiency::VoloxEff(temperature, reprocess_time, flowrate) {
	thermal = a * temperature + b;
	temporal = a * reprocess_time + b;
	rate = a * flowrate + b;
	volox_eff = thermal * temporal * rate;
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

Efficiency::WinningEff(porosity, currnet, reprocess_time, flowrate) {
	eff1 = a * porosity + b;
	eff2 = a * current + b;
	eff3 = a * time + b;
	eff4 = a * flowrate + b;
	winning_eff = eff1 * eff2 * eff3 * eff4;
}
