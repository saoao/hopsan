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

#include "HopsanEssentials.h"
#include "CoreUtilities/HmfLoader.h"
#include <iostream>
#include <string>

#include <tclap/CmdLine.h>
#include "../Utilities/TicToc.h"


using namespace std;
using namespace hopsan;

int main(int argc, char *argv[])
{

    cout << "Hello world!" << endl;

    try {
        TCLAP::CmdLine cmd("HopsanCLI", ' ', "put_version-number_here");

        // Define a value argument and add it to the command line.
        TCLAP::ValueArg<std::string> hmfPathOption("f","hmf","The Hopsan model file to simulate",false,"../Models/pressurerelifepaper.hmf","String containing file path", cmd);

        // Parse the argv array.
        cmd.parse( argc, argv );

        // Get the value parsed by each arg.
        string hmfFilePath = hmfPathOption.getValue();

        HmfLoader coreHmfLoader;

        double startTime=0, stopTime=2;
        ComponentSystem* pRootSystem = coreHmfLoader.loadModel(hmfFilePath, startTime, stopTime);

        std::string msg,type,tag;
        cout << "Check messages: " << HopsanEssentials::getInstance()->checkMessage() << endl;
        while (HopsanEssentials::getInstance()->checkMessage() > 0)
        {
            HopsanEssentials::getInstance()->getMessage(msg,type,tag);
            cout << msg << endl;
        }

        TicToc initTimer("InitializeTime");
        pRootSystem->initialize(startTime, stopTime);
        initTimer.TocPrint();
        TicToc simuTimer("SimulationTime");
        pRootSystem->simulate(startTime, stopTime);
        simuTimer.TocPrint();

        cout << "Check messages: " << HopsanEssentials::getInstance()->checkMessage() << endl;
        while (HopsanEssentials::getInstance()->checkMessage() > 0)
        {
            HopsanEssentials::getInstance()->getMessage(msg,type,tag);
            cout << msg << endl;
        }

        cout << endl << "HopsanCLI Done!" << endl;

    } catch (TCLAP::ArgException &e)  // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        std::cout << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
}
