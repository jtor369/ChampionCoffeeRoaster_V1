#include <Arduino.h>


#include "Seeed_MCP9600_interface.h"

const uint8_t addr_beans = 0x60, addr_environment = 0x67;
MCP9600 BeanTemperature(addr_beans), EnvironmentTemperature(addr_environment);


/*! \var float bean_temperature_target
    \brief Contains the last commanded desired bean temperature
*/
volatile float bean_temperature_target = 0.0;

/*! \var float bean_temperature_measured
    \brief Contains the last measured bean temperature
*/
volatile float bean_temperature_measured = 0.0;

/*! \var float environment_temperature_target
    \brief Contains the last commanded desired environment temperature
*/
volatile float environment_temperature_target = 0.0;

/*! \var float environment_temperature_measured
    \brief Contains the last measured environment temperature
*/
volatile float environment_temperature_measured = 0.0;

/*! \var float heating_power
    \brief 
*/
volatile float heating_power = 0.0;

/*! \var bool heating_is_on
    \brief 
*/
volatile bool heating_is_on = false;



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

ISR(TIMER0_COMPA_vect)
{
}


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}