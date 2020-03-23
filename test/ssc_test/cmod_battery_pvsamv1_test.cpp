#include <gtest/gtest.h>

#include "cmod_battery_pvsamv1_test.h"

#include "../input_cases/weather_inputs.h"
#include "../input_cases/pvsam1_battery_common_data.h"

/// Test PVSAMv1 with all defaults and battery enabled with 3 automatic dispatch methods
TEST_F(CMPvsamv1BatteryIntegration_cmod_pvsamv1, ResidentialACBatteryModelIntegration)
{
	ssc_data_t data = ssc_data_create();
	pvsamv_nofinancial_default(data);
	battery_data_default(data);

	std::map<std::string, double> pairs;
	pairs["en_batt"] = 1;
	pairs["batt_ac_or_dc"] = 1; //AC
	pairs["analysis_period"] = 1;
	set_array(data, "load", load_profile_path, 8760); // Load is required for peak shaving controllers

	ssc_number_t expectedEnergy[3] = { 8594, 8594, 8689 };
	ssc_number_t expectedBatteryChargeEnergy[3] = { 1442, 1443, 258 };
	ssc_number_t expectedBatteryDischargeEnergy[3] = { 1321, 1323, 233 };

	// Test peak shaving look ahead, peak shaving look behind, and automated grid power target. Others require additional input data
	for (int i = 0; i < 3; i++) {
		pairs["batt_dispatch_choice"] = i;

		int pvsam_errors = modify_ssc_data_and_run_module(data, "pvsamv1", pairs);
		EXPECT_FALSE(pvsam_errors);

		if (!pvsam_errors)
		{
			ssc_number_t annual_energy;
			ssc_data_get_number(data, "annual_energy", &annual_energy);
			EXPECT_NEAR(annual_energy, expectedEnergy[i], m_error_tolerance_hi) << "Annual energy.";

			auto data_vtab = static_cast<var_table*>(data);
			auto annualChargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_charge_energy");
			EXPECT_NEAR(annualChargeEnergy[0], expectedBatteryChargeEnergy[i], m_error_tolerance_hi) << "Battery annual charge energy.";
			
			auto annualDischargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_discharge_energy");
			EXPECT_NEAR(annualDischargeEnergy[0], expectedBatteryDischargeEnergy[i], m_error_tolerance_hi) << "Battery annual discharge energy.";
		}
	}
}

/// Test PVSAMv1 with all defaults and battery enabled with custom dispatch
TEST_F(CMPvsamv1BatteryIntegration_cmod_pvsamv1, ResidentialACDCBatteryModelIntegrationCustomDispatch)
{
	ssc_data_t data = ssc_data_create();
	pvsamv_nofinancial_default(data);
	battery_data_default(data);

	std::map<std::string, double> pairs;
	pairs["en_batt"] = 1;

	pairs["analysis_period"] = 1;
	set_array(data, "load", load_profile_path, 8760); // Load is required for peak shaving controllers
	pairs["batt_dispatch_choice"] = 3;
	set_array(data, "batt_custom_dispatch", custom_dispatch_residential_schedule, 8760);

	ssc_number_t expectedEnergy[2] = { 8710, 8717 };
	ssc_number_t expectedBatteryChargeEnergy[2] = { 4.6, 4.7};
	ssc_number_t expectedBatteryDischargeEnergy[2] = { 0.76, 7.6};

	// Test both AC and DC using the same dispatch model
	for (int i = 0; i < 2; i++) {
		pairs["batt_ac_or_dc"] = i;

		int pvsam_errors = modify_ssc_data_and_run_module(data, "pvsamv1", pairs);
		EXPECT_FALSE(pvsam_errors);

		if (!pvsam_errors)
		{
			ssc_number_t annual_energy;
			ssc_data_get_number(data, "annual_energy", &annual_energy);
			EXPECT_NEAR(annual_energy, expectedEnergy[i], m_error_tolerance_hi) << "Annual energy.";

			auto data_vtab = static_cast<var_table*>(data);
			auto annualChargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_charge_energy");
			EXPECT_NEAR(annualChargeEnergy[0], expectedBatteryChargeEnergy[i], m_error_tolerance_hi) << "Battery annual charge energy.";

			auto annualDischargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_discharge_energy");
			EXPECT_NEAR(annualDischargeEnergy[0], expectedBatteryDischargeEnergy[i], m_error_tolerance_hi) << "Battery annual discharge energy.";

		}
	}
}

/// Test PVSAMv1 with all defaults and battery enabled with manual dispatch
TEST_F(CMPvsamv1BatteryIntegration_cmod_pvsamv1, ResidentialACDCBatteryModelIntegrationManualDispatch)
{
	ssc_data_t data = ssc_data_create();
	pvsamv_nofinancial_default(data);
	battery_data_default(data);

	std::map<std::string, double> pairs;
	pairs["en_batt"] = 1;

	pairs["analysis_period"] = 1;
	set_array(data, "load", load_profile_path, 8760); // Load is required for peak shaving controllers
	pairs["batt_dispatch_choice"] = 4;

	ssc_number_t expectedEnergy[2] = { 8701, 8684 };
	ssc_number_t expectedBatteryChargeEnergy[2] = { 468, 477 };
	ssc_number_t expectedBatteryDischargeEnergy[2] = { 437, 446 };

	// Test both AC and DC using the same dispatch model
	for (int i = 0; i < 2; i++) {
		pairs["batt_ac_or_dc"] = i;

		int pvsam_errors = modify_ssc_data_and_run_module(data, "pvsamv1", pairs);
		EXPECT_FALSE(pvsam_errors);

		if (!pvsam_errors)
		{
			ssc_number_t annual_energy;
			ssc_data_get_number(data, "annual_energy", &annual_energy);
			EXPECT_NEAR(annual_energy, expectedEnergy[i], m_error_tolerance_hi) << "Annual energy.";

			auto data_vtab = static_cast<var_table*>(data);
			auto annualChargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_charge_energy");
			EXPECT_NEAR(annualChargeEnergy[0], expectedBatteryChargeEnergy[i], m_error_tolerance_hi) << "Battery annual charge energy.";

			auto annualDischargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_discharge_energy");
			EXPECT_NEAR(annualDischargeEnergy[0], expectedBatteryDischargeEnergy[i], m_error_tolerance_hi) << "Battery annual discharge energy.";
		}
	}
}

/// Test PVSAMv1 with all defaults and DC battery enabled with 3 automatic dispatch methods
TEST_F(CMPvsamv1BatteryIntegration_cmod_pvsamv1, ResidentialDCBatteryModelIntegration)
{
	ssc_data_t data = ssc_data_create();
	pvsamv_nofinancial_default(data);
	battery_data_default(data);

	std::map<std::string, double> pairs;
	pairs["en_batt"] = 1;
	pairs["batt_ac_or_dc"] = 0; //DC
	pairs["analysis_period"] = 1;
	set_array(data, "load", load_profile_path, 8760); // Load is required for peak shaving controllers

	ssc_number_t expectedEnergy[3] = { 8634, 8637, 8703 };
	ssc_number_t expectedBatteryChargeEnergy[3] = { 1373, 1377, 245 };
	ssc_number_t expectedBatteryDischargeEnergy[3] = { 1245, 1249, 219 };

	// Test peak shaving look ahead, peak shaving look behind, and automated grid power target. Others require additional input data
	for (int i = 0; i < 3; i++) {
		pairs["batt_dispatch_choice"] = i;

		int pvsam_errors = modify_ssc_data_and_run_module(data, "pvsamv1", pairs);
		EXPECT_FALSE(pvsam_errors);

		if (!pvsam_errors)
		{
			ssc_number_t annual_energy;
			ssc_data_get_number(data, "annual_energy", &annual_energy);
			EXPECT_NEAR(annual_energy, expectedEnergy[i], m_error_tolerance_hi) << "Annual energy.";

			auto data_vtab = static_cast<var_table*>(data);
			auto annualChargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_charge_energy");
			EXPECT_NEAR(annualChargeEnergy[0], expectedBatteryChargeEnergy[i], m_error_tolerance_hi) << "Battery annual charge energy.";

			auto annualDischargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_discharge_energy");
			EXPECT_NEAR(annualDischargeEnergy[0], expectedBatteryDischargeEnergy[i], m_error_tolerance_hi) << "Battery annual discharge energy.";
		}
	}
}

/// Test PVSAMv1 with all defaults and battery enabled with 3 automatic dispatch methods
TEST_F(CMPvsamv1BatteryIntegration_cmod_pvsamv1, PPA_ACBatteryModelIntegration)
{
	ssc_data_t data = ssc_data_create();
	pvsamv1_pv_defaults(data);
	pvsamv1_battery_defaults(data);
	grid_and_rate_defaults(data);
	singleowner_defaults(data);

	ssc_number_t expectedEnergy[3] = { 37322265, 37320217, 37321282 };
	ssc_number_t expectedBatteryChargeEnergy[3] = { 0, 0, 0 };
	ssc_number_t expectedBatteryDischargeEnergy[3] = { 0, 0, 0 };

	// Test peak shaving look ahead, peak shaving look behind, and automated grid power target. Others require additional input data
	for (int i = 0; i < 3; i++) {
		ssc_data_set_number(data, "batt_dispatch_choice", i);

		int pvsam_errors = run_pvsam1_battery_ppa(data);
		EXPECT_FALSE(pvsam_errors);

		if (!pvsam_errors)
		{
			ssc_number_t annual_energy;
			ssc_data_get_number(data, "annual_energy", &annual_energy);
			EXPECT_NEAR(annual_energy, expectedEnergy[i], m_error_tolerance_hi) << "Annual energy.";

			auto data_vtab = static_cast<var_table*>(data);
			auto annualChargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_charge_energy");
			EXPECT_NEAR(annualChargeEnergy[0], expectedBatteryChargeEnergy[i], m_error_tolerance_hi) << "Battery annual charge energy.";

			auto annualDischargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_discharge_energy");
			EXPECT_NEAR(annualDischargeEnergy[0], expectedBatteryDischargeEnergy[i], m_error_tolerance_hi) << "Battery annual discharge energy.";
		}
	}
}

/// Test PVSAMv1 with all defaults and battery enabled with custom dispatch and PPA financial model
TEST_F(CMPvsamv1BatteryIntegration_cmod_pvsamv1, PPA_CustomDispatchBatteryModelIntegration)
{
	ssc_data_t data = ssc_data_create();
	pvsamv1_pv_defaults(data);
	pvsamv1_battery_defaults(data);
	grid_and_rate_defaults(data);
	singleowner_defaults(data);

	ssc_number_t expectedEnergy = 37322006;
	ssc_number_t expectedBatteryChargeEnergy = 0;
	ssc_number_t expectedBatteryDischargeEnergy = 0;

	ssc_data_set_number(data, "batt_dispatch_choice", 3);
	set_array(data, "batt_custom_dispatch", custom_dispatch_singleowner_schedule, 8760);

	int pvsam_errors = run_pvsam1_battery_ppa(data);
	EXPECT_FALSE(pvsam_errors);

	if (!pvsam_errors)
	{
		ssc_number_t annual_energy;
		ssc_data_get_number(data, "annual_energy", &annual_energy);
		EXPECT_NEAR(annual_energy, expectedEnergy, m_error_tolerance_hi) << "Annual energy.";

		auto data_vtab = static_cast<var_table*>(data);
		auto annualChargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_charge_energy");
		EXPECT_NEAR(annualChargeEnergy[0], expectedBatteryChargeEnergy, m_error_tolerance_hi) << "Battery annual charge energy.";

		auto annualDischargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_discharge_energy");
		EXPECT_NEAR(annualDischargeEnergy[0], expectedBatteryDischargeEnergy, m_error_tolerance_hi) << "Battery annual discharge energy.";
	}
	
}

/// Test PVSAMv1 with all defaults and battery enabled with manual dispatch and PPA financial model
TEST_F(CMPvsamv1BatteryIntegration_cmod_pvsamv1, PPA_ManualDispatchBatteryModelIntegration)
{
	ssc_data_t data = ssc_data_create();
	pvsamv1_pv_defaults(data);
	pvsamv1_battery_defaults(data);
	grid_and_rate_defaults(data);
	singleowner_defaults(data);

	ssc_number_t expectedEnergy = 37145455;
	ssc_number_t expectedBatteryChargeEnergy = 0;
	ssc_number_t expectedBatteryDischargeEnergy = 0;

	ssc_data_set_number(data, "batt_dispatch_choice", 4);

	int pvsam_errors = run_pvsam1_battery_ppa(data);
	EXPECT_FALSE(pvsam_errors);

	if (!pvsam_errors)
	{
		ssc_number_t annual_energy;
		ssc_data_get_number(data, "annual_energy", &annual_energy);
		EXPECT_NEAR(annual_energy, expectedEnergy, m_error_tolerance_hi) << "Annual energy.";

		auto data_vtab = static_cast<var_table*>(data);
		auto annualChargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_charge_energy");
		EXPECT_NEAR(annualChargeEnergy[0], expectedBatteryChargeEnergy, m_error_tolerance_hi) << "Battery annual charge energy.";

		auto annualDischargeEnergy = data_vtab->as_vector_ssc_number_t("batt_annual_discharge_energy");
		EXPECT_NEAR(annualDischargeEnergy[0], expectedBatteryDischargeEnergy, m_error_tolerance_hi) << "Battery annual discharge energy.";
	}

}