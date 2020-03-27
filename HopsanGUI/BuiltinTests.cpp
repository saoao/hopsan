#include "BuiltinTests.h"

#include "HcomTest.hpp"

#include "global.h"
#include "ModelHandler.h"
#include <QObject>

void initAppForTest() {
    //Create the model handler object
    //! @todo should move this from mainwindow
    gpModelHandler = new ModelHandler(nullptr);
}

int runBuiltInTests() {

    HComTest hcomtest{};
    int rc = QTest::qExec(&hcomtest);
    return rc;
}
