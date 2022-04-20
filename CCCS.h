#include "Simulator.h"

class CCCS : public Device
{
    public:
    
    CCCS(int nodem, int noden, int nodep, int nodeq, double gain);
   
    void Init();
    void Step(double t, double dt);
    // by default DC() will call Step, which is correct for this device
   
    int nodem;
    int noden;
    int nodep;
    int nodeq;
    int nodez;
    double gain;
};

CCCS::CCCS(int nodem, int noden, int nodep, int nodeq, double gain)
{
    this->nodem = nodem;
    this->noden = noden;
    this->nodep = nodep;
    this->nodeq = nodeq;
    this->gain = gain;
}

void CCCS::Init()
{
    // this is where we get and store the index of the internal node:
    nodez = GetNextNode();
}

void CCCS::Step(double t, double dt)
{
    AddJacobian(nodem, nodez, 1);
    AddJacobian(noden, nodez, -1);
    AddJacobian(nodep, nodez, gain);
    AddJacobian(nodeq, nodez, -gain);
    AddJacobian(nodez, nodem, 1);
    AddJacobian(nodez, noden, -1);
}
