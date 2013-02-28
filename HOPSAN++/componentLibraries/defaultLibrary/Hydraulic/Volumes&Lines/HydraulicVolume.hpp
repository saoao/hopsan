/*-----------------------------------------------------------------------------
 This source file is part of Hopsan NG

 Copyright (c) 2011 
    Mikael Axin, Robert Braun, Alessandro Dell'Amico, Björn Eriksson,
    Peter Nordin, Karl Pettersson, Petter Krus, Ingo Staack

 This file is provided "as is", with no guarantee or warranty for the
 functionality or reliability of the contents. All contents in this file is
 the original work of the copyright holders at the Division of Fluid and
 Mechatronic Systems (Flumes) at Linköping University. Modifying, using or
 redistributing any part of this file is prohibited without explicit
 permission from the copyright holders.
-----------------------------------------------------------------------------*/

//!
//! @file   HydraulicVolume.hpp
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2009-12-19
//!
//! @brief Contains a Hydraulic Volume Component
//!
//$Id$

#ifndef HYDRAULICVOLUME_HPP_INCLUDED
#define HYDRAULICVOLUME_HPP_INCLUDED

#include "ComponentEssentials.h"

namespace hopsan {

    //!
    //! @brief A hydraulic volume component
    //! @ingroup HydraulicComponents
    //!
    class HydraulicVolume : public ComponentC
    {

    private:
        double mZc;
        double mAlpha;
        double mVolume;
        double mBulkmodulus;

        double *mpND_p1, *mpND_q1, *mpND_c1, *mpND_Zc1, *mpND_p2, *mpND_q2, *mpND_c2, *mpND_Zc2;

        Port *mpP1, *mpP2;

    public:
        static Component *Creator()
        {
            return new HydraulicVolume();
        }

        void configure()
        {
            //Set member attributes
            mBulkmodulus   = 1.0e9;
            mVolume        = 1.0e-3;
            mAlpha         = 0.1;

            //Add ports to the component
            mpP1 = addPowerPort("P1", "NodeHydraulic");
            mpP2 = addPowerPort("P2", "NodeHydraulic");

            //Register changable parameters to the HOPSAN++ core
            registerParameter("V", "Volume", "[m^3]",            mVolume);
            registerParameter("Beta_e", "Bulkmodulus", "[Pa]", mBulkmodulus);
            registerParameter("alpha", "Low pass coeficient to dampen standing delayline waves", "[-]",  mAlpha);

            setStartValue(mpP1, NodeHydraulic::FLOW, 0.0);
            setStartValue(mpP1, NodeHydraulic::PRESSURE, 1.0e5);
            setStartValue(mpP2, NodeHydraulic::FLOW, 0.0);
            setStartValue(mpP2, NodeHydraulic::PRESSURE, 1.0e5);
        }


        void initialize()
        {
            mpND_p1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::PRESSURE);
            mpND_q1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::FLOW);
            mpND_c1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::WAVEVARIABLE);
            mpND_Zc1 = getSafeNodeDataPtr(mpP1, NodeHydraulic::CHARIMP);

            mpND_p2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::PRESSURE);
            mpND_q2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::FLOW);
            mpND_c2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::WAVEVARIABLE);
            mpND_Zc2 = getSafeNodeDataPtr(mpP2, NodeHydraulic::CHARIMP);

            mZc = mBulkmodulus/mVolume*mTimestep/(1.0-mAlpha); //Need to be updated at simulation start since it is volume and bulk that are set.

            //Write to nodes
            (*mpND_c1) = getStartValue(mpP2,NodeHydraulic::PRESSURE)+mZc*getStartValue(mpP2,NodeHydraulic::FLOW);
            (*mpND_Zc1) = mZc;
            (*mpND_c2) = getStartValue(mpP1,NodeHydraulic::PRESSURE)+mZc*getStartValue(mpP1,NodeHydraulic::FLOW);
            (*mpND_Zc2) = mZc;
        }


        void simulateOneTimestep()
        {
            //Declare local variables
            double q1, c1, q2, c2, c10, c20;

            //Get variable values from nodes
            q1 = (*mpND_q1);
            q2 = (*mpND_q2);
            c1 = (*mpND_c1);
            c2 = (*mpND_c2);

            //Volume equations
            c10 = c2 + 2.0*mZc * q2;     //These two equations are from old Hopsan
            c20 = c1 + 2.0*mZc * q1;

            c1 = mAlpha*c1 + (1.0-mAlpha)*c10;
            c2 = mAlpha*c2 + (1.0-mAlpha)*c20;

            //Write new values to nodes
            (*mpND_c1) = c1;
            (*mpND_Zc1) = mZc;
            (*mpND_c2) = c2;
            (*mpND_Zc2) = mZc;
        }

        void finalize()
        {

        }
    };
}

#endif // HYDRAULICVOLUME_HPP_INCLUDED