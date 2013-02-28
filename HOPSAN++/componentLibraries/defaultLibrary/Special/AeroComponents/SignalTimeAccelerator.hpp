#ifndef SIGNALTIMEACCELERATOR_HPP_INCLUDED
#define SIGNALTIMEACCELERATOR_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file SignalTimeAccelerator.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Thu 23 Aug 2012 15:02:06
//! @brief Accelerate time in mission simulation
//! @ingroup SignalComponents
//!
//This component is generated by COMPGEN for HOPSAN-NG simulation 
//from 
/*{, C:, Documents and Settings, petkr14, My Documents, \
CompgenNG}/SignalControlAeroNG.nb*/

using namespace hopsan;

class SignalTimeAccelerator : public ComponentSignal
{
private:
     double mtimecomp;
     double mmassflow;
     double mvxcg;
     double mvycg;
     Port *mpPtimecomp;
     Port *mpPmassflow;
     Port *mpPvxcg;
     Port *mpPvycg;
     Port *mpPtimeE;
     Port *mpPmassflowE;
     Port *mpPxcgE;
     Port *mpPycgE;
     double delayParts1[9];
     double delayParts2[9];
     double delayParts3[9];
     double delayParts4[9];
     Matrix jacobianMatrix;
     Vec systemEquations;
     Matrix delayedPart;
     int i;
     int iter;
     int mNoiter;
     double jsyseqnweight[4];
     int order[3];
     int mNstep;
     //inputVariables
     double timecomp;
     double massflow;
     double vxcg;
     double vycg;
     //outputVariables
     double timeE;
     double massflowE;
     double xcgE;
     double ycgE;
     //Expressions variables
     //Delay declarations
     //inputVariables pointers
     double *mpND_timecomp;
     double *mpND_massflow;
     double *mpND_vxcg;
     double *mpND_vycg;
     //outputVariables pointers
     double *mpND_timeE;
     double *mpND_massflowE;
     double *mpND_xcgE;
     double *mpND_ycgE;
     Delay mDelayedPart10;
     Delay mDelayedPart11;
     Delay mDelayedPart20;
     Delay mDelayedPart21;
     Delay mDelayedPart30;
     Delay mDelayedPart31;
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new SignalTimeAccelerator();
     }

     void configure()
     {
        const double timecomp = 1.;
        const double massflow = 0.;
        const double vxcg = 0.;
        const double vycg = 0.;

        mNstep=9;
        jacobianMatrix.create(3,3);
        systemEquations.create(3);
        delayedPart.create(4,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;

        mtimecomp = timecomp;
        mmassflow = massflow;
        mvxcg = vxcg;
        mvycg = vycg;

        //Add ports to the component

        //Add inputVariables ports to the component
        mpPtimecomp=addReadPort("Ptimecomp","NodeSignal", Port::NOTREQUIRED);
        mpPmassflow=addReadPort("Pmassflow","NodeSignal", Port::NOTREQUIRED);
        mpPvxcg=addReadPort("Pvxcg","NodeSignal", Port::NOTREQUIRED);
        mpPvycg=addReadPort("Pvycg","NodeSignal", Port::NOTREQUIRED);

        //Add outputVariables ports to the component
        mpPtimeE=addWritePort("PtimeE","NodeSignal", Port::NOTREQUIRED);
        mpPmassflowE=addWritePort("PmassflowE","NodeSignal", \
Port::NOTREQUIRED);
        mpPxcgE=addWritePort("PxcgE","NodeSignal", Port::NOTREQUIRED);
        mpPycgE=addWritePort("PycgE","NodeSignal", Port::NOTREQUIRED);

        //Register changable parameters to the HOPSAN++ core
        registerParameter("timecomp", "time compression rate", "", \
mtimecomp);
        registerParameter("massflow", "Mass flow rate", "kg/s", mmassflow);
        registerParameter("vxcg", "x-position", "m", mvxcg);
        registerParameter("vycg", "y-position", "m", mvycg);
        mpSolver = new EquationSystemSolver(this,3);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Read inputVariables pointers from nodes
        mpND_timecomp=getSafeNodeDataPtr(mpPtimecomp, \
NodeSignal::VALUE,mtimecomp);
        mpND_massflow=getSafeNodeDataPtr(mpPmassflow, \
NodeSignal::VALUE,mmassflow);
        mpND_vxcg=getSafeNodeDataPtr(mpPvxcg, NodeSignal::VALUE,mvxcg);
        mpND_vycg=getSafeNodeDataPtr(mpPvycg, NodeSignal::VALUE,mvycg);
        //Read outputVariable pointers from nodes
        mpND_timeE=getSafeNodeDataPtr(mpPtimeE, NodeSignal::VALUE);
        mpND_massflowE=getSafeNodeDataPtr(mpPmassflowE, NodeSignal::VALUE);
        mpND_xcgE=getSafeNodeDataPtr(mpPxcgE, NodeSignal::VALUE);
        mpND_ycgE=getSafeNodeDataPtr(mpPycgE, NodeSignal::VALUE);

        //Read variables from nodes

        //Read inputVariables from nodes
        timecomp = (*mpND_timecomp);
        massflow = (*mpND_massflow);
        vxcg = (*mpND_vxcg);
        vycg = (*mpND_vycg);

        //Read outputVariables from nodes
        timeE = mpPtimeE->getStartValue(NodeSignal::VALUE);
        massflowE = mpPmassflowE->getStartValue(NodeSignal::VALUE);
        xcgE = mpPxcgE->getStartValue(NodeSignal::VALUE);
        ycgE = mpPycgE->getStartValue(NodeSignal::VALUE);



        //Initialize delays
        delayParts1[1] = (-(mTimestep*timecomp*vxcg) - 2*xcgE)/2.;
        mDelayedPart11.initialize(mNstep,delayParts1[1]);
        delayParts2[1] = (-(mTimestep*timecomp*vycg) - 2*ycgE)/2.;
        mDelayedPart21.initialize(mNstep,delayParts2[1]);
        delayParts3[1] = (-(mTimestep*timecomp) - 2*timeE)/2.;
        mDelayedPart31.initialize(mNstep,delayParts3[1]);

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
     }
    void simulateOneTimestep()
     {
        Vec stateVar(3);
        Vec stateVark(3);
        Vec deltaStateVar(3);

        //Read variables from nodes

        //Read inputVariables from nodes
        timecomp = (*mpND_timecomp);
        massflow = (*mpND_massflow);
        vxcg = (*mpND_vxcg);
        vycg = (*mpND_vycg);

        //LocalExpressions

        //Initializing variable vector for Newton-Raphson
        stateVark[0] = xcgE;
        stateVark[1] = ycgE;
        stateVark[2] = timeE;

        //Iterative solution using Newton-Rapshson
        for(iter=1;iter<=mNoiter;iter++)
        {
         //TimeAccelerator
         //Differential-algebraic system of equation parts

          //Assemble differential-algebraic equations
          systemEquations[0] =-(mTimestep*timecomp*vxcg)/2. + xcgE + \
delayedPart[1][1];
          systemEquations[1] =-(mTimestep*timecomp*vycg)/2. + ycgE + \
delayedPart[2][1];
          systemEquations[2] =-(mTimestep*timecomp)/2. + timeE + \
delayedPart[3][1];

          //Jacobian matrix
          jacobianMatrix[0][0] = 1;
          jacobianMatrix[0][1] = 0;
          jacobianMatrix[0][2] = 0;
          jacobianMatrix[1][0] = 0;
          jacobianMatrix[1][1] = 1;
          jacobianMatrix[1][2] = 0;
          jacobianMatrix[2][0] = 0;
          jacobianMatrix[2][1] = 0;
          jacobianMatrix[2][2] = 1;

          //Solving equation using LU-faktorisation
          mpSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);
          xcgE=stateVark[0];
          ycgE=stateVark[1];
          timeE=stateVark[2];
          //Expressions
          massflowE = massflow*timecomp;
        }

        //Calculate the delayed parts
        delayParts1[1] = (-(mTimestep*timecomp*vxcg) - 2*xcgE)/2.;
        delayParts2[1] = (-(mTimestep*timecomp*vycg) - 2*ycgE)/2.;
        delayParts3[1] = (-(mTimestep*timecomp) - 2*timeE)/2.;

        delayedPart[1][1] = delayParts1[1];
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];

        //Write new values to nodes
        //outputVariables
        (*mpND_timeE)=timeE;
        (*mpND_massflowE)=massflowE;
        (*mpND_xcgE)=xcgE;
        (*mpND_ycgE)=ycgE;

        //Update the delayed variabels
        mDelayedPart11.update(delayParts1[1]);
        mDelayedPart21.update(delayParts2[1]);
        mDelayedPart31.update(delayParts3[1]);

     }
};
#endif // SIGNALTIMEACCELERATOR_HPP_INCLUDED