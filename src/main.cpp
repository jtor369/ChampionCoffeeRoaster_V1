#include <Arduino.h>


#include "ControlInterface.h"
#include "AvgToPM.h"
#include "Filter.h"


AvgToPM heater_power(50);
DigitalIntegrator control_filter;

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
  pinMode(heater_control,OUTPUT);
  DisableHeating();
  Deactivate_Heater();
  Init_MCP9600();
  control_filter.SetGain(0.1);
  ControlEnvironmentTemperature();
  Serial.begin(115200);
  //Serial.write("Hello World!\r\n");
  setup_timer();
}



char rxBuffer[80];
uint16_t rxIndex = 0;

char processBuffer[80];


void ProcessCommand(){

  if (rxBuffer[0] == 'A'){
  //case 'A': //Set target temperature [A,999.99] = 999.99 C
      for (int i = 0; i < 6; i++){
        processBuffer[i] = rxBuffer[i+2];

      }
        processBuffer[6] = '\0';
        float t = atof(processBuffer);
        SetTargetControlTemperature(t);
  }
 else if (rxBuffer[0] == 'B'){

if (rxBuffer[2] == '0'){

          DisableHeating();
          Deactivate_Heater();
              Serial.println("Heating disabled!");

} else if (rxBuffer[2] == '1'){

          EnableHeating();
              Serial.println("Heating enabled!");

      }

  }


}
/*
  if ()
  {

  }
*/


void ProcessCommunication(char ch){
  if (ch != '\r'){
    if (ch != '\n'){
      rxBuffer[rxIndex++] = ch;
    }
  }else
  {
    rxBuffer[rxIndex] = '\0';
    ProcessCommand();
    rxIndex = 0;
  }

}

bool heater_activation = false;

char oBuf[80];

float scT = 0.4;

void loop() {
  if (Serial.available()){
    ProcessCommunication(Serial.read());
  }
  // put your main code here, to run repeatedly:
  if (_10Sps_tick)
  {
    _10Sps_tick = false;
    RefreshTemperatures();
    //note, target should always be environment, if bean target is considered it must be an extra loop controlling environment(Max 250C)...
    if (ReadHeatingEnabled()){
      float f = control_filter.ProcessSample( GetTargetControlTemperature() - GetControlTemperature() );
      Serial.print("PID: ");
      Serial.print(f);
      Serial.print(" ");
      Serial.println(f*scT);
    SetHeatingPower( f*scT ); 
    control_filter.CorrectY0(GetHeatingPower());
    control_filter.ProcessDelayLine();
    }else
    {
          SetHeatingPower(0.0 );
          control_filter.ProcessSample( 0 ); 
          control_filter.ProcessDelayLine();

    }

    Serial.print("T: ");
    Serial.print(GetTargetControlTemperature());
    Serial.print(", E: ");
    Serial.print(ReadEnvironmentMeasured());
    Serial.print(", B: ");
    Serial.print(ReadBeanMeasured());
    Serial.print(", P: ");
    Serial.println(GetHeatingPower());

    //sprintf(oBuf,"T: %f, E: %f, B: %f, P: %f\r",(double)GetTargetControlTemperature(), (double)ReadEnvironmentMeasured(), (double)ReadBeanMeasured(), (double)GetHeatingPower());
    //Serial.write(oBuf);
  }

  if ( _50Sps_tick )
  {
    _50Sps_tick = false;

    if ( ReadHeatingEnabled() )
    {
      heater_activation = heater_power.GetSignal( GetHeatingPower() );
      heater_activation ? Activate_Heater() : Deactivate_Heater();
    }
    else
    {
      heater_activation = heater_power.GetSignal(0.0); //update average
      Deactivate_Heater();
    }
  }

}