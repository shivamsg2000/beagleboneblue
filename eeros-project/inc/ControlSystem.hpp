#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include "customBlocks/Controller.hpp"

using namespace eeros::control;
class ControlSystem
{
    public:
    ControlSystem(double dt);
    
    // Define Blocks
    PeripheralInput<> encoder;
    Controller<> controller;
    PeripheralOutput<> motor;
    eeros::hal::HAL& hal = eeros::hal::HAL::instance();
    // PeripheralInput<bool> buttonMode;
    
    // eeros::hal::Input<bool>* buttonMode;

    TimeDomain timedomain;

};

#endif // CONTROLSYSTEM_HPP