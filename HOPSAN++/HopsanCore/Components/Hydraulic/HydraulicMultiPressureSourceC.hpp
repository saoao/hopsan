//!
//! @file   HydraulicMultiPressureSourceC.hpp
//! @author FluMeS
//! @date   2009-12-21
//!
//! @brief Contains a Hydraulic Multi Port Pressure Source Component of C-type
//!
//$Id: HydraulicPressureSourceC.hpp 2614 2011-03-10 14:58:17Z bjoer $

#ifndef HYDRAULICMULTIPRESSURESOURCEC_HPP_INCLUDED
#define HYDRAULICMULTIPRESSURESOURCEC_HPP_INCLUDED

#include <vector>
#include "../../ComponentEssentials.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup HydraulicComponents
    //!
    class HydraulicMultiPressureSourceC : public ComponentC
    {
    private:
        double Zc;
        double p;
        Port *mpIn, *mpMP;

        size_t mNumPorts;
        double *mpND_in;//, *mpND_p, *mpND_c, *mpND_Zc;
        std::vector<double*> mND_p_vec; //!< @todo Do we really need this, also in non multiport example
        std::vector<double*> mND_c_vec;
        std::vector<double*> mND_Zc_vec;

    public:
        static Component *Creator()
        {
            return new HydraulicMultiPressureSourceC();
        }

        HydraulicMultiPressureSourceC() : ComponentC()
        {
            //mTypeName = "HydraulicMultiPressureSourceC"; //!< @todo do we really need to set these here it is just one more place tpo make a mistake, we still need to write it again in the cc file as register key value, maybe this value should be aoutmatically uset to set teh mTypeName variable whne a component is registerd in the core
            p         = 1.0e5;
            Zc        = 0.0;

            mpIn = addReadPort("In", "NodeSignal", Port::NOTREQUIRED);
            mpMP = addMultiPort("MP", "NodeHydraulic"); //addPowerPort("MP", "NodeHydraulic");

            registerParameter("P", "Default pressure", "[Pa]", p);

            //! @todo should we set startvalues in one or all ports
            setStartValue(mpMP, NodeHydraulic::PRESSURE, p);
            setStartValue(mpMP, NodeHydraulic::FLOW, 0.0);
        }


        void initialize()
        {
            mpND_in = getSafeNodeDataPtr(mpIn, NodeSignal::VALUE, p);

            mNumPorts = mpMP->getNumPorts();
            for (size_t i=0; i<mNumPorts; ++i)
            {
                mND_p_vec.push_back(getSafeNodeDataPtr(mpMP, NodeHydraulic::PRESSURE, i)); //!< @todo Thiss is WRONG, getSafeNodeDataPtr dosn not suport multiports yet
                mND_c_vec.push_back(getSafeNodeDataPtr(mpMP, NodeHydraulic::WAVEVARIABLE, i));
                mND_Zc_vec.push_back(getSafeNodeDataPtr(mpMP, NodeHydraulic::CHARIMP, i));

                //! @todo how should we divide startvalues among connected ports
                *(mND_p_vec.back()) = p; //Override the startvalue for the pressure
            }
            mpMP->setStartValue(NodeHydraulic::PRESSURE, p); //This is here to show the user that the start value is hard coded!

//            mpND_p = getSafeNodeDataPtr(mpP1, NodeHydraulic::PRESSURE);
//            mpND_c = getSafeNodeDataPtr(mpP1, NodeHydraulic::WAVEVARIABLE);
//            mpND_Zc = getSafeNodeDataPtr(mpP1, NodeHydraulic::CHARIMP);

//            (*mpND_p) = p; //Override the startvalue for the pressure
//            setStartValue(mpP1, NodeHydraulic::PRESSURE, p); //This is here to show the user that the start value is hard coded!

            simulateOneTimestep();
        }


        void simulateOneTimestep()
        {
            for (size_t i=0; i<mNumPorts; ++i)
            {
                *(mND_c_vec[i]) = (*mpND_in);
                *(mND_Zc_vec[i]) = Zc;
            }
//            (*mpND_c) = (*mpND_in);
//            (*mpND_Zc) = Zc;
        }
    };
}

#endif // HYDRAULICPRESSURESOURCEC_HPP_INCLUDED
