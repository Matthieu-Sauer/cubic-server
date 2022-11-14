#ifndef CUBICSERVER_OVERWORLD_HPP
#define CUBICSERVER_OVERWORLD_HPP

#include "../Dimension.hpp"

class Overworld : public Dimension
{
public:
    void tick() override;
    void initialize() override;
};


#endif //CUBICSERVER_OVERWORLD_HPP
