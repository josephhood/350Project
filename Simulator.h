#ifndef SIMULATOR_H
#define SIMULATOR_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>

#include "Matrix.h"

using std::vector;
using std::string;
using std::max;
using std::abs;

class Simulator;

/**
Device base class. All devices should derive from this class and implement
at least the Step() function.
**/
class Device
{
    public:

    /////////////// Device Interface ///////////////
    
    /**
    REQUIRED Steps the device for transient analysis.
    Its function is to implement the behavior needed for transient analysis
    (.trans). It takes in the current time (s) and time step (s), and applies
    the stamps to the J and B matrices accordingly.
    @param t is the current simulation time in seconds
    @param dt is the current simulation time step in seconds
    **/
    virtual void Step(double t, double dt);

    /**
    OPTIONAL This function is where you should do any pre-simulation set up.
    Also, you should get and store integer values for internal nodes here with
    the GetNextNode() function (example: nodep = GetNextNode())
    **/
    virtual void Init();

    /**
    OPTIONAL Stamps the DC model for the DC operating point calculation
    (optional, default behavior is to call Step(0, 0)).
    **/
    virtual void DC();

    /**
    OPTIONAL Steps the device to update the simulator signals.
    It takes in the current time (s) and time step (s), and you should get and/or set
    signals her with the GetSignal() and SetSignal() function
    @param t is the current simulation time in seconds
    @param dt is the current simulation time step in seconds
    **/
    virtual void SignalStep(double t, double dt);


    ///////////// End Device Interface /////////////
    
    
    /*
    Wrapper for const double Simulator::GetSignal(const int signal) const
    */
    double GetSignal(const int signal) const;

    /*
    Wrapper for Simulator::SetSignal(const int signal, const double value)
    */
    void SetSignal(const int signal, const double value);
    

    // This function exists to set the parent simulator instance: 
    void SetSim(Simulator& sim);

    protected:

    // reference to the parent simulator object:
    Simulator* sim;

    // utility functions for accessing sim matrices
    // (wrappers around simulator accessors):

    /*
    Wrapper for Simulator::GetTime() const
    */
    double GetTime() const;

    /*
    Wrapper for Simulator::GetTimeStep() const
    */
    double GetTimeStep() const;
    
    /*
    Wrapper for Simulator::GetState(const int i) const
    */
    double GetState(const int i) const;

    /*
    Wrapper for Simulator::GetIterationState(const int i) const
    */
    double GetIterationState(const int i) const;

    /*
    Wrapper for Simulator::GetStateDifference(const int i, const int j) const
    */
    double GetStateDifference(const int i, const int j) const;

    /*
    Wrapper for Simulator::GetIterationStateDifference(const int i, const int j) const
    */
    double GetIterationStateDifference(const int i, const int j) const;

    /*
    Wrapper for Simulator::GetJacobian(const int i, const int j)  const
    */
    double GetJacobian(const int i, const int j) const;

    /*
    Wrapper for Simulator::GetBEquivalent(const int i)  const
    */
    double GetBEquivalent(const int i) const;

    /*
    Allows the addition of internal nodes
    */
    int GetNextNode();

    /*
    Wrapper for Simulator::SetInitialState(const int i, const double value)
    */
    void SetInitialState(const int i, const double value); 

    /*
    Wrapper for Simulator::AddJacobian(const int i, const int j, const double value)
    */
    void AddJacobian(const int i, const int j, const double value); 

    /*
    Wrapper for Simulator::AddBEquivalent(const int i, const double value)
    */
    void AddBEquivalent(const int i, const double value);     

};


/**
Simulation engine class that contains system state, device references, and various
utility functions.
**/
class Simulator
{
    public:

    /**
    Constructor
    @param nodes is the integer number of nodes (including internal, excluding ground)
    **/
    Simulator(const int nodes, const int signals = 0);

    /**
    initializes the simulator for transient analysis.
    @param dt is the time step in seconds
    @param tf is the final simulation time in seconds
    @param solveDCOperatingPoint if true, initializes the states by solving the DC operating point
    **/
    void Init(const double dt=0.0, const double tf=0.0, const int maxIter=100, const double tolerance=1e-6, bool solveDCOperatingPoint=false);

    /**
    Solves the DC operating point of the system.
    **/
    void SolveDC();

    /**
    Adds a device to the simulator.
    @param device: The device to add of base type Device
    **/
    void AddDevice(Device& device);

    /**
    Gets the current simulation time
    @return the current simulation time in seconds
    **/
    double GetTime() const;

    /**
    Gets the current simulation time step
    @returns the current time step in seconds 
    **/
    double GetTimeStep() const;

    /**
    Calls all device SignalStep functions
    **/
    void SignalStep();

    /**
    Clears Jacobian and B-Equivalent, calls step on all devices, updates the
    system states, and increments the time
    **/
    void Step();

    /**
    Gets the running state of the simulator
    @return true if t < tf, and false if t >= tf
    **/
    bool IsRunning() const;

    /**
    Sets the maximum number of Newton iterations (minor steps)
    @param value is the integer number of interations
    **/
    void SetMaxIterations(const int value);

    /**
    Sets the Newton iteration error tolerance magnitude 
    @param value is the double tolerance value (Volts or Amperes)
    **/
    void SetTolerance(const double value);

    /**
    Gets the value of a state variable
    @param i is the node index of the state
    @return The state value at node i
    **/
    double GetState(const int i) const;

    /**
    Gets the value of a state variable for the previous Newton iteration
    @param i is the node index of the state
    @return The state value at node i
    **/
    double GetIterationState(const int i) const;

    /**
    Gets the difference between the value of two state variables
    @param i is the node index of the first state
    @param i is the node index of the second state
    @return The difference bewteen state i and j (X(i) - X(j))
    **/
    double GetStateDifference(const int i, const int j) const;

    /**
    Gets the difference between the value of two state variables
    at the previous iteration for the Newton solution
    @param i is the node index of the first state
    @param i is the node index of the second state
    @return The difference bewteen state i and j (X(i) - X(j))
    **/
    double GetIterationStateDifference(const int i, const int j) const;

    /**
    Gets the value at Jacobian matrix cell (i, j)
    @param i the Jacobian matrix row
    @param i the Jacobian matrix column
    @return The Jacobian matrix cell value at row i and column j
    **/
    double GetJacobian(const int i, const int j) const;

    /**
    Gets the value at B-Equivalent vector row i
    @param i the B-Equivalent vector row
    @return The B-Equivalent vector row value at row i
    **/
    double GetBEquivalent(const int i) const;

    /**
    Sets the initial value of the state at index i
    @param i is the state index to set
    @param value is the value to set at state i
    **/
    void SetInitialState(const int i, const double value);

    /**
    Sets the value at Jacobian matrix cell (i, j)
    @param i the Jacobian matrix row
    @param i the Jacobian matrix column
    @param The Jacobian matrix cell value to set at row i and column j
    **/
    void AddJacobian(const int i, const int j, const double value);

    /**
    Sets the value at B-Equivalent vector row i
    @param i the B-Equivalent vector row
    @param The B-Equivalent vector row value to set at row i
    **/
    void AddBEquivalent(const int i, const double value);

    /**
    Returns the current iteration number
    **/
    int GetIterations() const;

    /**
    Get a pointer to the Jacobian Matrix
    @return A pointer to the Jacobian Matrix
    **/
    Matrix* GetJacobian();

    /**
    Get a pointer to the B-Equivalent Vector
    @return A pointer to the B-Equivalent Vector
    **/
    ColumnVector* GetBEquivalent();

    /**
    Get a pointer to the State Vector
    @return A pointer to the State Vector
    **/
    ColumnVector* GetStateVector();

    /**
    Get a pointer to the Iteration State Vector
    @return A pointer to the Iteration State Vector
    **/
    ColumnVector* GetIterationStateVector();

    /**
    Get the next node integer and adds to the system
    @return the next node added to the system
    **/
    int GetNextNode();

    /*
    Get the current value for a signal
    @param signal is the signal index
    @return the value for signal at the index
    */
    double GetSignal(const int signal) const;

    /*
    Sets the current value for a signal
    @param signal is the signal index
    @param value is the value to set to the signal at the index
    */
    void SetSignal(const int signal, const double value);

    private:

    /**
    Updates the states for transient and DC analysis (X = J\B)
    @param dc is a flag for solving the dc state (otherwise transient)
    **/
    void Solve(bool dc=false); 

    // Transient analysis:
    double t;                     ///< Current simualtion time for transient simulation (s)
    double tf;                    ///< Final time for the transient simulation (s) 
    double dt;                    ///< Current time step for the transient simulation (s) 

    // Linear Solver:
    Matrix J;                     ///< System Jacobian Matrix
    ColumnVector B;               ///< System B-Equivalent Vector
    ColumnVector X;               ///< System State Vector
    ColumnVector X0;              ///< System Initial State Vector
    vector<Device*> devices;      ///< List of System Devices
    int nodes;                    ///< Number of Nodes (including internal, excluding reference)
    int externalNodes;
    
    // Newton Solver:
    ColumnVector Xk;              ///< State vector for last iteration 
    int iteration;                ///< Current Newton iteration
    int maxIter;                  ///< Maximum number of Newton iterations
    double tolerance;             ///< Maximum error magnitude for states in X vector for Newton solver

    // Signal Solver:
    ColumnVector S;               ///< Signal vector
    int signals;
};


Simulator::Simulator(const int nodes, const int signals)
{
    this->externalNodes = nodes;
    this->signals = signals;

    // init device list to empty list:
    this->devices = vector<Device*>(0);
}

void Simulator::Init(const double dt, const double tf, const int maxIter, const double tolerance, const bool solveDCOperatingPoint)
{
    // transient values:
    this->dt = dt;
    this->tf = tf;
    this->t = 0.0;

    // Newton values:
    this->maxIter = maxIter;
    this->tolerance = tolerance;

    this->nodes = this->externalNodes;

    for (unsigned int i = 0; i < devices.size(); i++)
    {
        devices[i]->Init();
    }

    J = Matrix(nodes, nodes);
    B = ColumnVector(nodes);
    X0 = ColumnVector(nodes);
    X = ColumnVector(nodes);
    Xk = ColumnVector(nodes);

    // solve for DC steady-state if requested:
    if (solveDCOperatingPoint)
    {
        SolveDC();
    }

    S = ColumnVector(signals + 1); // add 1 for 1-based indexing
}

void Simulator::SolveDC()
{
    Solve(true);
}

void Simulator::AddDevice(Device& device)
{
    // add reference to sim object to device:
    device.SetSim(*this);

    // add device to device list:
    this->devices.push_back(&device);
}

double Simulator::GetTime() const
{
    return this->t;
}

double Simulator::GetTimeStep() const
{
    return this->dt;
}

void Simulator::Step()
{
    SignalStep();
    Solve();
    t += dt;
}

void Simulator::SignalStep()
{
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        {
            devices[i]->SignalStep(t, dt);
        }
    }
}

bool Simulator::IsRunning() const
{
    return t < tf;
}

void Simulator::SetMaxIterations(const int value)
{
    this->maxIter = value;
}

void Simulator::SetTolerance(const double value)
{
    this->tolerance = value;
}

double Simulator::GetState(const int i) const
{
    return X(i);
}

double Simulator::GetIterationState(const int i) const
{
    return Xk(i);
}

double Simulator::GetStateDifference(const int i, const int j) const
{
    return X(i) - X(j);
}

double Simulator::GetIterationStateDifference(const int i, const int j) const
{
    return Xk(i) - Xk(j);
}

double Simulator::GetJacobian(const int i, const int j) const
{
    return J(i, j);
}

double Simulator::GetBEquivalent(const int i) const
{
    return B(i);
}

void Simulator::SetInitialState(const int i, const double value)
{
    X0(i) = value;
}

void Simulator::AddJacobian(const int i, const int j, const double value)
{
    J(i, j) += value;
}

void Simulator::AddBEquivalent(const int i, const double value)
{
    B(i) += value;
}

Matrix* Simulator::GetJacobian()
{
    return &J;
}

ColumnVector* Simulator::GetBEquivalent()
{
    return &B;
}

ColumnVector* Simulator::GetStateVector()
{
    return &X;
}

ColumnVector* Simulator::GetIterationStateVector()
{
    return &Xk;
}

int Simulator::GetIterations() const
{
    return iteration;
}

int Simulator::GetNextNode()
{
    nodes += 1;
    return nodes;
}

double Simulator::GetSignal(const int signal) const
{
    return S(signal);
}

void Simulator::SetSignal(const int signal, const double value)
{
    S(signal) = value;
}

///// private functions:

void Simulator::Solve(bool dc)
{
    double error;
    ColumnVector Xkm1;

    Xk = X;

    iteration = 0;

    while (iteration < maxIter)
    {
        J.Clear();
        B.Clear();

        for (unsigned int i = 0; i < devices.size(); i++)
        {
            if (dc)
            {
                devices[i]->DC();
            }
            else
            {
                devices[i]->Step(t, dt);
            }
        }

        Xkm1 = Xk;
        Xk = J.LeftDivide(B);

        error = 0.0;

        for (int i = 1; i <= nodes; i++)
        {
            error = max(abs(Xk(i) - Xkm1(i)), error);
        }

        if (error < tolerance)
        {
            break;
        }

        iteration += 1;
    }

    X = Xk;
}

////////////// Device Implementation //////////////////


void Device::Init()
{
    
}

void Device::DC()
{
    // by default, call transient step function with t=0 and dt=0:
    this->Step(0.0, 0.0);
}

void Device::Step(double t, double dt)
{
    // needs to be implemented in the derived class
}

void Device::SignalStep(double t, double dt)
{
    // needs to be implemented in the derived class
}

void Device::SetSim(Simulator& sim)
{
    this->sim = &sim;
}

double Device::GetTime() const
{
    return sim->GetTime();
}

double Device::GetTimeStep() const
{
    return sim->GetTimeStep();
}

double Device::GetState(const int i)  const
{
    return sim->GetState(i);
}

double Device::GetIterationState(const int i)  const
{
    return sim->GetIterationState(i);
}

double Device::GetStateDifference(const int i, const int j)  const
{
    return sim->GetStateDifference(i, j);
}

double Device::GetIterationStateDifference(const int i, const int j)  const
{
    return sim->GetIterationStateDifference(i, j);
}

double Device::GetJacobian(const int i, const int j)  const
{
    return sim->GetJacobian(i, j);
}

double Device::GetBEquivalent(const int i) const
{
    return sim->GetBEquivalent(i);
}

int Device::GetNextNode()
{
    return sim->GetNextNode();
}

void Device::SetInitialState(const int i, const double value)
{
    sim->SetInitialState(i, value);
}

void Device::AddJacobian(const int i, const int j, const double value)
{
    sim->AddJacobian(i, j, value);
}

void Device::AddBEquivalent(const int i, const double value)
{
    sim->AddBEquivalent(i, value);
}

double Device::GetSignal(const int signal) const
{
    return sim->GetSignal(signal);
}

void Device::SetSignal(const int signal, const double value)
{
    sim->SetSignal(signal, value);
}

#endif
