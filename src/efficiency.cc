#include "efficiency.h"

using namespace std;

double Efficiency::VoloxEff(temperature, reprocess_time, flowrate) {
	thermal = (8.8333 * 10^(-7)*temperature^3 - 0.001755*temperature^2+1.166*temperature-159.6) / 100;
	temporal = 0.2903 * ln(reprocess_time*3600) - 1.696;
	rate = 0.12435 * ln(flowrate) + 0.7985;
	volox_eff = thermal * temporal * rate;
	return volox_eff;
}

double Efficiency::ReductEff(current, lithium_oxide) {
	coulombic_eff = -0.00685*current^4 + 0.20413*current^3 - 2.273*current^2 + 11.2046*current - 19.7493;
	catalyst_eff = 0.075 * lithium_oxide + 0.775;
	reduct_eff = coulombic_eff * catalyst_eff;
	return reduct_eff;
}

double Efficiency::RefineEff(temperature, pressure, rotation) {
	thermal = (8.8333 * 10^(-7)*temperature^3 - 0.00255*temperature^2+2.4572*temperature-691.1) / 100;
	pres_eff = -0.0055128 * pressure + 100.5;
	if (rotation =< 1) {
		agitation = 0.032*rotation + 0.72;
	} else {
		agitation = 0.0369924675*ln(rotation)+0.829777331;
		if (agitation > 1) {
			agitation = 1;
		}
	}
	refine_eff = thermal * pres_eff * agitation;
	return refine_eff;
}

double Efficiency::WinningEff(current, reprocess_time, flowrate) {
	coulombic_eff = -0.00685*current^4 + 0.20413*current^3 - 2.273*current^2 + 11.2046*current - 19.7493;
	temporal = 0.2903 * ln(reprocess_time*3600) - 1.696;
	rate = 0.12435 * ln(flowrate) + 0.7985;
	winning_eff = eff1 * coulombic_eff * thermal * rate;
	return winning_eff;
}
