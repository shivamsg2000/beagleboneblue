#include "ControlSystem.hpp"


ControlSystem::ControlSystem(double dt)
    : encoder("enc1"),
      motor("motor1"),
      timedomain("Main time domain", dt, true)
    //   buttonMode("onBoardButtonMode")
{
    // Name all blocks
    encoder.setName("Encoder");
    controller.setName("Controller");
    motor.setName("Motor");
    // buttonMode.setName("mode button");

    // Name all signals
    encoder.getOut().getSignal().setName("phi [rads]");
    controller.getOut().getSignal().setName("U [V]");

    // Connect signals
    controller.getIn().connect(encoder.getOut());
    motor.getIn().connect(controller.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(encoder);
    timedomain.addBlock(controller);
    timedomain.addBlock(motor);
    // timedomain.addBlock(buttonMode);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);

    // buttonMode = static_cast<eeros::hal::Input<bool>*>(hal.getLogicInput("onBoardButtonMode"));
}