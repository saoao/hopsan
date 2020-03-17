#include "HcomHandler.h"
#include "Widgets/HcomWidget.h"

#include <QtTest>

class HComTest: public QObject
{
    Q_OBJECT
private:
    HcomHandler* mpHcom;

private slots:
    void initTestCase()
    {
        mpHcom = new HcomHandler(new TerminalConsole(nullptr));
    }

    void testScalarMath() {  
        QFETCH(QString, expression);
        QFETCH(bool, expectEvalOK);
        QFETCH(double, expectedValue);

        bool actualEvalOK;
        double evaluatedValue = mpHcom->evaluateScalarExpression(expression, actualEvalOK);
        QVERIFY(expectEvalOK == actualEvalOK);
        QCOMPARE(evaluatedValue, expectedValue);
    }

    void testScalarMath_data() {
        QTest::addColumn<QString>("expression");
        QTest::addColumn<bool>("expectEvalOK");
        QTest::addColumn<double>("expectedValue");

        QTest::newRow("1") << "1+1" << true << 2.0;
        QTest::newRow("2") << "1+(1+1)" << true << 3.0;
        QTest::newRow("3") << "1+(1-1)" << true << 1.0;
        QTest::newRow("4") << "1-(1-2)" << true << 2.0;
        QTest::newRow("5") << "-1+1" << true << 0.0;
        QTest::newRow("6") << "1*2*3" << true << 6.0;
        QTest::newRow("7") << "2*2/3" << true << 4.0/3.0;
        QTest::newRow("8") << "2+2/3" << true << 2+(2.0/3.0);
    }


};
