#include "core.h"

static var_info _cm_vtab_levpartflip[] = {

/*   VARTYPE           DATATYPE         NAME                         LABEL                              UNITS     META                      GROUP          REQUIRED_IF                 CONSTRAINTS                      UI_HINTS*/
	{ SSC_INPUT,        SSC_ARRAY,      "system_enet",				"Annual energy produced by system",	"kWh",   "",                      "DHF",             "*",						   "",                              "" },
/* constraint is > 0 */
	{ SSC_INPUT,        SSC_NUMBER,     "system_nameplate",			"System nameplate capacity",		"kW",    "",                      "DHF",             "*",						   "MIN=1",                         "" },
/* want analysis years determined by enet length - system degradation, availablity compute module output */
	{ SSC_INPUT,        SSC_NUMBER,     "analysis_years",           "Number of years in analysis",     "years",  "",                      "DHF",             "?=30",                      "INTEGER,MIN=1,MAX=40",          "" }, 
	{ SSC_INPUT,        SSC_NUMBER,     "inflation",			    "Inflation rate",					"%",     "",                      "DHF",             "*",						   "MIN=0,MAX=100",					"" },
	{ SSC_INPUT,        SSC_NUMBER,     "discount_real",			"Real discount rate",				"%",     "",                      "DHF",             "*",						   "MIN=0,MAX=100",					"" },
	{ SSC_INPUT,        SSC_NUMBER,     "sales_tax_rate",		    "Sales tax rate",	    			"%",     "",                      "DHF",             "*",						   "MIN=0,MAX=100",					"" },

/* costs - to be updated based on meetings with DHF */
	{ SSC_INPUT,        SSC_NUMBER,     "cost_gen_equip",           "Generation equiptment cost",		"$",	 "",					  "DHF",             "?=24000000",              "MIN=0",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,     "cost_bop",					"Balance of plant cost",			"$",	 "",					  "DHF",             "?=8000000",               "MIN=0",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,     "cost_network",             "Network upgrade cost",				"$",	 "",					  "DHF",             "?=3500000",               "MIN=0",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,     "percent_contingency",      "Contingency percent",				"%",	 "",					  "DHF",             "?=1",                       "MIN=0,MAX=100",		        	"" },

	{ SSC_INPUT,        SSC_NUMBER,     "cost_developer",           "Developer cost & fees",		    "$",	 "",					  "DHF",             "?=2000000",               "MIN=0",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,     "cost_land_improve",        "Land improvements cost",		    "$",	 "",					  "DHF",             "?=200000",                 "MIN=0",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,     "cost_other",               "Other cost",						"$",	 "",					  "DHF",             "?=75000",                  "MIN=0",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,     "percent_taxable",          "Taxable cost",				        "%",	 "",					  "DHF",             "?=100",                     "MIN=0,MAX=100",      			"" },

/* inputs in DHF model not currently in SAM 11/15/10 */
	{ SSC_INPUT,        SSC_NUMBER,     "reserves_interest",        "Interest on reserves",				"%",	 "",					  "DHF",             "?=1.75",                     "MIN=0,MAX=100",      			"" },
	{ SSC_INPUT,        SSC_NUMBER,     "prop_tax_cost_assessed",   "Percent of pre-financing costs assessed","%","",					  "DHF",			 "?=95",                     "MIN=0,MAX=100",      			"" },
	{ SSC_INPUT,        SSC_NUMBER,     "prop_tax_assessed_decline","Assessed value annual decline",	"%",	 "",					  "DHF",             "?=95",                     "MIN=0,MAX=100",      			"" },

/* DHF replacement reserve on top of regular o and m */
	{ SSC_INPUT,        SSC_NUMBER,     "equip_reserve1_cost",      "Major equipment reserve1 cost",	"$/Wdc",	 "",				  "DHF",             "?=0.25",               "MIN=0",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,     "equip_reserve1_freq",      "Major equipment reserve1 frequency",	"years",	 "",			  "DHF",             "?=12",               "INTEGER,MIN=0",                         "" },

	{ SSC_INPUT,        SSC_NUMBER,     "equip_reserve2_cost",      "Major equipment reserve2 cost",	"$/Wdc",	 "",				  "DHF",             "?=0",               "MIN=0",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,     "equip_reserve2_freq",      "Major equipment reserve2 frequency",	"years",	 "",			  "DHF",             "?=15",               "INTEGER,MIN=0",                         "" },

	{ SSC_INPUT,        SSC_NUMBER,     "equip_reserve3_cost",      "Major equipment reserve3 cost",	"$/Wdc",	 "",				  "DHF",             "?=0",               "MIN=0",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,     "equip_reserve3_freq",      "Major equipment reserve3 frequency",	"years",	 "",			  "DHF",             "?=20",               "INTEGER,MIN=0",                         "" },

// TODO - add to depreciation static table */
	{ SSC_INPUT,        SSC_NUMBER,     "equip_reserve_depr_sta",   "Major equipment reserve state depreciation",	"",	 "0=5yr MACRS,1=15yr MACRS,2=5yr SL,3=15yr SL, 4=20yr SL,5=39yr SL",  "DHF", "?=0",   "INTEGER,MIN=0,MAX=5",  "" },
	{ SSC_INPUT,        SSC_NUMBER,     "equip_reserve_depr_fed",   "Major equipment reserve federal depreciation",	"",	 "0=5yr MACRS,1=15yr MACRS,2=5yr SL,3=15yr SL, 4=20yr SL,5=39yr SL",  "DHF", "?=0",   "INTEGER,MIN=0,MAX=5",  "" },
	
/* DHF market specific inputs - leveraged partnership flip */
	{ SSC_INPUT,        SSC_NUMBER,		"ppa_soln_mode",            "PPA solution mode",                "0/1",   "0=specify ppa,1=solve ppa", "DHF",         "?=0",                     "INTEGER,MIN=0,MAX=1",            "" },
	{ SSC_INPUT,        SSC_NUMBER,     "ppa_price",				"Initial year PPA price",			"cents/kWh",	 "",			  "DHF",			 "ppa_soln_mode=1",         "",      			"" },
	{ SSC_INPUT,        SSC_NUMBER,     "ppa_escalation",           "PPA escalation",					"%",	 "",					  "DHF",             "?=0",                     "MIN=0,MAX=100",      			"" },
/* DHF construction period */
	{ SSC_INPUT,        SSC_NUMBER,     "constr_period",            "Construction period",				"months", "",				      "DHF",             "?=10",					"INTEGER,MIN=0",      			"" },
	{ SSC_INPUT,        SSC_NUMBER,     "constr_int_rate",          "Construction interest rate",		"%",	 "",					  "DHF",             "?=4",                     "MIN=0,MAX=100",      			"" },
	{ SSC_INPUT,        SSC_NUMBER,     "ppa_escalation",           "Construction up-front fee",    	"%",	 "",					  "DHF",             "?=1",                     "MIN=0,MAX=100",      			"" },
/* DHF term financing */
	{ SSC_INPUT,        SSC_NUMBER,     "term_tenor",               "Term financing tenor",				"years", "",				      "DHF",             "?=10",					"INTEGER,MIN=0",      			"" },
	{ SSC_INPUT,        SSC_NUMBER,     "term_int_rate",            "Term financing interest rate",		"%",	 "",					  "DHF",             "?=8.5",                   "MIN=0,MAX=100",      			"" },
	{ SSC_INPUT,        SSC_NUMBER,     "dscr",						"Debt service coverage ratio",		"",	     "",				      "DHF",             "?=1.5",					"MIN=0",      			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "dscr_reserve",				"Debt service reserve account",		"months P&I","",			      "DHF",             "?=6",					    "INTEGER,MIN=0",      			        "" },
/* DHF Capital Cost */
	{ SSC_INPUT,        SSC_NUMBER,     "cost_debt_closing",		"Debt closing cost",				"$",	 "",					  "DHF",             "?=250000",					    "MIN=0",      			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "cost_equity_closing",		"Equity closing cost",				"$",	 "",					  "DHF",             "?=100000",					    "MIN=0",      			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "cost_working_reserve",		"Q&M/Working capital reserve",		"$",	 "",					  "DHF",             "?=150000",					    "MIN=0",      			        "" },
/* DHF Equity Structure */
	{ SSC_INPUT,        SSC_NUMBER,     "equity_tax_investor",		"Tax investor equity",				"%",	 "",					  "DHF",             "?=98",					  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "preflip_sharing_tax_investor",		"Tax investor pre-flip cash sharing ratio",		"%",	 "",  "DHF",             "?=98",					  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "postflip_sharing_tax_investor",	"Tax investor post-flip cash sharing ratio",	"%",	 "",  "DHF",             "?=15",					  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "return_target",			"After-tax flip/return target",		"%",	 "",					  "DHF",             "?=11",					  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "return_target_year",		"Return target year",				"",		 "",					  "DHF",             "?=11",					  "MIN=1",     			        "" },
/* DHF depreciation allocation */
	{ SSC_INPUT,        SSC_NUMBER,     "depr_alloc_macrs_5",		"5-yr MACRS depreciation federal and state allocation",	"%", "",	  "DHF",             "?=89",					  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "depr_alloc_macrs_15",		"15-yr MACRS depreciation federal and state allocation",	"%", "",  "DHF",             "?=1.5",					  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "depr_alloc_sl_5",		"5-yr straight line depreciation federal and state allocation",	"%", "",  "DHF",             "?=0",						  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "depr_alloc_sl_15",		"15-yr straight line depreciation federal and state allocation","%", "",  "DHF",             "?=3",						  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "depr_alloc_sl_20",		"20-yr straight line depreciation federal and state allocation","%", "",  "DHF",             "?=3",						  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,     "depr_alloc_sl_39",		"39-yr straight line depreciation federal and state allocation","%", "",  "DHF",             "?=0.5",					  "MIN=0,MAX=100",     			        "" },
/* DHF bonus depreciation */
	{ SSC_INPUT,        SSC_NUMBER,     "depr_bonus_sta",			"State bonus depreciation",			"%",	 "",					  "DHF",             "?=0",						  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_sta_macrs_5",   "State bonus depreciation 5-yr MACRS","0/1", "",                      "DHF",			 "?=1",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_sta_macrs_15",   "State bonus depreciation 15-yr MACRS","0/1","",                     "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_sta_sl_5",   "State bonus depreciation 5-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_sta_sl_15",   "State bonus depreciation 15-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_sta_sl_20",   "State bonus depreciation 20-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_sta_sl_39",   "State bonus depreciation 39-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },

	{ SSC_INPUT,        SSC_NUMBER,     "depr_bonus_fed",			"Federal bonus depreciation",			"%",	 "",					  "DHF",             "?=0",						  "MIN=0,MAX=100",     			        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_fed_macrs_5",   "Federal bonus depreciation 5-yr MACRS","0/1", "",                      "DHF",			 "?=1",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_fed_macrs_15",   "Federal bonus depreciation 15-yr MACRS","0/1","",                     "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_fed_sl_5",   "Federal bonus depreciation 5-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_fed_sl_15",   "Federal bonus depreciation 15-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_fed_sl_20",   "Federal bonus depreciation 20-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_bonus_fed_sl_39",   "Federal bonus depreciation 39-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
/* DHF ITC depreciation */
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_sta_macrs_5",   "State itc depreciation 5-yr MACRS","0/1", "",                      "DHF",			 "?=1",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_sta_macrs_15",   "State itc depreciation 15-yr MACRS","0/1","",                     "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_sta_sl_5",   "State itc depreciation 5-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_sta_sl_15",   "State itc depreciation 15-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_sta_sl_20",   "State itc depreciation 20-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_sta_sl_39",   "State itc depreciation 39-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },

	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_fed_macrs_5",   "Federal itc depreciation 5-yr MACRS","0/1", "",                      "DHF",			 "?=1",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_fed_macrs_15",   "Federal itc depreciation 15-yr MACRS","0/1","",                     "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_fed_sl_5",   "Federal itc depreciation 5-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_fed_sl_15",   "Federal itc depreciation 15-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_fed_sl_20",   "Federal itc depreciation 20-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },
	{ SSC_INPUT,        SSC_NUMBER,		"depr_itc_fed_sl_39",   "Federal itc depreciation 39-yr straight line","0/1","",                  "DHF",			 "?=0",                       "BOOLEAN",                        "" },

/* intermediate outputs */
	{ SSC_OUTPUT,       SSC_NUMBER,      "cost_contingency",        "Contingency cost",                 "$",     "",					   "DHF",			   "*",                         "",                             "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "cost_hard",               "Hard cost",                        "$",     "",					   "DHF",			   "*",                         "",                             "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "cost_soft",               "Soft cost",                        "$",     "",					   "DHF",			   "*",                         "",                             "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "cost_salestax",           "Sales tax",                        "$",     "",					   "DHF",			   "*",                         "",                             "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "cost_installed",          "Installed cost",                   "$",     "",					   "DHF",			   "*",                         "",                             "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "cost_installedperwatt",   "Installed cost per watt",          "$/W",   "",					   "DHF",			   "*",                         "",                             "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "discount_nominal",        "Nominal discount rate",            "%",     "",					   "DHF",			   "*",                         "",                             "" },
	{ SSC_OUTPUT,       SSC_NUMBER,      "prop_tax_assessed_value", "Assessed value of property for tax purposes","$", "",				   "DHF",			   "*",                         "",                             "" },

var_info_invalid };

extern var_info
	vtab_standard_financial[],
	vtab_standard_loan[],
	vtab_oandm[],
	vtab_depreciation[],
	vtab_utility_rate[],
	vtab_tax_credits[],
	vtab_payment_incentives[];

enum {
	CF_energy_net,
	CF_energy_value,

	CF_om_fixed_expense,
	CF_om_production_expense,
	CF_om_capacity_expense,
	CF_om_fuel_expense,
	CF_property_tax_expense,
	CF_insurance_expense,
	CF_operating_expenses,

	CF_deductible_expenses,

	CF_debt_balance,
	CF_debt_payment_interest,
	CF_debt_payment_principal,
	CF_debt_payment_total,
	
	CF_ibi_fed_amt,
	CF_ibi_sta_amt,
	CF_ibi_uti_amt,
	CF_ibi_oth_amt,
	CF_ibi_fed_per,
	CF_ibi_sta_per,
	CF_ibi_uti_per,
	CF_ibi_oth_per,
	CF_ibi_total,
	
	CF_cbi_fed,
	CF_cbi_sta,
	CF_cbi_uti,
	CF_cbi_oth,
	CF_cbi_total,

	CF_pbi_fed,
	CF_pbi_sta,
	CF_pbi_uti,
	CF_pbi_oth,
	CF_pbi_total,
	
	CF_ptc_fed,
	CF_ptc_sta,
	
	CF_itc_fed_amt,
	CF_itc_fed_per,
	CF_itc_fed_total,

	CF_itc_sta_amt,
	CF_itc_sta_per,
	CF_itc_sta_total,
	
	CF_sta_depr_sched,
	CF_sta_depreciation,
	CF_sta_incentive_income_less_deductions,
	CF_sta_taxable_income_less_deductions,
	CF_sta_tax_savings,
	
	CF_fed_depr_sched,
	CF_fed_depreciation,
	CF_fed_incentive_income_less_deductions,
	CF_fed_taxable_income_less_deductions,
	CF_fed_tax_savings,

	CF_sta_and_fed_tax_savings,
	CF_after_tax_net_equity_cost_flow,
	CF_after_tax_cash_flow,

	CF_payback_with_expenses,
	CF_cumulative_payback_with_expenses,
	
	CF_payback_without_expenses,
	CF_cumulative_payback_without_expenses,

	CF_max };



class cm_levpartflip : public compute_module
{
private:
	util::matrix_t<double> cf;

public:
	cm_levpartflip()
	{
		add_var_info( _cm_vtab_levpartflip );
	}

	void exec( ) throw( general_error )
	{
		double inf = as_double("inflation") / 100.0;
		double disc_real = as_double("discount_real") / 100.0;
		assign( "discount_nominal", var_data((ssc_number_t) ((1+inf)*(1+disc_real)-1) ) );

		double gen = as_double("cost_gen_equip");
		double bop = as_double("cost_bop");
		double net = as_double("cost_network");
		double cont = as_double("percent_contingency") / 100.0;

		double cost_hard = gen + bop + net;
		double cost_cont = cost_hard * cont;

		double dev = as_double("cost_developer");
		double land = as_double("cost_land_improve");
		double other = as_double("cost_other");
		double cost_taxable = as_double("percent_taxable") / 100.0;
		double sales_tax_rate = as_double("sales_tax_rate") / 100.0;

		double cost_soft = dev + land + other;
		double cost_salestax = (cost_hard + cost_cont + cost_soft) * cost_taxable * sales_tax_rate;
		assign( "cost_contingency", var_data((ssc_number_t) cost_cont ) );
		assign( "cost_hard", var_data( (ssc_number_t)(cost_hard + cost_cont)) );
		assign( "cost_salestax", var_data((ssc_number_t)cost_salestax ) );
		assign( "cost_soft", var_data((ssc_number_t) (cost_soft + cost_salestax) ) );

		// use DHF named range names for variables whenever possible
		double pre_financing_installed_cost_total = cost_soft + cost_salestax + cost_hard + cost_cont;
		assign( "cost_installed", var_data((ssc_number_t) pre_financing_installed_cost_total ) );

		double nameplate = as_double("system_nameplate");
		assign( "cost_installedperwatt", var_data((ssc_number_t)( pre_financing_installed_cost_total / nameplate / 1000.0 ) ));
	}

	// std lib
	void depreciation_sched_5_year_macrs_half_year( int cf_line, int nyears )
	{
		for (int i=1; i<=nyears; i++)
		{
			double factor = 0.0;
			switch(i)
			{
			case 1: factor = 0.2000; break;
			case 2: factor = 0.3200; break;
			case 3: factor = 0.1920; break;
			case 4: factor = 0.1152; break;
			case 5: factor = 0.1152; break;
			case 6: factor = 0.0576; break;
			default: factor = 0.0; break;
			}
			cf.at(cf_line, i) = factor;
		}
	}
	// std lib
	void depreciation_sched_15_year_macrs_half_year( int cf_line, int nyears )
	{
		for (int i=1; i<=nyears; i++)
		{
			double factor = 0.0;
			switch(i)
			{
			case 1: factor = 0.0500; break;
			case 2: factor = 0.0950; break;
			case 3: factor = 0.0855; break;
			case 4: factor = 0.0770; break;
			case 5: factor = 0.0693; break;
			case 6: factor = 0.0623; break;
			case 7: factor = 0.0590; break;
			case 8: factor = 0.0590; break;
			case 9: factor = 0.0591; break;
			case 10: factor = 0.0590; break;
			case 11: factor = 0.0591; break;
			case 12: factor = 0.0590; break;
			case 13: factor = 0.0591; break;
			case 14: factor = 0.0590; break;
			case 15: factor = 0.0591; break;
			case 16: factor = 0.0295; break;
			default: factor = 0.0; break;
			}
			cf.at(cf_line, i) = factor;
		}
	}
	// std lib
	void depreciation_sched_5_year_straight_line_half_year( int cf_line, int nyears )
	{
		for (int i=1; i<=nyears; i++)
		{
			double factor = 0.0;
			switch(i)
			{
			case 1: factor = 0.1000; break;
			case 2: factor = 0.2000; break;
			case 3: factor = 0.2000; break;
			case 4: factor = 0.2000; break;
			case 5: factor = 0.2000; break;
			case 6: factor = 0.1000; break;
			default: factor = 0.0; break;
			}
			cf.at(cf_line, i) = factor;
		}
	}
	// std lib
	void depreciation_sched_15_year_straight_line_half_year( int cf_line, int nyears )
	{
		for (int i=1; i<=nyears; i++)
		{
			double factor = 0.0;
			switch(i)
			{
			case 1: factor = 0.0333; break;
			case 2: factor = 0.0667; break;
			case 3: factor = 0.0667; break;
			case 4: factor = 0.0667; break;
			case 5: factor = 0.0667; break;
			case 6: factor = 0.0667; break;
			case 7: factor = 0.0667; break;
			case 8: factor = 0.0666; break;
			case 9: factor = 0.0667; break;
			case 10: factor = 0.0666; break;
			case 11: factor = 0.0667; break;
			case 12: factor = 0.0666; break;
			case 13: factor = 0.0667; break;
			case 14: factor = 0.0666; break;
			case 15: factor = 0.0667; break;
			case 16: factor = 0.0333; break;
			default: factor = 0.0; break;
			}
			cf.at(cf_line, i) = factor;
		}
	}
	// std lib
	void depreciation_sched_20_year_straight_line_half_year( int cf_line, int nyears )
	{
		for (int i=1; i<=nyears; i++)
		{
			double factor = 0.0;
			switch(i)
			{
			case 1: factor = 0.0250; break;
			case 2: factor = 0.0500; break;
			case 3: factor = 0.0500; break;
			case 4: factor = 0.0500; break;
			case 5: factor = 0.0500; break;
			case 6: factor = 0.0500; break;
			case 7: factor = 0.0500; break;
			case 8: factor = 0.0500; break;
			case 9: factor = 0.0500; break;
			case 10: factor = 0.0500; break;
			case 11: factor = 0.0500; break;
			case 12: factor = 0.0500; break;
			case 13: factor = 0.0500; break;
			case 14: factor = 0.0500; break;
			case 15: factor = 0.0500; break;
			case 16: factor = 0.0500; break;
			case 17: factor = 0.0500; break;
			case 18: factor = 0.0500; break;
			case 19: factor = 0.0500; break;
			case 20: factor = 0.0500; break;
			case 21: factor = 0.0250; break;
			default: factor = 0.0; break;
			}
			cf.at(cf_line, i) = factor;
		}
	}
	// std lib
	void depreciation_sched_39_year_straight_line_half_year( int cf_line, int nyears )
	{
		for (int i=1; i<=nyears; i++)
		{
			double factor = 0.0;
			switch(i)
			{
			case 1: factor = 0.0128; break;
			case 2: factor = 0.0256; break;
			case 3: factor = 0.0256; break;
			case 4: factor = 0.0256; break;
			case 5: factor = 0.0256; break;
			case 6: factor = 0.0256; break;
			case 7: factor = 0.0256; break;
			case 8: factor = 0.0256; break;
			case 9: factor = 0.0256; break;
			case 10: factor = 0.0256; break;
			case 11: factor = 0.0256; break;
			case 12: factor = 0.0256; break;
			case 13: factor = 0.0256; break;
			case 14: factor = 0.0256; break;
			case 15: factor = 0.0256; break;
			case 16: factor = 0.0256; break;
			case 17: factor = 0.0256; break;
			case 18: factor = 0.0256; break;
			case 19: factor = 0.0256; break;
			case 20: factor = 0.0256; break;
			case 21: factor = 0.0256; break;
			case 22: factor = 0.0256; break;
			case 23: factor = 0.0256; break;
			case 24: factor = 0.0256; break;
			case 25: factor = 0.0256; break;
			case 26: factor = 0.0256; break;
			case 27: factor = 0.0256; break;
			case 28: factor = 0.0256; break;
			case 29: factor = 0.0256; break;
			case 30: factor = 0.0256; break;
			case 31: factor = 0.0256; break;
			case 32: factor = 0.0256; break;
			case 33: factor = 0.0256; break;
			case 34: factor = 0.0256; break;
			case 35: factor = 0.0256; break;
			case 36: factor = 0.0256; break;
			case 37: factor = 0.0256; break;
			case 38: factor = 0.0256; break;
			case 39: factor = 0.0256; break;
			case 40: factor = 0.0128; break;
			default: factor = 0.0; break;
			}
			cf.at(cf_line, i) = factor;
		}
	}


};

DEFINE_MODULE_ENTRY( levpartflip, "DHF Leveraged Partnership Flip Financial Model_", 1 );


