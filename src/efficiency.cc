#include "pyre.h"
#include "efficiency.h"
#include <cmath>
#include <stdio.h>
#include <math.h>

Efficiency::Efficiency() {
	double temperature = temperature; //default temperature
	double reprocess_time = 1; //default of 1 hour
	double flowrate = 0.4;
	double current = 1; //default of 1 Amp
	double lithium_oxide = 1; //1 wt% lithium oxide in reduction
	double pressure = 300;
	double rotation = 0;
}

static double Efficiency::VoloxEff(double temperature, double reprocess_time, double flowrate) {
	double thermal = (8.8333E-7*pow(temperature,3) - 0.001755*(temperature,2)+1.166*temperature-159.6) / 100;
	double temporal = 0.2903 * log(reprocess_time*3600) - 1.696;
	double rate = 0.12435 * log(flowrate) + 0.7985;
	double volox_eff = thermal * temporal * rate;
	return volox_eff;
}

static double Efficiency::ReductEff(double current, double lithium_oxide) {
	double coulombic_eff = -0.00685*pow(current,4) + 0.20413*pow(current,3) - 2.273*pow(current,2) + 11.2046*current - 19.7493;
	double catalyst_eff = 0.075 * lithium_oxide + 0.775;
	double reduct_eff = coulombic_eff * catalyst_eff;
	return reduct_eff;
}

static double Efficiency::RefineEff(double temperature, double pressure, double rotation) {
	thermal = (8.8333E-7*pow(temperature,3) - 0.00255*(temperature,2)+2.4572*temperature-691.1) / 100;
	pres_eff = -0.0055128 * pressure + 100.5;
	if (rotation <= 1) {
		agitation = 0.032*rotation + 0.72;
	} else {
		agitation = 0.0369924675*log(rotation)+0.829777331;
		if (agitation > 1) {
			agitation = 1;
		}
	}
	refine_eff = thermal * pres_eff * agitation;
	return refine_eff;
}

static double Efficiency::WinningEff(double current, double reprocess_time, double flowrate) {
	coulombic_eff = -0.00685*pow(current,4) + 0.20413*pow(current,3) - 2.273*pow(current,2) + 11.2046*current - 19.7493;
	temporal = 0.2903 * log(reprocess_time*3600) - 1.696;
	rate = 0.12435 * log(flowrate) + 0.7985;
	winning_eff = coulombic_eff * thermal * rate;
	return winning_eff;
}
