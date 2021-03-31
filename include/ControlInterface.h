#pragma once

#include "Seeed_MCP9600_interface.h"

enum TemperatureControl{
    Environment,
    Bean
};

/*! \fn void ControlEnvironmentTemperature()
    \brief Sets the control loop feedback to use the measured Environment Temperature
*/
void ControlEnvironmentTemperature();

/*! \fn void ControlBeanTemperature()
    \brief Sets the control loop feedback to use the measured Bean Temperature
*/
//void ControlBeanTemperature();


/*! \fn TemperatureControl ReadTemperatureControl()
    \brief Returns an enum variable describing which feedback temperature is being used in the temperature control loop
*/
TemperatureControl ReadTemperatureControl();


/*! \fn void SetBeanTarget(float target_temperature)
    \brief 
*/
//void SetBeanTarget(float target_temperature);

/*! \fn float GetBeanTarget();
    \brief 
*/
float GetBeanTarget();

/*! \fn float ReadBeanMeasured();
    \brief 
*/
float ReadBeanMeasured();
	
/*! \fn void SetEnvironmentTarget(float target_temperature);
    \brief 
*/
//void SetEnvironmentTarget(float target_temperature);

/*! \fn float GetEnvironmentTarget()
    \brief 
*/
float GetEnvironmentTarget();

/*! \fn float ReadEnvironmentMeasured()
    \brief 
*/
float ReadEnvironmentMeasured();
	
/*! \fn void EnableHeating();
    \brief 
*/
void EnableHeating();

/*! \fn void DisableHeating();
    \brief 
*/
void DisableHeating();

/*! \fn ReadHeatingStatus();
    \brief 
*/
	
//void SetTemperatureUpdateSpeed(); //should be permanent, since it is used in a control loop;
	
float GetControlTemperature();

float GetTargetControlTemperature();
void SetTargetControlTemperature(float target);

float GetHeatingPower();
void SetHeatingPower(float power);
bool ReadHeatingEnabled();

void RefreshTemperatures();


void Init_MCP9600();


