#include "ControlSystem.hpp"
#include <eeros/hal/HAL.hpp>
#include <eeros/hal/Input.hpp>
#include <eeros/hal/Output.hpp>

ControlSystem::ControlSystem(double dt)
    : encoder("enc1"),
      motor("motor1"),
      timedomain("Main time domain", dt, true)
{
    // Name all blocks
    encoder.setName("Encoder");
    controller.setName("Controller");
    motor.setName("Motor");

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

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);

}