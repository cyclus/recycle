#ifndef _EFFICIENCY_H
#define _EFFICIENCY_H

#include "pyre.h"


class Efficiency {


public:

	double thermal;
	double temporal;
	double coulombic_eff;
	double rate;
	double catalyst_eff;
	double pres_eff;
	double agitation;
	double volox_eff;
	double reduct_eff;
	double refine_eff;
	double winning_eff;
	double temperature;
	double reprocess_time;
	double flowrate;
	double current;
	double lithium_oxide;
	double pressure;
	double rotation;
	
	Efficiency();
	Efficiency(double volox_temp, double volox_time, double volox_flowrate, double reduct_current, 
		double reduct_li2o, double refine_temp, double refine_press, double refine_rotation, 
		double winning_current, double winning_time, double winning_flowrate);

	/// @return efficiency total efficiency throughout voloxidation
	static double VoloxEff(double volox_temp, double volox_time, double volox_flowrate);

	/// @return efficiency total efficiency throughout electroreduction
	static double ReductEff(double reduct_current, double reduct_lithium_oxide);

	/// @return efficiency total efficiency throughout electrorefining
	static double RefineEff(double refine_temp, double refine_press, double refine_rotation);

	/// @return efficiency total efficiency throughout electrowinning
	static double WinningEff(double winning_current, double winning_time, double winning_flowrate);
};



#endif //
