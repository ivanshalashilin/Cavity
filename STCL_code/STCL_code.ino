// *****************************************************************
// Transfer cavity lock algorithm 
// Written in C++ for the Arduino Due
// Last Updated: 23/04/2024
// Ivan Shalashilin and Jacob J. J. Edginton
// *****************************************************************

int interrupt_pin_1 = 30; // pin for rising edge interrupt
int interrupt_pin_2 = 26; // pin for falling edge interrupt
int i= 0; // initialize counter

int master_digital_input = 50; // pin for  Master input
int slave_1_digital_input = 46; //pin for Slave 1 input (Boba Fett) 
int print_precision = 8;

// initialize varaibles
unsigned long time_master_1;
unsigned long time_master_2;
unsigned long time_slave_1;
unsigned long time_TTL_high;
unsigned long time_TTL_low;

double time_difference_slave_1_master_1;
double time_difference_master_2_master_1;

double time_start_trigger;
double time_end_trigger;
double time_delay;

//variables for PI loop
unsigned long time_previous = 0;
unsigned long time_pi_loop;
unsigned long time_now;
double time_ratio;
double delta_r;

double error_laser;
double last_error_laser = 0;
double integral_error_laser;
double derivative_error_laser;
double output_laser;

// For laser feedback: The user must fine-tune the PID values
double kp_laser = 700, ki_laser = 0.0003, kd_laser = 0;

double setpoint_laser = 0.5;
double cavity_setpoint = 0.25;

double error_offset;
double last_error_offset = 0;
double integral_error_offset;
double derivative_error_offset;
double output_offset;
// For cavity offset feedback: The user must fine-tune the PID values
double kp_offset = -1000, ki_offset = -0.001, kd_offset = 0000;

float offset_to_drift_laser = 800;

int putty_log_counter = 0;

float offset_to_cavity = 2800;
// Boolean variables changed in hardware interrupts must be volatile
volatile bool on_ttl_high = false;
volatile bool master_1_found = false;
volatile bool master_2_found = false;
volatile bool slave_1_found = false;

bool offset_setpoint_found = false;


double time_now_offset;
double time_pi_loop_offset;

double time_previous_offset = 0;

void setup() {
  analogWrite(DAC0, offset_to_drift_laser);
  Serial.begin(19200);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin_1),
  enable_input, RISING); // When rising turn ON master & slave pins
  attachInterrupt(digitalPinToInterrupt(interrupt_pin_2),
  disable_input, FALLING); // When falling turn OFF master & slave pins
  pinMode(master_digital_input, INPUT);
  pinMode(slave_1_digital_input, INPUT);

  analogWriteResolution(12);


  analogWrite(DAC1, offset_to_cavity);
  delay(2000);

  time_previous = micros();
  int i = 0;
}

void enable_input(){
  on_ttl_high = true;
}

void disable_input(){
  on_ttl_high = false;
  master_1_found = false;
  slave_1_found = false;
  master_2_found = false;

}

void loop() {

  if (on_ttl_high){   
    time_TTL_high = micros();
    while(!master_1_found){
    int pinstate_master_1 = digitalRead(master_digital_input);
    if(!on_ttl_high){
      return; \\ terminate loop if downramp detected
    }
    if (pinstate_master_1 == HIGH){
      time_master_1 = micros();
      master_1_found = true;

    }

    }

    while(!slave_1_found){
    int pinstate_slave_1 = digitalRead(slave_1_digital_input);
    if(!on_ttl_high){
      return; \\ terminate loop if downramp detected
    }
    if (pinstate_slave_1 == HIGH){
      time_slave_1 = micros();
      slave_1_found = true;
    } 
    }

    while(!master_2_found){
    if(!on_ttl_high){
      return; \\ terminate loop if downramp detected
    }
    int pinstate_master_2 = digitalRead(master_digital_input);
    if (pinstate_master_2 == HIGH){
      //Serial.println("finding master 2...");
      // Serial.println(i);
      time_master_2 = micros();
      master_2_found = true;
    }
  } 

  time_difference_master_2_master_1 = time_master_2-time_master_1;
  time_difference_slave_1_master_1 = time_slave_1-time_master_1;

  //How long since we last calculated
  time_now = micros();
  time_pi_loop = time_now - time_previous;
  //Compute all the working error variables
  time_ratio = time_difference_slave_1_master_1
  /time_difference_master_2_master_1;

  error_laser = time_ratio-setpoint_laser; //normalised error
  integral_error_laser += (error_laser * time_pi_loop);
  derivative_error_laser = (error_laser - last_error_laser) 
  / time_pi_loop;
  output_laser = (kp_laser * error_laser + ki_laser 
  * integral_error_laser 
  + kd_laser * derivative_error_laser);

  while (on_ttl_high){
  }

  double time_TTL_low = micros();

  time_now_offset = micros();
  time_pi_loop_offset = time_now_offset - time_previous_offset;

  time_delay = (time_master_1-time_TTL_high)
  /(time_TTL_low-time_TTL_high);

  error_offset = cavity_setpoint-time_delay;

  integral_error_offset += (error_offset * time_pi_loop);
  derivative_error_offset = (error_offset - last_error_offset) 
  / time_pi_loop;
  output_offset = (kp_offset * error_offset + ki_offset 
  * integral_error_offset 
  + kd_offset * derivative_error_offset);

  if (i > putty_log_counter){
  analogWrite(DAC0, offset_to_drift_laser+output_laser);
  analogWrite(DAC1, output_offset+offset_to_cavity);
  }
  i ++; 

  time_previous = time_now;
  last_error_laser = error_laser;
  last_error_offset = error_offset;

  master_1_found = false;
  slave_1_found = false;
  master_2_found = false;
  on_ttl_high = false; 
}
}