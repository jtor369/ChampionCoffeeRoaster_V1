#include <Arduino.h>

#include "ControlInterface.h"
#include "AvgToPM.h"


AvgToPM heater_power(50);

/*! \fn void setup_timer()
    \brief Initializes TIMER 0 for interrupt frequency 1000 Hz
*/
void setup_timer()
{
    // TIMER 0 for interrupt frequency 1000 Hz:
  cli(); // stop interrupts
  TCCR0A = 0; // set entire TCCR0A register to 0
  TCCR0B = 0; // same for TCCR0B
  TCNT0  = 0; // initialize counter value to 0
  // set compare match register for 1000 Hz increments
  OCR0A = 249; // = 16000000 / (64 * 1000) - 1 (must be <256)
  // turn on CTC mode
  TCCR0B |= (1 << WGM01);
  // Set CS02, CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  sei(); // allow interrupts

}


uint16_t Timer0_50Sps_counter = 0;
const uint16_t Timer0_50Sps_top = 20;
bool _50Sps_tick = false;

uint16_t Timer0_10Sps_counter = 0;
const uint16_t Timer0_10Sps_top = 100;
bool _10Sps_tick = false;

ISR(TIMER0_COMPA_vect)
{
  Timer0_50Sps_counter++;
  Timer0_10Sps_counter++;

  if (Timer0_50Sps_counter >= Timer0_50Sps_top)
  {
    Timer0_50Sps_counter = 0;
    _50Sps_tick = true;
  }

  if (Timer0_10Sps_counter >= Timer0_10Sps_top)
  {
    Timer0_10Sps_counter = 0;
    _10Sps_tick = true;
  }

}

const uint8_t heater_control = 2;
void Activate_Heater(){
digitalWrite(heater_control,1);
}

void Deactivate_Heater(){
digitalWrite(heater_control,0);
}

void setup() {
  // put your setup code here, to run once:
}

bool heater_activation = false;

void loop() {
  // put your main code here, to run repeatedly:
  if (_50Sps_tick){
    _50Sps_tick = false;
    

    if (ReadHeatingEnabled()){
      heater_activation = heater_power.GetSignal(GetHeatingPower());
      heater_activation ? Activate_Heater() : Deactivate_Heater();
    }else{
      heater_activation = heater_power.GetSignal(0.0); //update average
      Deactivate_Heater();
    }
  }
}