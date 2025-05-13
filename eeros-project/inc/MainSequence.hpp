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

          sleep("", this)
    {
        log.info() << "Sequence created: " << name;
        // sleep.somethingToChangeLogLevel(ERROR or WARN)
        
    }

    int action()
    {
        // bool last = false;
        // int pressCnt = 0;

        while (eeros::sequencer::Sequencer::running) {

            // bool current = cs.buttonMode.getOut().getSignal().getValue();
            // log.info() << current;
            // if (current != last) {
            //     pressCnt ++;
            //     log.info() << "ButtonMode pressed " << pressCnt;

            //     if (ss.getCurrentLevel() == sp.slSystemOn) {
            //         ss.triggerEvent(sp.doMotorOn);
            //     }
            //     else if (ss.getCurrentLevel() == sp.slMotorOn) {
            //         ss.triggerEvent(sp.doMotorOff);
            //     }
            // }

            // sleep(0.05);  // 50 ms debounce delay
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