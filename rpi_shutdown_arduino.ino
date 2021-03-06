
//digital input pins
int ign_input_pin = 0;
int rpi_state_pin = 2;

//digital output pins
int power_out_pin = 1;

//digital input states
int ign_state = 0;
int rpi_state = 0;

//digital output state;
int power_out_state = 0;

//analog input state
int system_state = 0;

//misc
unsigned long sys_on_timer = millis();
unsigned long ignition_timer = millis();
int prev_rpi_state;

void setup() {
  //setup digital outputs and inputs
  pinMode(power_out_pin, OUTPUT);   //POWER_OUTPUT
  pinMode(ign_input_pin, INPUT);  //IGNITION_INPUT
}

void loop() {

  //read digital inputs
  ign_state = !digitalRead(ign_input_pin);
  rpi_state = digitalRead(rpi_state_pin);

  //check rpi heartbeat
  if (rpi_state != prev_rpi_state) {
    prev_rpi_state = rpi_state;
    sys_on_timer = millis(); //if system is considered on, then keep resetting timer
  }

  //rpi state debounce
  if (((millis() - sys_on_timer) >= 3000) && (ign_state == 0)) {  //ign off and heartbeat lost?
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
