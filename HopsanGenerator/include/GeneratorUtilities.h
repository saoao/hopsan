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
//! @file   ComponentGeneratorUtilities.h
//! @author Robert Braun <robert.braun@liu.se
//! @date   2012-01-08
//!
//! @brief Contains component generation utiluty functions
//!
//$Id$


#ifndef GENERATORUTILITIES_H
#define GENERATORUTILITIES_H

#include <QPointF>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QDomElement>
#include "win32dll.h"

#include "SymHop.h"

//class ModelObjectAppearance;

namespace hopsan {
class ComponentSystem;
}

class HopsanGenerator;


class FMIPortSpecification
{
public:
    FMIPortSpecification(QString varName, QString portName, QString mpndName, QString valueRef, QString portType, QString nodeType, QString dataType, QString causality);
    QString varName;
    QString portName;
    QString mpndName;
    QString valueRef;
    QString portType;
    QString nodeType;
    QString dataType;
    QString causality;
};


class FMIParameterSpecification
{
public:
    FMIParameterSpecification(QString varName, QString parName, QString description, QString initValue, QString valueRef);
    QString varName;
    QString parName;
    QString description;
    QString initValue;
    QString valueRef;
};


class PortSpecification
{
public:
    PortSpecification(QString porttype = "ReadPortType", QString nodetype = "NodeSignal", QString name = QString(), bool notrequired=false, QString defaultvalue=QString());
    QString porttype;
    QString nodetype;
    QString name;
    bool notrequired;
    QString defaultvalue;
};

class ParameterSpecification
{
public:
    ParameterSpecification(QString name = QString(), QString displayName = QString(), QString description = QString(), QString unit = QString(), QString init = QString());
    QString name;
    QString displayName;
    QString description;
    QString unit;
    QString init;
};

class VariableSpecification
{
public:
    VariableSpecification(QString name = QString(), QString init = QString());
    QString name;
    QString init;
};

class UtilitySpecification
{
public:
    UtilitySpecification(QString utility="FirstOrderTransferFunction", QString name=QString());
    QString utility;
    QString name;
};

class StaticVariableSpecification
{
public:
    StaticVariableSpecification(QString datatype="double", QString name=QString());
    QString datatype;
    QString name;
};

class ComponentSpecification
{
public:
    ComponentSpecification(QString typeName="", QString displayName="", QString cqsType="");
    QString typeName;
    QString displayName;
    QString cqsType;
    QStringList utilities;
    QStringList utilityNames;
    QStringList parNames;
    QStringList parInits;
    QStringList parDisplayNames;
    QStringList parDescriptions;
    QStringList parUnits;
    QStringList varNames;
    QStringList varTypes;
    QStringList varInits;
    QStringList portNames;
    QStringList portTypes;
    QStringList portNodeTypes;
    QStringList portDefaults;
    QList<bool> portNotReq;
    QStringList confEquations;
    QStringList initEquations;
    QStringList simEquations;
    QStringList finalEquations;
    QStringList deconfEquations;
    QStringList auxiliaryFunctions;

    QString plainCode;
};



QDomElement loadXMLDomDocument(QFile &rFile, QDomDocument &rDomDocument, QString rootTagName);
bool removeDir(QString path);
void copyDir(const QString fromPath, QString toPath);
bool compileComponentLibrary(QString path, HopsanGenerator *pGenerator, QString extraLinks="");
bool compile(QString path, QString o, QString c, QString i, QString l, QString flags, QString &output);
QString toVarName(const QString org);
QString extractTaggedSection(QString str, QString tag);
void replaceTaggedSection(QString &str, QString tag, QString replacement);
QString replaceTag(QString str, QString tag, QString replacement);
QString replaceTags(QString str, QStringList tags, QStringList replacements);

bool verifyEquationSystem(QList<SymHop::Expression> equations, QList<SymHop::Expression> stateVars, HopsanGenerator *pGenerator);
void findAllFilesInFolderAndSubFolders(QString path, QString ext, QStringList &files);
QStringList getHopsanCoreSourceFiles();
QStringList getHopsanCoreIncludeFiles(bool skipDependencies=false);


class GeneratorNodeInfo
{
    public:
        GeneratorNodeInfo(QString nodeType);
        static void getNodeTypes(QStringList &nodeTypes);

        QString niceName;
        QStringList qVariables;
        QStringList cVariables;
        QStringList variableLabels;
        QStringList shortNames;
        QList<size_t> varIdx;
};


//! @brief This utility class wraps a QTextStream and have stream operators to write whole lines. You do not need to add the newline char yourself.
class QTextLineStream
{
public:
    QTextLineStream(QTextStream &rTextStream)
    {
        mpQTextSream = &rTextStream;
    }
    friend QTextLineStream& operator <<(QTextLineStream &rLineStream, const char* input);

private:
    QTextStream* mpQTextSream;
};


#endif // GENERATORUTILITIES_H