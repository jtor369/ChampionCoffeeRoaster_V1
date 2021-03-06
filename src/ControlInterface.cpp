#include "ControlInterface.h"
#include "UnitSpecifications.h"


const uint8_t addr_beans = 0x60, addr_environment = 0x67;
MCP9600 BeanTemperature(addr_beans), EnvironmentTemperature(addr_environment);

TemperatureControl temperature_feedback = Environment;

/*! \var float bean_temperature_target
    \brief Contains the last commanded desired bean temperature
*/
volatile float bean_temperature_target = 0.0;

/*! \var float bean_temperature_measured
    \brief Contains the last measured bean temperature
*/
float bean_temperature_measured = 0.0;

/*! \var float environment_temperature_target
    \brief Contains the last commanded desired environment temperature
*/
volatile float environment_temperature_target = 0.0;

/*! \var float environment_temperature_measured
    \brief Contains the last measured environment temperature
*/
float environment_temperature_measured = 0.0;

/*! \var float heating_power
    \brief 
*/
volatile float heating_power = 0.0;

/*! \var bool heating_is_enabled
    \brief 
*/
volatile bool heating_is_enabled = false;


/*! \fn void refreshTemperatures()
    \brief Reads and stores the temperatures from the MCP9600 thermocouples.
*/
void RefreshTemperatures()
{
    get_temperature(&BeanTemperature,&bean_temperature_measured);
    get_temperature(&EnvironmentTemperature,&environment_temperature_measured);
}

/*! \fn void ControlEnvironmentTemperature()
    \brief Sets the control loop feedback to use the measured Environment Temperature
*/
void ControlEnvironmentTemperature()
{
    temperature_feedback = Environment;
}

/*! \fn void ControlBeanTemperature()
    \brief Sets the control loop feedback to use the measured Bean Temperature
*/
void ControlBeanTemperature()
{
    temperature_feedback = Bean;
}

/*! \fn TemperatureControl ReadTemperatureControl()
    \brief Returns an enum variable describing which feedback temperature is being used in the temperature control loop
*/
TemperatureControl ReadTemperatureControl()
{
    return temperature_feedback;
}

/*! \fn void SetBeanTarget(float target_temperature)
    \brief 
*/
void SetBeanTarget(float target_temperature)
{
    if (target_temperature <= MaxBeanTemperature && target_temperature >= 0.0){
        bean_temperature_target = target_temperature;
    }
    
}

/*! \fn float GetBeanTarget();
    \brief 
*/
float GetBeanTarget()
{
    return bean_temperature_target;
}


/*! \fn float ReadBeanMeasured();
    \brief 
*/
float ReadBeanMeasured()
{
    return bean_temperature_measured;
}

/*! \fn void SetEnvironmentTarget(float target_temperature);
    \brief 
*/
void SetEnvironmentTarget(float target_temperature)
{
    if (target_temperature <= MaxEnvironmentTemperature && target_temperature >= 0.0){
        environment_temperature_target = target_temperature;
    }
}

/*! \fn float GetEnvironmentTarget()
    \brief 
*/
float GetEnvironmentTarget()
{
    return environment_temperature_target;
}

/*! \fn float ReadEnvironmentMeasured()
    \brief 
*/
float ReadEnvironmentMeasured()
{
    return environment_temperature_measured;
}

/*! \fn void EnableHeating();
    \brief 
*/
void EnableHeating()
{
    heating_is_enabled = true;
}

/*! \fn void DisableHeating();
    \brief 
*/
void DisableHeating()
{
    heating_is_enabled = false;
}

/*! \fn ReadHeatingStatus();
    \brief Return whether the heating element is currently enabled
*/
bool ReadHeatingEnabled()
{
    return heating_is_enabled;
}

float GetControlTemperature()
{
    switch (temperature_feedback)
    {
        case Environment:
            return environment_temperature_measured;
        break;
        case Bean:
            return bean_temperature_measured;
        break;
    
    default:
        break;
    }
    return 250.0;
}

//Debug
void SetHeatingPower(float power)
{
    if (power > 1.0){
        heating_power = 1.0;
    }else if (power < 0.0){
        heating_power = 0.0;
    }else{
        heating_power = power;
    }
}

//Monitor
float GetHeatingPower()
{
    return heating_power;
}



	
	
