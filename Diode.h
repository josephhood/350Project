#include "Simulator.h"  // has Simulation and Device class


/*

Device Schematic:

    nodepos    nodeneg
        o--->|---o
    
Device I-V formula:

    Vd = V(nodepos) - V(nodeneg)
    Id = I(nodepos) = -I(nodeneg) = Is * (exp(Vd / (mu * Vt)) - 1)

Newton-Raphson formulation:

    x = Vd
    f(x) = Id = Is * (exp(Vd / (mu * Vt)) - 1)
    f'(x) = gd = df(Vd) / dVd = Is / (mu * Vt) * exp(Vd / (mu * Vt))
    
Iteration update equation:

    x[k+1] = x[k] - f(x[k]) / f'(x[k])
    
Cast into 0 = g * x - b form:

    f'(x[k]) * x[k+1] = f'(x[k]) * x[k] - f(x[k])
    0 = f'(x[k]) * x[k+1] - f'(x[k]) * x[k] + f(x[k])
    
    therefore:
    
    gd[k] = f'(x[k]) = Is / (mu * Vt) * exp(Vd[k] / (mu * Vt))
    bd[k] = -(Id[k] - gd[k] * Vdk[k]);
    
    where:
    
    Vd[k] = V(nodepos)[k] - V(nodeneg)[k]
    Id[k] = Is * (exp(Vd[k] / (mu * Vt)) - 1)
    
*/
    
class Diode : public Device // extends Device class
{

    public:
    
    // Constructor:
    Diode(int nodepos, int nodeneg, double Vt=25.85e-3, double Is=1.0e-9, double mu=1.5);
    
    // The Device Interface:
    void Step(double t, double dt);
    // we do not need to implement Init() because there are no internal nodes
    // we do no need to implement DC() because it is the same as Step()
    
    private:
    
    // member variables:
    int nodepos;
    int nodeneg;
    
    double Vt;  // thermal voltage
    double Is;  // Saturation current
    double mu;  // ideality factor
};

Diode::Diode(int nodepos, int nodeneg, double Vt, double Is, double mu)
{
    this->nodepos = nodepos;
    this->nodeneg = nodeneg;
    this->Vt = Vt;
    this->Is = Is;
}

void Diode::Step(double t, double dt)
{

    // get the diode voltage (x(k)) for the previous iteration:
    double Vdk = GetIterationState(nodepos) - GetIterationState(nodeneg);
    
    // calculate the diode current (f(x(k))) for the previous iteration:
    double Idk = Is * (exp(Vdk / Vt) - 1);
    
    // calculate the equivalent (f'(x[k])) conductance:
    double gdk = Is / Vt * exp(Vdk / Vt);
    
    // calculate the equivalent b-equiv current source value:
    double bdk = -(Idk - gdk * Vdk);
    
    AddJacobian(nodepos, nodepos, gdk);
    AddJacobian(nodepos, nodeneg, -gdk);
    AddJacobian(nodeneg, nodepos, -gdk);
    AddJacobian(nodeneg, nodeneg, gdk);
    
    AddBEquivalent(nodepos, bdk);
    AddBEquivalent(nodeneg, -bdk);
}
