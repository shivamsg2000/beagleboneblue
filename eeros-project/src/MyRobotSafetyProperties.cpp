#include "MyRobotSafetyProperties.hpp"

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("Offline"),
      slSystemOn("Online"),
      slMotorOn("Idling"),
      slMoving("Moving"),
      slError("Error"),
      slEmergency("Emergency"),

      doSystemOn("Startup system"),
      doSystemOff("Shutdown system"),
      doMotorOn("Turn On motor"),
      doMotorOff("Turn Off motor"),
      evStartMotion("Start Motion"),
      evStopMotion("Stop motion"),
      evSetEmergency("Set Emergency"),
      evUndoEmergency("Undo Emergency"),
      evEmergencyError("Emergency caused Error"),
      evErrorReset("Error Reset to startup"),
      evErrorRecovery("Error recovered and ready"),
      evMovingError("Error during motion"),
      evMotionStartError("Error before moving")
{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    // ... = hal.getLogicOutput("...");

    // criticalOutputs = { ... };

    // Declare and add critical inputs
    // ... = eeros::hal::HAL::instance().getLogicInput("...", ...);

    // criticalInputs = { ... };

    // Add all safety levels to the safety system
    addLevel(slSystemOff);
    addLevel(slSystemOn);
    addLevel(slMotorOn);
    addLevel(slMoving);
    addLevel(slError);
    addLevel(slEmergency);

    // Add events to individual safety levels
    slSystemOff.addEvent(doSystemOn, slSystemOn, kPublicEvent);
    slSystemOn.addEvent(doSystemOff, slSystemOff, kPublicEvent);
    slSystemOn.addEvent(doMotorOn, slMotorOn, kPublicEvent);
    slMotorOn.addEvent(doMotorOff, slSystemOn, kPublicEvent);
    slMotorOn.addEvent(evStartMotion, slMoving, kPublicEvent);
    slMoving.addEvent(evStopMotion, slMotorOn, kPublicEvent);
    slMoving.addEvent(evSetEmergency, slEmergency, kPublicEvent);
    slMotorOn.addEvent(evSetEmergency, slEmergency, kPublicEvent);
    slEmergency.addEvent(evUndoEmergency, slMoving, kPublicEvent);
    slEmergency.addEvent(evEmergencyError, slError, kPublicEvent);
    slError.addEvent(evErrorReset, slSystemOn, kPublicEvent);
    slError.addEvent(evErrorRecovery, slMotorOn, kPublicEvent);
    slMoving.addEvent(evMovingError, slError, kPublicEvent);
    slMotorOn.addEvent(evMotionStartError, slError, kPublicEvent);



    // Add events to multiple safety levels
    // addEventToAllLevelsBetween(lowerLevel, upperLevel, event, targetLevel, kPublicEvent/kPrivateEvent);

    // Define input actions for all levels
    // level.setInputActions({ ... });

    // Define output actions for all levels
    // level.setOutputActions({ ... });

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        // cs.timedomain.stop();
        eeros::Executor::stop();
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        // cs.timedomain.start();
    });

    slMotorOn.setLevelAction([&](SafetyContext *privateContext) {
        // Start PWM output, enable motors
        // Example: motorEnable->set(true);
    });
    
    slMoving.setLevelAction([&](SafetyContext *privateContext) {
        // Robot is moving; keep everything running
        // Could monitor motion status here
    });
    
    slEmergency.setLevelAction([&](SafetyContext *privateContext) {
        // cs.timedomain.stop();
        // Disable motors immediately
        // Example: motorEnable->set(false);
        // Log or blink LED
    });
    
    slError.setLevelAction([&](SafetyContext *privateContext) {
        // cs.timedomain.stop();
        // Also disable motors, log error
    });

    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSystemOff);
    });
}
