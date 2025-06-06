#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "MyRobotSafetyProperties.hpp"
#include "ControlSystem.hpp"
#include <eeros/sequencer/Wait.hpp>

class MainSequence : public eeros::sequencer::Sequence
{
public:
    MainSequence(std::string name, eeros::sequencer::Sequencer &seq,
                 eeros::safety::SafetySystem &ss,
                 MyRobotSafetyProperties &sp, ControlSystem &cs)
        : eeros::sequencer::Sequence(name, seq),
          ss(ss),
          sp(sp),
          cs(cs),

          sleep("Sleep", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        bool last = false;

        while (eeros::sequencer::Sequencer::running) {
            bool current = cs.buttonMode->get(); // what the button is *now*

            if (current && !last) {  // Rising edge detected // This means: last was false, now it's true → just pressed!
                auto currentLevel = ss.getCurrentLevel(); 

                if (currentLevel == sp.slSystemOn) {
                    ss.triggerEvent(sp.doMotorOn);
                } else if (currentLevel == sp.slMotorOn) {
                    ss.triggerEvent(sp.doMotorOff);
                }
            }

            last = current;

            sleep(0.05);  // 50 ms debounce delay
        }

        return 0;
    }

private:
    eeros::safety::SafetySystem &ss;
    ControlSystem &cs;
    MyRobotSafetyProperties &sp;

    eeros::sequencer::Wait sleep;
};

#endif // MAINSEQUENCE_HPP_