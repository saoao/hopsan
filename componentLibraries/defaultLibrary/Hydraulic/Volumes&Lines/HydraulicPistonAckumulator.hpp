#ifndef HYDRAULICPISTONACKUMULATOR_HPP_INCLUDED
#define HYDRAULICPISTONACKUMULATOR_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file HydraulicPistonAckumulator.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Tue 8 Apr 2014 15:49:36
//! @brief This is piston with an inertia load
//! @ingroup HydraulicComponents
//!
//==This code has been autogenerated using Compgen==
//from 
/*{, C:, HopsanTrunk, ComponentLibraries, defaultLibrary, Hydraulic, \
Volumes&Lines}/HydraulicPistonAckumulator.nb*/

using namespace hopsan;

class HydraulicPistonAckumulator : public ComponentQ
{
private:
     double Ap;
     double SL;
     double Bp;
     double ML;
     double kappa;
     double p0;
     Port *mpP1;
     double delayParts1[9];
     double delayParts2[9];
     double delayParts3[9];
     double delayParts4[9];
     double delayParts5[9];
     Matrix jacobianMatrix;
     Vec systemEquations;
     Matrix delayedPart;
     int i;
     int iter;
     int mNoiter;
     double jsyseqnweight[4];
     int order[5];
     int mNstep;
     //Port P1 variable
     double p1;
     double q1;
     double T1;
     double dE1;
     double c1;
     double Zc1;
//==This code has been autogenerated using Compgen==
     //inputVariables
     //outputVariables
     double Va;
     double pa;
     double xmp;
     double vmp;
     //Expressions variables
     //Port P1 pointer
     double *mpND_p1;
     double *mpND_q1;
     double *mpND_T1;
     double *mpND_dE1;
     double *mpND_c1;
     double *mpND_Zc1;
     //Delay declarations
//==This code has been autogenerated using Compgen==
     //inputVariables pointers
     //inputParameters pointers
     double *mpAp;
     double *mpSL;
     double *mpBp;
     double *mpML;
     double *mpkappa;
     double *mpp0;
     //outputVariables pointers
     double *mpVa;
     double *mppa;
     double *mpxmp;
     double *mpvmp;
     Delay mDelayedPart10;
     Delay mDelayedPart11;
     Delay mDelayedPart12;
     Delay mDelayedPart20;
     Delay mDelayedPart21;
     Delay mDelayedPart30;
     Delay mDelayedPart40;
     EquationSystemSolver *mpSolver;

public:
     static Component *Creator()
     {
        return new HydraulicPistonAckumulator();
     }

     void configure()
     {
//==This code has been autogenerated using Compgen==

        mNstep=9;
        jacobianMatrix.create(5,5);
        systemEquations.create(5);
        delayedPart.create(6,6);
        mNoiter=2;
        jsyseqnweight[0]=1;
        jsyseqnweight[1]=0.67;
        jsyseqnweight[2]=0.5;
        jsyseqnweight[3]=0.5;


        //Add ports to the component
        mpP1=addPowerPort("P1","NodeHydraulic");
        //Add inputVariables to the component

        //Add inputParammeters to the component
            addInputVariable("Ap", "Piston area 1", "m2", 0.001,&mpAp);
            addInputVariable("SL", "Stroke", "m", 0.5,&mpSL);
            addInputVariable("Bp", "Visc. friction coeff.", "N/m/s", \
10.,&mpBp);
            addInputVariable("ML", "Inertia", "kg", 1.,&mpML);
            addInputVariable("kappa", "polytropic exp. of gas", "", \
1.2,&mpkappa);
            addInputVariable("p0", "Preload pressure", "N/m^2", 1.e7,&mpp0);
        //Add outputVariables to the component
            addOutputVariable("Va","Oil volume","m^3",0.001,&mpVa);
            addOutputVariable("pa","Ackumulator oil \
pressure","Pa",1.e7,&mppa);
            addOutputVariable("xmp","Piston position","m",0.,&mpxmp);
            addOutputVariable("vmp","Piston speed","m",0.,&mpvmp);

//==This code has been autogenerated using Compgen==
        //Add constantParameters
        mpSolver = new EquationSystemSolver(this,5);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port P1
        mpND_p1=getSafeNodeDataPtr(mpP1, NodeHydraulic::Pressure);
        mpND_q1=getSafeNodeDataPtr(mpP1, NodeHydraulic::Flow);
        mpND_T1=getSafeNodeDataPtr(mpP1, NodeHydraulic::Temperature);
        mpND_dE1=getSafeNodeDataPtr(mpP1, NodeHydraulic::HeatFlow);
        mpND_c1=getSafeNodeDataPtr(mpP1, NodeHydraulic::WaveVariable);
        mpND_Zc1=getSafeNodeDataPtr(mpP1, NodeHydraulic::CharImpedance);

        //Read variables from nodes
        //Port P1
        p1 = (*mpND_p1);
        q1 = (*mpND_q1);
        T1 = (*mpND_T1);
        dE1 = (*mpND_dE1);
        c1 = (*mpND_c1);
        Zc1 = (*mpND_Zc1);

        //Read inputVariables from nodes

        //Read inputParameters from nodes
        Ap = (*mpAp);
        SL = (*mpSL);
        Bp = (*mpBp);
        ML = (*mpML);
        kappa = (*mpkappa);
        p0 = (*mpp0);

        //Read outputVariables from nodes
        Va = (*mpVa);
        pa = (*mppa);
        xmp = (*mpxmp);
        vmp = (*mpvmp);

//==This code has been autogenerated using Compgen==


        //Initialize delays
        delayParts1[1] = (-2*Ap*Power(mTimestep,2)*p1 + \
2*Ap*Power(mTimestep,2)*pa - 8*ML*xmp)/(4*ML + 2*Bp*mTimestep);
        mDelayedPart11.initialize(mNstep,delayParts1[1]);
        delayParts1[2] = (-(Ap*Power(mTimestep,2)*p1) + \
Ap*Power(mTimestep,2)*pa + 4*ML*xmp - 2*Bp*mTimestep*xmp)/(4*ML + \
2*Bp*mTimestep);
        mDelayedPart12.initialize(mNstep,delayParts1[2]);
        delayParts2[1] = (-(Ap*mTimestep*p1) + Ap*mTimestep*pa - 2*ML*vmp + \
Bp*mTimestep*vmp)/(2*ML + Bp*mTimestep);
        mDelayedPart21.initialize(mNstep,delayParts2[1]);

        delayedPart[1][1] = delayParts1[1];
        delayedPart[1][2] = mDelayedPart12.getIdx(1);
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];
     }
    void simulateOneTimestep()
     {
        Vec stateVar(5);
        Vec stateVark(5);
        Vec deltaStateVar(5);

        //Read variables from nodes
        //Port P1
        T1 = (*mpND_T1);
        c1 = (*mpND_c1);
        Zc1 = (*mpND_Zc1);

        //Read inputVariables from nodes

        //LocalExpressions

        //Initializing variable vector for Newton-Raphson
        stateVark[0] = xmp;
        stateVark[1] = vmp;
        stateVark[2] = q1;
        stateVark[3] = pa;
        stateVark[4] = p1;

        //Iterative solution using Newton-Rapshson
        for(iter=1;iter<=mNoiter;iter++)
        {
         //PistonAckumulator
         //Differential-algebraic system of equation parts

          //Assemble differential-algebraic equations
          systemEquations[0] =xmp - limit((Ap*Power(mTimestep,2)*(p1 - \
pa))/(4*ML + 2*Bp*mTimestep) - delayedPart[1][1] - delayedPart[1][2],0.,SL);
          systemEquations[1] =vmp - dxLimit(limit((Ap*Power(mTimestep,2)*(p1 \
- pa))/(4*ML + 2*Bp*mTimestep) - delayedPart[1][1] - \
delayedPart[1][2],0.,SL),0.,SL)*(-((Ap*mTimestep*(-p1 + pa))/(2*ML + \
Bp*mTimestep)) - delayedPart[2][1]);
          systemEquations[2] =q1 + Ap*vmp;
          systemEquations[3] =pa - (p0*Power(Ap*SL,kappa))/Power(Ap*(SL - \
xmp),kappa);
          systemEquations[4] =p1 - (c1 + q1*Zc1)*onPositive(p1);

          //Jacobian matrix
          jacobianMatrix[0][0] = 1;
          jacobianMatrix[0][1] = 0;
          jacobianMatrix[0][2] = 0;
          jacobianMatrix[0][3] = \
(Ap*Power(mTimestep,2)*dxLimit((Ap*Power(mTimestep,2)*(p1 - pa))/(4*ML + \
2*Bp*mTimestep) - delayedPart[1][1] - delayedPart[1][2],0.,SL))/(4*ML + \
2*Bp*mTimestep);
          jacobianMatrix[0][4] = \
-((Ap*Power(mTimestep,2)*dxLimit((Ap*Power(mTimestep,2)*(p1 - pa))/(4*ML + \
2*Bp*mTimestep) - delayedPart[1][1] - delayedPart[1][2],0.,SL))/(4*ML + \
2*Bp*mTimestep));
          jacobianMatrix[1][0] = 0;
          jacobianMatrix[1][1] = 1;
          jacobianMatrix[1][2] = 0;
          jacobianMatrix[1][3] = \
(Ap*mTimestep*dxLimit(limit((Ap*Power(mTimestep,2)*(p1 - pa))/(4*ML + \
2*Bp*mTimestep) - delayedPart[1][1] - delayedPart[1][2],0.,SL),0.,SL))/(2*ML \
+ Bp*mTimestep);
          jacobianMatrix[1][4] = \
-((Ap*mTimestep*dxLimit(limit((Ap*Power(mTimestep,2)*(p1 - pa))/(4*ML + \
2*Bp*mTimestep) - delayedPart[1][1] - delayedPart[1][2],0.,SL),0.,SL))/(2*ML \
+ Bp*mTimestep));
          jacobianMatrix[2][0] = 0;
          jacobianMatrix[2][1] = Ap;
          jacobianMatrix[2][2] = 1;
          jacobianMatrix[2][3] = 0;
          jacobianMatrix[2][4] = 0;
          jacobianMatrix[3][0] = \
-(Ap*kappa*p0*Power(Ap*SL,kappa)*Power(Ap*(SL - xmp),-1 - kappa));
          jacobianMatrix[3][1] = 0;
          jacobianMatrix[3][2] = 0;
          jacobianMatrix[3][3] = 1;
          jacobianMatrix[3][4] = 0;
          jacobianMatrix[4][0] = 0;
          jacobianMatrix[4][1] = 0;
          jacobianMatrix[4][2] = -(Zc1*onPositive(p1));
          jacobianMatrix[4][3] = 0;
          jacobianMatrix[4][4] = 1;
//==This code has been autogenerated using Compgen==

          //Solving equation using LU-faktorisation
          mpSolver->solve(jacobianMatrix, systemEquations, stateVark, iter);
          xmp=stateVark[0];
          vmp=stateVark[1];
          q1=stateVark[2];
          pa=stateVark[3];
          p1=stateVark[4];
          //Expressions
          Va = Ap*(SL - xmp);
        }

        //Calculate the delayed parts
        delayParts1[1] = (-2*Ap*Power(mTimestep,2)*p1 + \
2*Ap*Power(mTimestep,2)*pa - 8*ML*xmp)/(4*ML + 2*Bp*mTimestep);
        delayParts1[2] = (-(Ap*Power(mTimestep,2)*p1) + \
Ap*Power(mTimestep,2)*pa + 4*ML*xmp - 2*Bp*mTimestep*xmp)/(4*ML + \
2*Bp*mTimestep);
        delayParts2[1] = (-(Ap*mTimestep*p1) + Ap*mTimestep*pa - 2*ML*vmp + \
Bp*mTimestep*vmp)/(2*ML + Bp*mTimestep);

        delayedPart[1][1] = delayParts1[1];
        delayedPart[1][2] = mDelayedPart12.getIdx(0);
        delayedPart[2][1] = delayParts2[1];
        delayedPart[3][1] = delayParts3[1];
        delayedPart[4][1] = delayParts4[1];
        delayedPart[5][1] = delayParts5[1];

        //Write new values to nodes
        //Port P1
        (*mpND_p1)=p1;
        (*mpND_q1)=q1;
        (*mpND_dE1)=dE1;
        //outputVariables
        (*mpVa)=Va;
        (*mppa)=pa;
        (*mpxmp)=xmp;
        (*mpvmp)=vmp;

        //Update the delayed variabels
        mDelayedPart11.update(delayParts1[1]);
        mDelayedPart12.update(delayParts1[2]);
        mDelayedPart21.update(delayParts2[1]);

     }
    void deconfigure()
    {
        delete mpSolver;
    }
};
#endif // HYDRAULICPISTONACKUMULATOR_HPP_INCLUDED
