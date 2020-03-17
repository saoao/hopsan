#include "BuiltinTests.h"

#include "HcomTest.hpp"

int runBuiltInTests() {

    HComTest hcomtest{};
    int rc = QTest::qExec(&hcomtest);
    return rc;
}
