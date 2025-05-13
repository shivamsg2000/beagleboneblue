#include "MyRobotSafetyProperties.hpp"
#include <eeros/logger/StreamLogWriter.hpp>

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
      
    // Levels
      slSystemOff("Offline"),
      slSystemOn("Online"),
      slMotorOn("Idling"),
      slMoving("Moving"),
      slError("Error"),
      slEmergency("Emergency"),
    
    // Events
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
    stationLED   = hal.getLogicOutput("onBoardLEDgreen");
    moveLED   = hal.getLogicOutput("onBoardLEDred");
    
    // criticalOutputs = { ... };
    
    // Declare and add critical inputs
    // ... = eeros::hal::HAL::instance().getLogicInput("...", ...);
    eStopButton   = hal.getLogicInput ("onBoardButtonPause");
    operateButton    = hal.getLogicInput ("onBoardButtonMode");

    // criticalInputs = { ... };

    // Add all safety levels to the safety system
    addLevel(slSystemOff);
    addLevel(slSystemOn);
    addLevel(slMotorOn);
    addLevel(slMoving);
    addLevel(slError);
    addLevel(slEmergency);

    // Add events to individual safety levels
    slSystemOff.addEvent(doSystemOn, slSystemOn, kPublicEvent);  // slSystemOff -> slSystemOn 1
    slSystemOn.addEvent(doSystemOff, slSystemOff, kPublicEvent);  // slSystemOn -> slSystemOff 2
    slSystemOn.addEvent(doMotorOn, slMotorOn, kPublicEvent);  // slSystemOn -> slMotorOn 3
    slMotorOn.addEvent(doMotorOff, slSystemOn, kPublicEvent);  // slMotorOn -> slSystemOn 4
    slMotorOn.addEvent(doSystemOff, slSystemOff, kPublicEvent);  // slMotorOn -> slSystemOff 4
    slMotorOn.addEvent(evStartMotion, slMoving, kPublicEvent);  // slMotorOn -> slMoving 5
    slMoving.addEvent(evStopMotion, slMotorOn, kPublicEvent);  // slMoving -> slMotorOn 6
    slMoving.addEvent(evSetEmergency, slEmergency, kPublicEvent);  // (slMoving -> slEmergency) 7
    slMotorOn.addEvent(evSetEmergency, slEmergency, kPublicEvent); // (slMotorOn -> slEmergency) 7
    slEmergency.addEvent(evUndoEmergency, slMoving, kPublicEvent);  // slEmergency -> slMoving 8
    slEmergency.addEvent(evEmergencyError, slError, kPublicEvent);  // slEmergency -> slError 9
    slError.addEvent(evErrorReset, slSystemOn, kPublicEvent);  // slError -> slSystemOn 10
    slError.addEvent(evErrorRecovery, slMotorOn, kPublicEvent);  // slError -> slMotorOn 11
    slMoving.addEvent(evMovingError, slError, kPublicEvent);  // slMoving -> slError 12
    slMotorOn.addEvent(evMotionStartError, slError, kPublicEvent);  // slMotorOn -> slError 13

    auto log = eeros::logger::Logger::getLogger();



    // Add events to multiple safety levels
    // addEventToAllLevelsBetween(lowerLevel, upperLevel, event, targetLevel, kPublicEvent/kPrivateEvent);

    // Define input actions for all levels
    // Doing it the following way, when button is pressed once, the state falls through from slSystemOff to slMoving at once, since button press action is
    // much slower than the detection of button states.
    // slSystemOff.setInputActions({ check(operateButton, false, doSystemOn), ignore(eStopButton)});
    // slSystemOn.setInputActions({ check(operateButton, false, doMotorOn), check(eStopButton, false, doSystemOff)});
    // slMotorOn.setInputActions({ check(operateButton, false, evStartMotion), check(eStopButton, false, doMotorOff)});
    // slMoving.setInputActions({ ignore(operateButton), check(eStopButton, false, evStopMotion)});

    slSystemOff.setInputActions({ check(operateButton, false, doSystemOn), ignore(eStopButton)}); // slSystemOn via pressing operateButton
    slSystemOn.setInputActions({ ignore(operateButton), check(eStopButton, false, doSystemOff)}); // no state transitions via buttons
    slMotorOn.setInputActions({ check(operateButton, false, evStartMotion), ignore(eStopButton)}); // slMoving via pressing operateButton
    slMoving.setInputActions({ ignore(operateButton), check(eStopButton, false, evSetEmergency)}); //slEmergency via pressing eStopButton
    slEmergency.setInputActions({ check(operateButton, false, evUndoEmergency), ignore(eStopButton)}); //slMoving via pressing operateButton
    slError.setInputActions({ check(operateButton, false, evErrorReset), ignore(eStopButton)}); //slSystemOn via pressing operateButton

    // Define output actions for all levels
    slSystemOff.setOutputActions({ set(moveLED, false), set(stationLED, false)});
    slSystemOn.setOutputActions({set(moveLED, false), set(stationLED, true)});
    slMotorOn.setOutputActions({set(moveLED, true), set(stationLED, true)});
    slMoving.setOutputActions({set(moveLED, true), set(stationLED, true)});
    slError.setOutputActions({set(moveLED, false), set(stationLED, false)});
    slEmergency.setOutputActions({set(moveLED, false), set(stationLED, false)});

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        // cs.timedomain.stop();
        // eeros::Executor::stop();
    });

    slSystemOn.setLevelAction([&, dt](SafetyContext *privateContext) {
        // cs.timedomain.start();
        // log.info() << "Motor power Off";
        if (slSystemOn.getNofActivations() * dt >= 5 ) {
            privateContext->triggerEvent(doMotorOn);
        }
    });

    slMotorOn.setLevelAction([&, dt](SafetyContext *privateContext) {
        // Start PWM output, enable motors
        // Example: motorEnable->set(true);
        // log.info() << "Encoder val: " << cs.encoder.getOut().getSignal();
        if (slMotorOn.getNofActivations() * dt >= 5 ) {
            privateContext->triggerEvent(doSystemOff);
        }
    });
    
    slMoving.setLevelAction([&](SafetyContext *privateContext) {
        // Robot is moving; keep everything running
        // Could monitor motion status here
    });
    
    slEmergency.setLevelAction([&, dt](SafetyContext *privateContext) {
        // cs.timedomain.stop();
        // Disable motors immediately
        // Example: motorEnable->set(false);
        // Log or blink LED
        if (slEmergency.getNofActivations() * dt >= 5 ) {
            privateContext->triggerEvent(evEmergencyError);
        }
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
