
#include "Simulator.h"

class Resistor : public Device
{
    public:

    // Constructor:
    
    Resistor(int nodei, int nodej, double R);

    // Device interface (don't need Init() or DC() functions for resistor):
    
    void Step(double t, double dt);

    // Viewable functions:
    
    double GetVoltage();
    double GetCurrent();
    double GetPower();

    // Member variables:

    int nodei;
    int nodej;
    double R;
    double g;
};

Resistor::Resistor(int nodei, int nodej, double R)
{
    this->nodei = nodei;
    this->nodej = nodej;
    this->R = R;
    g = 1.0 / R;
}

void Resistor::Step(double t, double dt)
{
    AddJacobian(nodei, nodei, g);
    AddJacobian(nodei, nodej, -g);
    AddJacobian(nodej, nodei, -g);
    AddJacobian(nodej, nodej, g);
}

double Resistor::GetVoltage()
{
    return GetStateDifference(nodei, nodej);
}

double Resistor::GetCurrent()
{
    return GetVoltage() * g;
}

double Resistor::GetPower()
{
    return GetVoltage() * GetCurrent();
}
