#include "aoa/Factory.hpp"
#include "Pulse.hpp"


AOA &Factory::create(const Factory::Type type) {
    switch (type) {
    case Type::PULSE:
        return (AOA &)Pulse::get();
        break;
    case Type::WASAPI:

        break;
    default:
        break;
    }
}
