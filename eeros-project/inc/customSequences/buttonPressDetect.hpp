#ifndef CUSTOMSEQUENCETEMPLATE_HPP_
#define CUSTOMSEQUENCETEMPLATE_HPP_


#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>

class buttonPressDetector : public eeros::sequencer::Sequence
{
public:
buttonPressDetector(std::string name, eeros::sequencer::Sequence *caller)
        : eeros::sequencer::Sequence(name, caller, true)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        // do something
        return 0;
    }

private:
    // Define steps, sub-sequences, variables, conditions, monitors, exception sequences, ...
};

#endif // CUSTOMSEQUENCETEMPLATE_HPP_
