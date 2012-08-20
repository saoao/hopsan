#ifndef ELECTRICRESISTOR_HPP_INCLUDED
#define ELECTRICRESISTOR_HPP_INCLUDED

#include <iostream>
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "math.h"

//!
//! @file ElectricResistor.hpp
//! @author Petter Krus <petter.krus@liu.se>
//! @date Tue 31 Jan 2012 15:03:07
//! @brief Variable resistor
//! @ingroup ElectricComponents
//!
//This component is generated by COMPGEN for HOPSAN-NG simulation 
//from 
/*{, C:, Documents and Settings, petkr14, My Documents, \
CompgenNG}/ElectricNG.nb*/

using namespace hopsan;

class ElectricResistor : public ComponentQ
{
private:
     double mConduct;
     Port *mpPel1;
     Port *mpPel2;
     int mNstep;
     //Port Pel1 variable
     double uel1;
     double iel1;
     double cel1;
     double Zcel1;
     //Port Pel2 variable
     double uel2;
     double iel2;
     double cel2;
     double Zcel2;
     //inputVariables
     //outputVariables

     //Expressions variables
     //Port Pel1 pointer
     double *mpND_uel1;
     double *mpND_iel1;
     double *mpND_cel1;
     double *mpND_Zcel1;
     //Port Pel2 pointer
     double *mpND_uel2;
     double *mpND_iel2;
     double *mpND_cel2;
     double *mpND_Zcel2;
     //Delay declarations
     //inputVariables pointers
     //outputVariables pointers

public:
     static Component *Creator()
     {
        return new ElectricResistor();
     }

     void configure()
     {
        const double Conduct = 0.1;

        mNstep=9;
        mConduct = Conduct;

        //Add ports to the component
        mpPel1=addPowerPort("Pel1","NodeElectric");
        mpPel2=addPowerPort("Pel2","NodeElectric");

        //Add inputVariables ports to the component

        //Add outputVariables ports to the component

        //Register changable parameters to the HOPSAN++ core
        registerParameter("Conduct", "Conductivity (1/resistance)", "A/V", \
mConduct);
     }

    void initialize()
     {
        //Read port variable pointers from nodes
        //Port Pel1
        mpND_uel1=getSafeNodeDataPtr(mpPel1, NodeElectric::VOLTAGE);
        mpND_iel1=getSafeNodeDataPtr(mpPel1, NodeElectric::CURRENT);
        mpND_cel1=getSafeNodeDataPtr(mpPel1, NodeElectric::WAVEVARIABLE);
        mpND_Zcel1=getSafeNodeDataPtr(mpPel1, NodeElectric::CHARIMP);
        //Port Pel2
        mpND_uel2=getSafeNodeDataPtr(mpPel2, NodeElectric::VOLTAGE);
        mpND_iel2=getSafeNodeDataPtr(mpPel2, NodeElectric::CURRENT);
        mpND_cel2=getSafeNodeDataPtr(mpPel2, NodeElectric::WAVEVARIABLE);
        mpND_Zcel2=getSafeNodeDataPtr(mpPel2, NodeElectric::CHARIMP);
        //Read inputVariables pointers from nodes
        //Read outputVariable pointers from nodes

        //Read variables from nodes
        //Port Pel1
        uel1 = (*mpND_uel1);
        iel1 = (*mpND_iel1);
        cel1 = (*mpND_cel1);
        Zcel1 = (*mpND_Zcel1);
        //Port Pel2
        uel2 = (*mpND_uel2);
        iel2 = (*mpND_iel2);
        cel2 = (*mpND_cel2);
        Zcel2 = (*mpND_Zcel2);

        //Read inputVariables from nodes

        //Read outputVariables from nodes



        //Initialize delays

     }
    void simulateOneTimestep()
     {
        //Read variables from nodes
        //Port Pel1
        cel1 = (*mpND_cel1);
        Zcel1 = (*mpND_Zcel1);
        //Port Pel2
        cel2 = (*mpND_cel2);
        Zcel2 = (*mpND_Zcel2);

        //Read inputVariables from nodes

        //LocalExpressions

          //Expressions
          iel2 = ((cel1 - cel2)*mConduct)/(1 + mConduct*(Zcel1 + Zcel2));
          uel1 = cel1 - iel2*Zcel1;
          uel2 = cel2 + iel2*Zcel2;
          iel1 = -iel2;

        //Calculate the delayed parts


        //Write new values to nodes
        //Port Pel1
        (*mpND_uel1)=uel1;
        (*mpND_iel1)=iel1;
        //Port Pel2
        (*mpND_uel2)=uel2;
        (*mpND_iel2)=iel2;
        //outputVariables

        //Update the delayed variabels

     }
};
#endif // ELECTRICRESISTOR_HPP_INCLUDED
