#pragma once
/*
    This module implements roast profile storage in eeprom and roast profile execution.
    Also detection of physical buttons for profile start and stop [no pause since a roast can be messed up if it is interrupted]

    Functionality to program the default roast profile via serial communication, must be implemented as well..
*/

//prototypes defined in main.cpp
void HeaterOff();
void HeaterOn();

#include "ControlInterface.h"
// void SetTargetControlTemperature(float target); //simple as can be..


struct temperature_entry{
    __UINT16_TYPE__ time; //time in seconds, will be counted using 1kHz timer and 32bit uint...
    __INT16_TYPE__ temperature; // -32768 <-> 32767 => 0.00 <-> 327.67 degrees, t < 0 = end of roast and temp = 0.00. could use negative values as codes for specific operations though.
};

temperature_entry default_profile[4] = 
{
    temperature_entry{0,19000},
    temperature_entry{300,21000},
    temperature_entry{600,22000},
    temperature_entry{840,-1}
}; //hardcoded,for now ..

//temperature_entry* user_profile; // not yet implemented, needs commands to edit and select..

//optimally a control instance for the heater would be passed to this object
class roast_profile_manager{
public :
    //constructor
    roast_profile_manager(temperature_entry* profile) : profile(profile)
    {

    } 

    void start(){
        if (!running){
            index = 0;
            time = 0;
            running = true;
            for (int i = 0; i < 4; i++)
            {
                Serial.print(i);
                Serial.write(": ");
                Serial.print(profile[i].time);
                Serial.write(" ");
                Serial.println(profile[i].temperature);
            }
            SetTargetControlTemperature(0);
            HeaterOn();
        }
    }
    void stop(){
        HeaterOff();
        SetTargetControlTemperature(0);
        running = false;

    }

    void update() //per second
    {

         Serial.println("Update()");
        if (running){
            Serial.println("IsRunning");
            if (time >= profile[index].time){
                Serial.write("Time: ");
                Serial.println(time);
                if (profile[index].temperature < 0){
                    stop();
                }
                else
                {
                    float temp = ((float)profile[index].temperature)/100.0;
                    SetTargetControlTemperature(temp);
                    Serial.write("Auto set temp: ");
                    Serial.println(temp);
                    index++; //no bounds check here.. :/
                }
            }
        }
        time++;

    }


private:
    temperature_entry* profile;
    __UINT16_TYPE__ index  = 0;
    __UINT16_TYPE__ time = 0;
    bool running = false;

};


