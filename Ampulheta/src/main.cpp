#include <Arduino.h>

#define LED1_pin 9
#define LED2_pin 10
#define LED3_pin 11
#define LED4_pin 12
#define LED5_pin 13
#define LED6_pin 14
#define LED7_pin 15

#define S1_pin 1


typedef struct {
  int state, new_state;

  // tes - time entering state
  // tis - time in state
  unsigned long tes, tis;
} fsm_t;


//
int ascend;

// Input variables
uint8_t S1, prevS1;

// Output variables
uint8_t LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7;

// Our finite state machines
fsm_t fsm1, fsm2;

unsigned long interval, last_cycle;
unsigned long loop_micros;

// Set new state
void set_state(fsm_t& fsm, int new_state)
{
  if (fsm.state != new_state) {  // if the state chnanged tis is reset
    fsm.state = new_state;
    fsm.tes = millis();
    fsm.tis = 0;
  }
}


void setup() 
{
  pinMode(LED1_pin, OUTPUT);
  pinMode(LED2_pin, OUTPUT);
  pinMode(LED3_pin, OUTPUT);
  pinMode(LED4_pin, OUTPUT);
  pinMode(LED5_pin, OUTPUT);
  pinMode(LED6_pin, OUTPUT);
  pinMode(LED7_pin, OUTPUT);

  pinMode(S1_pin, INPUT_PULLUP); // Attention to pull up if not using pull up
  
  // Start the serial port with 115200 baudrate
  Serial.begin(115200);

  interval = 10;
  ascend = 0;
  set_state(fsm1, 0);
    
}

void loop() 
{
    // To measure the time between loop() calls
    //unsigned long last_loop_micros = loop_micros; 
    
    // Do this only every "interval" miliseconds 
    // It helps to clear the switches bounce effect
    unsigned long now = millis();
    if (now - last_cycle > interval) {
      loop_micros = micros();
      last_cycle = now;
      
      // Read the inputs
      prevS1 = S1;
      
      S1 = !digitalRead(S1_pin);
      

      // FSM processing

      // Update tis for all state machines
      unsigned long cur_time = millis();   // Just one call to millis()
      fsm1.tis = cur_time - fsm1.tes;
      
      
      if (prevS1 == 0 && S1) {
        if (ascend) {
          ascend = 0;
      } else {
        ascend = 1;
      }
      }
      if (ascend) {
        if (fsm1.state == 0 && fsm1.tis > 1000) {
          fsm1.new_state = 1;
        }
        if (fsm1.state == 1 && fsm1.tis > 1000) {
          fsm1.new_state = 2;
        } else if (fsm1.state == 2 && fsm1.tis > 1000) {
          fsm1.new_state = 3;
        } else if (fsm1.state == 3 && fsm1.tis > 1000) {
          fsm1.new_state = 4;
        } else if (fsm1.state == 4 && fsm1.tis > 1000) {
          fsm1.new_state = 5;
        } else if (fsm1.state == 5 && fsm1.tis > 1000) {
          fsm1.new_state = 6;
        } else if (fsm1.state == 6 && fsm1.tis > 1000) {
          fsm1.new_state = 7;
        }
      } else {
        if (fsm1.state == 7 && fsm1.tis > 1000) {
          fsm1.new_state = 6;
        } else if (fsm1.state == 6 && fsm1.tis > 1000) {
          fsm1.new_state = 5;
        } else if (fsm1.state == 5 && fsm1.tis > 1000) {
          fsm1.new_state = 4;
        } else if (fsm1.state == 4 && fsm1.tis > 1000) {
          fsm1.new_state = 3;
        } else if (fsm1.state == 3 && fsm1.tis > 1000) {
          fsm1.new_state = 2;
        } else if (fsm1.state == 2 && fsm1.tis > 1000) {
          fsm1.new_state = 1;
        } else if (fsm1.state == 1 && fsm1.tis > 1000) {
          fsm1.new_state = 0;
        }
      }

      set_state(fsm1, fsm1.new_state); 
      
      LED_1 = (fsm1.state >= 1);
      LED_2 = (fsm1.state >= 2);
      LED_3 = (fsm1.state >= 3);
      LED_4 = (fsm1.state >= 4);
      LED_5 = (fsm1.state >= 5);
      LED_6 = (fsm1.state >= 6);
      LED_7 = (fsm1.state >= 7);


      // Set the outputs
      digitalWrite(LED1_pin, LED_1);
      digitalWrite(LED2_pin, LED_2);
      digitalWrite(LED3_pin, LED_3);
      digitalWrite(LED4_pin, LED_4);
      digitalWrite(LED5_pin, LED_5);
      digitalWrite(LED6_pin, LED_6);
      digitalWrite(LED7_pin, LED_7);

     
    }
    Serial.print(ascend);
}