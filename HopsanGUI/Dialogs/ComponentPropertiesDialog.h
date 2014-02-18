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
//! @file   ComponentPropertiesDialog.h
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-03-01
//!
//! @brief Contains a class for interact with paramters
//!
//$Id$

#ifndef COMPONENTPROPERTIESDIALOG_H
#define COMPONENTPROPERTIESDIALOG_H

#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QDialogButtonBox>

#include "Dialogs/ModelObjectPropertiesDialog.h"

class Component;
class ParameterSettingsLayout;

class ComponentPropertiesDialog : public ModelObjectPropertiesDialog
{
    Q_OBJECT

public:
    ComponentPropertiesDialog(Component *pComponent, QWidget *pParent=0);

protected slots:
    void okPressed();
    void editPortPos();

protected:
    void setParametersAndStartValues();
    void recompileCppFromDialog();
    virtual void closeEvent(QCloseEvent *);
    virtual void reject();

private:
    Component *mpComponent;

    void createEditStuff();
    void createCppEditStuff();
    void createModelicaEditStuff();
    bool interpretedAsStartValue(QString &parameterDescription);

    QLabel *mpLabel;
    QLineEdit *mpLineEdit;
    QLineEdit *mpNameEdit;

    QVector<ParameterSettingsLayout*> mvParameterLayout;
    QVector<ParameterSettingsLayout*> mvStartValueLayout;

    QDialogButtonBox *mpButtonBox;
    QPushButton *mpOkButton;
    QPushButton *mpCancelButton;
    QPushButton *mpEditPortPos;
    QWidget *mpExtension;

    QSpinBox *mpInputPortsSpinBox;
    QSpinBox *mpOutputPortsSpinBox;
    QTextEdit *mpTextEdit;
};

#endif // COMPONENTPROPERTIESDIALOG_H