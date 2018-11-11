#ifndef RPI_DNP3_GPIO_CONFIG_H
#define RPI_DNP3_GPIO_CONFIG_H

#include <cstdint>
#include <vector>
#include <set>

#include <opendnp3/outstation/OutstationStackConfig.h>

class Config
{
    typedef std::vector<uint16_t> assignvec;
   
public:

    Config() : sample_period_ms(100)
    {}

    bool AddInput(uint16_t gpiopin);
    bool AddOutput(uint8_t gpiopin);

    std::vector<uint16_t> inputs;
    std::vector<uint8_t> outputs;

    opendnp3::OutstationStackConfig stack;
    int sample_period_ms;

private:

    bool Add(uint16_t gpiopin, assignvec& vec);

    std::set<uint16_t> assigned;
};

#endif //RPI_DNP3_GPIO_CONFIG_H
