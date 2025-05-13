#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/hal/Input.hpp>
#include <eeros/hal/Output.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent doSystemOn; // slSystemOff to slSystemOn 1
    eeros::safety::SafetyEvent doSystemOff; // slSystemOn to slSystemOff 2
    eeros::safety::SafetyEvent doMotorOn; // slSystemOn to slMotorOn 3
    eeros::safety::SafetyEvent doMotorOff; // slMotorOn to slSystemOn 4
    eeros::safety::SafetyEvent evStartMotion; // slMotorOn to slMoving 5
    eeros::safety::SafetyEvent evStopMotion; // slMoving to slMotorOn 6
    eeros::safety::SafetyEvent evSetEmergency; // (slMoving to slEmergency) or (slMotorOn to slEmergency) 7 
    eeros::safety::SafetyEvent evUndoEmergency; // slEmergency to slMoving 8
    eeros::safety::SafetyEvent evEmergencyError; // slEmergency to slError 9
    eeros::safety::SafetyEvent evErrorReset; // slError to slSystemOn 10
    eeros::safety::SafetyEvent evErrorRecovery; // slError to slMotorOn 11
    eeros::safety::SafetyEvent evMovingError; // slMoving to slError 12
    eeros::safety::SafetyEvent evMotionStartError; // slMotorOn to slError 13
    
    

    // Defina all possible levels
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slSystemOn; // system on but motor off: combined level slMotorOff
    eeros::safety::SafetyLevel slMotorOn; // Idle state: motor on but not moving
    // eeros::safety::SafetyLevel slMotorOff; // System on but motor off
    eeros::safety::SafetyLevel slMoving; // Motor on and moving
    eeros::safety::SafetyLevel slError; // Error during, before or after moving, when motor on or off
    eeros::safety::SafetyLevel slEmergency; //Emergency during moving



private:
    // Define all critical outputs
    eeros::hal::Output<bool>* stationLED; // LED2
    eeros::hal::Output<bool>* moveLED; // LED1

    // Define all critical inputs
    eeros::hal::Input<bool>* eStopButton; // BUTTON2
    eeros::hal::Input<bool>* operateButton; // BUTTON1

    ControlSystem &cs;
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
