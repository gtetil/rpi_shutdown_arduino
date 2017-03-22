//digital input pins
int ign_input_pin = 0;

//digital output pins
int power_out_pin = 1;

//analog input pins
int current_sense_pin = A1;

//digital input states
int ign_state = 0;

//digital output state;
int power_out_state = 0;

//analog input state
int system_state = 0;

//misc
unsigned long sys_on_timer = millis();
unsigned long ignition_timer = millis();

void setup() {
  //setup digital outputs and inputs
  pinMode(power_out_pin, OUTPUT);   //POWER_OUTPUT
  pinMode(ign_input_pin, INPUT);  //IGNITION_INPUT
}

void loop() {

  //read analog inputs
  if (analogRead(current_sense_pin) > 6) {  //if greater than 6 (20mA), then system is "on"
    system_state = 1;
  }
  else {
    system_state = 0;
  }

  //read digital inputs
  ign_state = !digitalRead(ign_input_pin);

  //check rpi state
  if (system_state == 1) {
    sys_on_timer = millis(); //if system is considered on, then keep resetting timer
  }

  //rpi state debounce
  if (((millis() - sys_on_timer) >= 500) && (ign_state == 0)) {  //system "off" for more than 500ms?
    digitalOutput("power_out", 0); //turn off rpi power
  }

  //check ignition state
  if (ign_state == 0) {
    ignition_timer = millis(); //if ignition is off, then keep resetting timer
  }

  //ignition debounce
  if (((millis() - ignition_timer) >= 500) && (power_out_state == 0)) {
    digitalOutput("power_out", 1); //turn on rpi power
  }    
}

void digitalOutput(String tag, int state) {
  if (tag == "power_out") {
    power_out_state = state;
    digitalWrite(power_out_pin, state);
  }
}
