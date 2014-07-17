// HTF_props.h -- function prototypes for HTF property routines
#ifndef __HTF_PROPS_
#define __HTF_PROPS_

#include <numeric>
#include <limits>

//#include <shared/lib_util.h>

#include "interpolation_routines.h"

class HTFProperties
{
public:
	HTFProperties();
	
	enum {
		Air = 1,
		Stainless_AISI316,
		Water_liquid,
		Steam,
		CO2,
		Salt_68_KCl_32_MgCl2,
		Salt_8_NaF_92_NaBF4,
		Salt_25_KF_75_KBF4,
		Salt_31_RbF_69_RbBF4,
		Salt_465_LiF_115_NaF_42KF,
		Salt_49_LiF_29_NaF_29_ZrF4,
		Salt_58_KF_42_ZrF4,
		Salt_58_LiCl_42_RbCl,
		Salt_58_NaCl_42_MgCl2,
		Salt_595_LiCl_405_KCl,
		Salt_595_NaF_405_ZrF4,
		Salt_60_NaNO3_40_KNO3,
		Nitrate_Salt,
		Caloria_HT_43,
		Hitec_XL,
		Therminol_VP1,
		Hitec,
		Dowtherm_Q,
		Dowtherm_RP,
		Blank1,
		Argon_ideal,
		Hydrogen_ideal,
		T91_Steel,
		Therminol_66,
		Therminol_59,
		End_Library_Fluids,
		User_defined = 50
	};
	
	/* User defined table must have 7 columns, and at least 3 rows
	 col 0: Temp 'C
	 col 1: Cp kJ/kg-K
	 col 2: Dens kg/m3
	 col 3: Visc Pa-s
	 col 4: Kinematic visc m2/s
	 col 5: conductivity W/m-K
	 col 6: Enthalpy J/kg
	*/
	
	const char *UserFluidErrMessage(){return uf_err_msg.c_str();}
	bool SetFluid( int fluid );
	int GetFluid() { return m_fluid; }
	bool SetUserDefinedFluid( const util::matrix_t<double> &table );

	double Cp( double T_K );
	double dens( double T_K, double P );
	double visc( double T_K );
	double cond( double T_K );
	double Cv( double T_K );
	double kin_visc( double T_K, double P );
	double therm_diff( double T_K, double P );
	double Pr( double T_K, double P );
	double Re( double T_K, double P, double vel, double d );
	double temp( double H );
	double enth( double T_K );

private:
	static const int m_m = 2;		// Integer for interpolation routine

	Linear_Interp User_Defined_Props;		// Define interpolation class in case user defined propeties are required

	int m_fluid;	// Store fluid number as member integer
	util::matrix_t<double> m_userTable;	// User table of properties

	std::string uf_err_msg;	//Error message when the user HTF table is invalid
};

class AbsorberProps
{
	int mnum;
public:
	//Absorber materials:
	// (1)   304L
	// (2)   216L
	// (3)   321H
	// (4)   B42 Copper Pipe
	enum{
		Mat_304L=1,
		Mat_216L,
		Mat_321H,
		Mat_B42_Copper_Pipe
	};

	void setMaterial(int mat_num){mnum = mat_num;}

	double cond(double T, int mat_num=-1){
		//T [C]
		//returns W/m-K
		//optional to use argument for material number
		int mtemp = mat_num;
		if(mat_num < 0) mtemp = mnum;
		switch(mtemp){
		case Mat_304L:
			return 0.013 * T + 15.2;  //[W/m-K];
		case Mat_216L:
			return 0.013 * T + 15.2;  //[W/m-K];
		case Mat_321H:
			return 0.0153 * T + 14.775;
		case Mat_B42_Copper_Pipe:
			return 400.;
		};
		return std::numeric_limits<double>::quiet_NaN();
	};
};

#endif