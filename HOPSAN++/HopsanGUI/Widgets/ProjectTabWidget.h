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
//! @file   ProjectTabWidget.h
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-02-05
//!
//! @brief Contain classes for Project Tabs
//!
//$Id$

#ifndef PROJECTTABWIDGET_H
#define PROJECTTABWIDGET_H

#include <QTabWidget>
#include "CoreAccess.h"
#include "QuickNavigationWidget.h"
#include "AnimationWidget.h"
#include "common.h"
#include <QDomDocument>
#include "ModelHandler.h"

//Forward declaration
class QGraphicsScene;
class GraphicsView;
class ModelWidget;
class SystemContainer;
class AnimationWidget;
class SimulationThreadHandler;
class ModelHandler;


class CentralTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    CentralTabWidget(MainWindow *pParentMainWindow = 0);

    void setTabNotClosable(int index);

protected:
    void tabInserted(int index);
    void tabRemoved(int index);

};

class ModelWidget : public QWidget
{
    Q_OBJECT

public:
    ModelWidget(ModelHandler *modelHandler, CentralTabWidget *parent = 0);
    ~ModelWidget();

    void setTopLevelSimulationTime(const QString startTime, const QString timeStep, const QString stopTime);
    void setToolBarSimulationTimeParametersFromTab();
    QString getStartTime();
    QString getTimeStep();
    QString getStopTime();
    bool isSaved();
    void setSaved(bool value);
    void hasChanged();
    SystemContainer *getTopLevelSystemContainer();
    ContainerObject *getViewContainerObject();
    GraphicsView *getGraphicsView();
    QuickNavigationWidget *getQuickNavigationWidget();
    void setLastSimulationTime(int time);
    int getLastSimulationTime();
    bool isEditingEnabled();
    ModelHandler *mpParentModelHandler;
    GraphicsView *mpGraphicsView;
    AnimationWidget *mpAnimationWidget;
    SimulationThreadHandler *mpSimulationThreadHandler;

public slots:
    bool simulate_nonblocking();
    bool simulate_blocking();
    void startCoSimulation();
    void save();
    void saveAs();
    void exportModelParameters();
    void setExternalSystem(bool value);
    void setEditingEnabled(bool value);
    void openAnimation();
    void lockTab(bool locked);


private slots:
    void collectPlotData();
    void openCurrentContainerInNewTab();
    void closeAnimation();
    void unlockSimulateMutex();

signals:
    void checkMessages();
    void simulationFinished();

private:
    void saveModel(SaveTargetEnumT saveAsFlag, SaveContentsEnumT contents=FullModel);

    QString mStartTime, mStopTime;

    bool mIsSaved;
    SystemContainer *mpToplevelSystem;
    QuickNavigationWidget *mpQuickNavigationWidget;
    QWidget *mpExternalSystemWidget;
    int mLastSimulationTime;
    bool mEditingEnabled;

    QMutex mSimulateMutex;
};

#endif // PROJECTTABWIDGET_H
