#ifndef _EFFICIENCY_H
#define _EFFICIENCY_H

#include "pyre.h"





class Efficiency:public Pyre {


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
	Efficiency(double temperature, double reprocess_time, double flowrate,
		double current, double lithium_oxide, double pressure, double rotation);

	/// @return efficiency total efficiency throughout voloxidation
	static double VoloxEff(double temperature, double reprocess_time, double flowrate);

	/// @return efficiency total efficiency throughout electroreduction
	static double ReductEff(double current, double lithium_oxide);

	/// @return efficiency total efficiency throughout electrorefining
	static double RefineEff(double temperature, double pressure, double rotation);

	/// @return efficiency total efficiency throughout electrowinning
	static double WinningEff(double current, double reprocess_time, double flowrate);
};



#endif //
