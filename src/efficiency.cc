#include "efficiency.h"

using namespace std;
// a and b are placeholders (will not build - these will be numbers when finalized)
double Efficiency::VoloxEff(temperature, reprocess_time, flowrate) {
	thermal = (8.8333 * 10^(-7)*temperature^3 - 0.001755*temperature^2+1.166*temperature-159.6) / 100;
	temporal = 0.2903 * ln(reprocess_time*3600) - 1.696;
	rate = 0.12435 * ln(flowrate) + 0.7985;
	volox_eff = thermal * temporal * rate;
	return volox_eff;
}

double Efficiency::ReductEff(porosity, current, batch_size, li2o) {
	eff1 = a * porosity + b;
	eff2 = -0.00685*current^4 + 0.20413*current^3 - 2.273*current^2 + 11.2046*current - 19.7493;
	eff3 = a * batch_size + b;
	eff4 = 0.075 * li2o + 0.775;
	reduct_eff = eff1 * eff2 * eff3 * eff4;
	return reduct_eff;
}

double Efficiency::RefineEff(temperature, pressure, rotation, volume) {
	eff1 = (8.8333 * 10^(-7)*temperature^3 - 0.00255*temperature^2+2.4572*temperature-691.1) / 100;
	eff2 = -0.0055128 * pressure + 100.5;
	eff3 = a * rotation + b;
	eff4 = a * volume + b;
	refine_eff = eff1 * eff2 * eff3 * eff4;
	return refine_eff;
}

double Efficiency::WinningEff(porosity, current, reprocess_time, flowrate) {
	eff1 = a * porosity + b;
	eff2 = -0.1477 * ln(current) + 0.6767;
	temporal = 0.2903 * ln(reprocess_time*3600) - 1.696;
	eff4 = 0.12435 * ln(flowrate) + 0.7985;
	winning_eff = eff1 * eff2 * eff3 * eff4;
	return winning_eff;
}
