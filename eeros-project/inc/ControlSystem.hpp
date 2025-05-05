#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/hal/Input.hpp>
#include <eeros/hal/Output.hpp>

using namespace eeros::control;

class ControlSystem
{
public:
    ControlSystem(double dt);

    // Define Blocks
    Constant<> myConstant;
    Gain<> myGain;

    TimeDomain timedomain;

    eeros::hal::Input<bool>* buttonMode;
    eeros::hal::Output<bool>* led1;
    eeros::hal::Output<bool>* led2;
};

#endif // CONTROLSYSTEM_HPP