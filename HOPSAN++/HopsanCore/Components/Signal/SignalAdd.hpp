//!
//! @file   SignalAdd.hpp
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010-01-11
//!
//! @brief Contains a mathematical summator component
//!
//$Id$

#ifndef SIGNALADD_HPP_INCLUDED
#define SIGNALADD_HPP_INCLUDED

#include "../../ComponentEssentials.h"

namespace hopsan {

    //!
    //! @brief
    //! @ingroup SignalComponents
    //!
    class SignalAdd : public ComponentSignal
    {

    private:
        double *mpND_in1, *mpND_in2, *mpND_out;
        Port *mpIn1, *mpIn2, *mpOut;

    public:
        static Component *Creator()
        {
            return new SignalAdd("Add");
        }

        SignalAdd(const std::string name) : ComponentSignal(name)
        {
            mTypeName = "SignalAdd";

            mpIn1 = addReadPort("in1", "NodeSignal", Port::NOTREQUIRED);
            mpIn2 = addReadPort("in2", "NodeSignal", Port::NOTREQUIRED);
            mpOut = addWritePort("out", "NodeSignal", Port::NOTREQUIRED);
        }


        void initialize()
        {

            mpND_in1 = getSafeNodeDataPtr(mpIn1, NodeSignal::VALUE, 0);
            mpND_in2 = getSafeNodeDataPtr(mpIn2, NodeSignal::VALUE, 0);
            mpND_out = getSafeNodeDataPtr(mpOut, NodeSignal::VALUE);
        }


        void simulateOneTimestep()
        {
            (*mpND_out) = (*mpND_in1) + (*mpND_in2);
        }
    };
}
#endif // SIGNALADD_HPP_INCLUDED
