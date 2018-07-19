#ifndef _EFFICIENCY_H
#define _EFFICIENCY_H

#include "pyre.h"



class Efficiency {

public:
	Efficiency();

	Efficiency(double temperature, double reprocess_time, double flowrate,
		double current, double lithium_oxide, double pressure, double rotation);
	
	/// @return efficiency total efficiency throughout voloxidation
	double VoloxEff(double temperature, double reprocess_time, double flowrate);

	/// @return efficiency total efficiency throughout electroreduction
	double ReductEff(double current, double lithium_oxide);

	/// @return efficiency total efficiency throughout electrorefining
	double RefineEff(double temperature, double pressure, double rotation);

	/// @return efficiency total efficiency throughout electrowinning
	double WinningEff(double current, double reprocess_time, double flowrate);
};



#endif //
