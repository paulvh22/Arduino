/*
 * MECOTRON TUTORIAL
 *
 * This is a template to get you started in the course of the tutorial on the
 * control theory platforms, a.k.a. the MECOtrons.
 * The tasks of the tutorial session will guide you through this template and
 * ask you to make use of the platform's capabilities in a step-by-step fashion.
 *
 * Every function in this template comes with an opening comment that describes
 * its purpose and functionality. Please also pay attention to the remarks that
 * are made in comment blocks.
 * Test change
 */
#define Kp  2.0
#define Ki   0.0
#define Kd  0.0
float D = 2.0*0.01;
float A = (2.0*Kp*0.01 + Ki*0.01*0.01 + 4.0*Kd)/D;
float B = (2.0*Ki*0.01*0.01 - 8.0*Kd)/D;

float C = (-2.0*Kp*0.01 + Ki*0.01*0.01 + 4.0*Kd)/D;




#include "robot.h"

bool Robot::init() {
  MECOtron::init(); // Initialize the MECOtron

  // Initializing the robot's specific variables
  for(int k=0; k<2; k++){
    x[k] = 0.0;   // Set all components of the vector (float array) x to 0 as initialization
  }

  return true;
}

void Robot::controllerHook() {
  

  // Compute update of motor voltages if controller is enabled (triggered by
  // pushing Button 1 in QRoboticsCenter)
  if(controlEnabled()) {
    // Fill your control law here to conditionally update the motor voltage...
    LED1(ON);
    LED2(OFF);
    // Update register
    u[2] = u[1];u[1] = u[0]; 
    e[2] = e[1];e[1] = e[0];  
    //setpoint 
    float r = readFloat(7); //radians
    float y = getPositionMotorA(); //radians
    e[0] = r - y;
    u [0] = A*e[0] + B*e[1] + C*e[2] + u[2];
    setVoltageMotorA(u[0]); // Apply 6.0 volts to motor A if the control is enabled
    writeFloat(6, u[0]);
    writeFloat(7, e[0]);
  } else {
    // If the controller is disabled, you might want to do something else...
    LED1(OFF);
    LED2(ON);
    setVoltageMotorA(0.0); // Apply 0.0 volts to motor B if the control is disabled
  }

  float va = getSpeedMotorA();    // Get the wheel speed of motor A (in radians/second)
  x[1] = x[0]; x[0] = va;         // Memorize the last two samples of the speed of motor A (in fact, a shift register)

  int k = readInt(0);     // Read the value you set on QRoboticsCenter's integer channel 0
  writeFloat(0, 0.01);    // Send the value 0.01 to QRoboticsCenter's float channel 0
  writeInt(0, 1);         // Send the value 1 to QRoboticsCenter's integer channel 1

}

bool Robot::controlEnabled() { 
  return _button_states[0];       // The control is enabled if the state of button 0 is true
}

void Robot::button0callback() {
  if(toggleButton(0)) {           // Switches the state of button 0 and checks if the new state is true
    message("Robot enabled.");    // Display a message in the status bar of QRoboticsCenter
  }
  else {
    message("Robot disabled.");
  }
}

void Robot::button1callback() {
  toggleButton(1);  
  init();                         // Reset the MECOtron and reinitialize the Robot object
  message("Reset.");
}
