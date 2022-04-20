
#include "Simulator.h"


class VoltageSource : public Device
{
    public:

    // Constructor (Va, f, and phi are optional arguments):

    VoltageSource(int nodei, int nodej, double Vdc, double Va=0.0, double f=0.0, double phi=0.0);

    // Device interface:
    
    void Init();
    void DC();
    void Step(double t, double dt);

    // Viewable functions:
    
    double GetVoltage();
    double GetCurrent();
    double GetPower();

    // Member Variables:
    
    int nodei;
    int nodej;
    int nodep;
    double Vdc;
    double Va;
    double f;
    double phi;

};


VoltageSource::VoltageSource(int nodei, int nodej, double Vdc, double Va, double f, double phi)
{
    this->nodei = nodei;
    this->nodej = nodej;
    this->Vdc = Vdc;
    this->Va = Va;
    this->f = f;
    this->phi = phi;
}

void VoltageSource::Init()
{
    // this is where and how to setup internal nodes:
    this->nodep = GetNextNode();
}

void VoltageSource::Step(double t, double dt)
{
    double v = Vdc + Va * sin(2.0 * M_PI * f * t + phi);
    
    AddJacobian(nodei, nodep, 1.0);
    AddJacobian(nodej, nodep, -1.0);
    AddJacobian(nodep, nodei, 1.0);
    AddJacobian(nodep, nodej, -1.0);
    AddBEquivalent(nodep, v);
}

void VoltageSource::DC()
{
    AddJacobian(nodei, nodep, -1.0);
    AddJacobian(nodej, nodep, 1.0);
    AddJacobian(nodep, nodei, -1.0);
    AddJacobian(nodep, nodej, 1.0);
    AddBEquivalent(nodep, Vdc);
}

double VoltageSource::GetVoltage()
{
    return GetStateDifference(nodei, nodej);
}

double VoltageSource::GetCurrent()
{
    return -GetState(nodep);
}

double VoltageSource::GetPower()
{
    return GetVoltage() * GetCurrent();
}
