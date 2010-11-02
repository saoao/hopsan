//!
//! @file   AppearanceData.cpp
//! @author Peter Nordin <peter.nordin@liu.se>
//! @date   2010-04-22
//!
//! @brief Contains appearance data to be used by guiobjects and library widget
//! @todo Also contains some other appearance stuff that maybe should not be in this fil
//!
//$Id$

#include "AppearanceData.h"
#include "qdebug.h"
#include "GUIUtilities.h"

AppearanceData::AppearanceData()
{
    //Assume all strings default to ""
    mPortAppearanceMap.clear();
}

//QTextStream& operator >>(QTextStream &is, AppearanceData &rData)
//{
//    //! @todo handle returned error indication
//    bool sucess = rData.readFromTextStream(is);
//    rData.mIsReadOK = sucess;
//    return is;
//}

//QTextStream& operator <<(QTextStream &os, AppearanceData &rData)
//{
//    //! @todo maybe write header here (probaly not a good place, better somewhere else)
//    //! @todo find out how to make newline in qt instead of "\n"
//    os << "TYPENAME " << addQuotes(rData.mTypeName) << "\n";
//    os << "DISPLAYNAME " << addQuotes(rData.mName) << "\n";
//    //os << "BASEPATH " << rData.getBasePath() << "\n"; //Base path is computer dependant
//    if (!rData.mQString(ICONPATH)ISO.isEmpty())
//    {
//        os << "ISOICON " << addQuotes(rData.mQString(ICONPATH)ISO) << "\n";
//    }
//    if (!rData.mQString(ICONPATH)User.isEmpty())
//    {
//        os << "USERICON " << addQuotes(rData.mQString(ICONPATH)User) << "\n";
//    }
//    if (!rData.mIconRotationBehaviour.isEmpty())
//    {
//        os << "ICONROTATION " << rData.mIconRotationBehaviour << "\n";
//    }

//    PortAppearanceMapT::iterator it;
//    for (it = rData.mPortAppearanceMap.begin(); it != rData.mPortAppearanceMap.end(); ++it)
//    {
//        os << "PORT " << " "
//           << addQuotes(it.key()) << " "
//           << it.value().x << " "
//           << it.value().y << " "
//           << it.value().rot << "\n";
//    }
//    return os;
//}

//! @brief get the type-name
//! @returns The type-name
QString AppearanceData::getTypeName()
{
    return mTypeName;
}

//! @brief get the display name, even if it is empty
//! @returns The display name
QString AppearanceData::getName()
{
    return mName;
}

//! @brief This function returns the name or typename (if name is empty)
//! Useful if display name has not been specified, then we use the type name
//! @returns A non-empty name
QString AppearanceData::getNonEmptyName()
{
    if (mName.isEmpty())
    {
        return mTypeName;
    }
    else
    {
        return mName;
    }
}

QString AppearanceData::getFullIconPath(graphicsType gfxType)
{
    if ( !mIconPathUser.isEmpty() && (gfxType == USERGRAPHICS) )
    {
        //Use user icon
        return mBasePath + mIconPathUser;
    }
    else if ( !mIconPathISO.isEmpty() && (gfxType == ISOGRAPHICS) )
    {
        //use iso icon
        return mBasePath + mIconPathISO;
    }
    else if ( mIconPathUser.isEmpty() && !mIconPathISO.isEmpty() )
    {
        //Want user icon but not available, use iso icon
        return mBasePath + mIconPathISO;
    }
    else if ( !mIconPathUser.isEmpty() && mIconPathISO.isEmpty() )
    {
        //Want ISO icon but not available, Use user icon
        return mBasePath + mIconPathUser;
    }
    else
    {
        //No icon available use som noname icon
        return "som noname file"; //!< @todo Fix this, Empty icon
    }
}

QString AppearanceData::getIconPathUser()
{
    return mIconPathUser;
}

QString AppearanceData::getIconPathISO()
{
    return mIconPathISO;
}

QString AppearanceData::getIconRotationBehaviour()
{
    return mIconRotationBehaviour;
}

QPointF AppearanceData::getNameTextPos()
{
    return mNameTextPos;
}


PortAppearanceMapT &AppearanceData::getPortAppearanceMap()
{
    return mPortAppearanceMap;
}


QString AppearanceData::getBasePath()
{
    return mBasePath;
}

void AppearanceData::readFromTextStream(QTextStream &rIs)
{
    QString command;
    QString lineStr;
    this->mIsReadOK = true; //Assume read will be ok, set to false if fail bellow

    while (!rIs.atEnd())
    {
        //! @todo need som error handling here if file stream has incorect data
        rIs >> command; //Read the command word

        if (command == "TYPENAME")
        {
            mTypeName = readName(rIs.readLine().trimmed());
        }
        else if (command == "DISPLAYNAME")
        {
            mName = readName(rIs.readLine().trimmed());
        }
        else if (command == "ISOICON")
        {
            mIconPathISO = readName(rIs.readLine().trimmed());
        }
        else if (command == "USERICON")
        {
            mIconPathUser = readName(rIs.readLine().trimmed());
        }
        else if (command == "ICONROTATION")
        {
            mIconRotationBehaviour = rIs.readLine().trimmed();
        }
        else if (command == "PORT")
        {
            lineStr = rIs.readLine();
            QTextStream portStream(&lineStr);
            QString portName=readName(portStream);
            if(portName == "")
            {
                qDebug() << "FEL I PORTNAMN";
                mIsReadOK = false;
            }

            GUIPortAppearance portapp;

            if(portStream.atEnd())
            {
                qDebug() << "SAKNAS DATA";
                mIsReadOK = false;
            }
            portStream >> portapp.x;
            if(portStream.atEnd())
            {
                qDebug() << "SAKNAS DATA";
                mIsReadOK = false;
            }
            portStream >> portapp.y;
            if(portStream.atEnd())
            {
                qDebug() << "SAKNAS DATA";
                mIsReadOK = false;
            }
            portStream >> portapp.rot;

            if( (portapp.rot == 0) || (portapp.rot == 180) )
            {
                portapp.direction = LEFTRIGHT;
            }
            else
            {
                portapp.direction = TOPBOTTOM;
            }

            mPortAppearanceMap.insert(portName, portapp);
        }
        else if (command == "BASEPATH")
        {
            mBasePath = rIs.readLine().trimmed();
        }
        else
        {
            //If incorrect command discard rest of line, ignoring empty lines
            if (!command.isEmpty())
            {
                rIs.readLine().trimmed();
                //qDebug() << "appearanceData: Incorrect command: " + command;
            }
        }
    }

    //Check if read OK!
    //We must have at least a type name
    if (mTypeName.isEmpty())
    {
        mIsReadOK = false;
    }

    this->saveToXML("caf");
}


void AppearanceData::readFromDomElement(QDomElement &rDomElement)
{
    mTypeName       = rDomElement.firstChildElement(HMF_TYPETAG).text();
    mName           = rDomElement.firstChildElement(HMF_DISPLAYNAMETAG).text();
    mIconPathISO    = rDomElement.firstChildElement(HMF_ISOICONTAG).text();
    mIconPathUser   = rDomElement.firstChildElement(HMF_USERICONTAG).text();
    mIconRotationBehaviour = rDomElement.firstChildElement(HMF_ICONROTATIONTAG).text();

    QDomElement xmlPort = rDomElement.firstChildElement(HMF_PORTTAG);
    while (!xmlPort.isNull())
    {
        GUIPortAppearance portApp;
        parseDomValueNode3(xmlPort.firstChildElement(HMF_POSETAG), portApp.x, portApp.y, portApp.rot);
        mPortAppearanceMap.insert(xmlPort.firstChildElement(HMF_NAMETAG).text(), portApp);
        xmlPort = xmlPort.nextSiblingElement(HMF_PORTTAG);
    }
     this->mIsReadOK = true; //Assume read will be ok
     //! @todo maybe remove this in xml
}


void AppearanceData::saveToDomElement(QDomElement &rDomElement)
{
    appendDomTextNode(rDomElement, HMF_TYPETAG, mTypeName);
    appendDomTextNode(rDomElement, HMF_DISPLAYNAMETAG, mName);
    appendDomTextNode(rDomElement, HMF_ISOICONTAG, mIconPathISO);
    appendDomTextNode(rDomElement, HMF_USERICONTAG, mIconPathUser);
    appendDomTextNode(rDomElement, HMF_ICONROTATIONTAG, mIconRotationBehaviour);

    PortAppearanceMapT::iterator pit;
    for (pit=mPortAppearanceMap.begin(); pit!=mPortAppearanceMap.end(); ++pit)
    {
        QDomElement xmlPort = appendDomElement(rDomElement,HMF_PORTTAG);
        appendDomTextNode(xmlPort, HMF_NAMETAG, pit.key());
        appendDomValueNode3(xmlPort, HMF_POSETAG, pit.value().x, pit.value().y, pit.value().rot);
    }
}

//! @brief Temporary hack to test xml appearancedata
void AppearanceData::saveToXML(QString filename)
{
    //Save to file
    #include <QFile>
    #include "version.h"
    QDomDocument doc;
    QDomElement cafroot = doc.createElement("componentappearancefile");
    doc.appendChild(cafroot);
    cafroot.setAttribute("version", CAFVERSION);
    this->saveToDomElement(cafroot);
    const int IndentSize = 4;
    QFile xml(filename);
    if (!xml.open(QIODevice::WriteOnly | QIODevice::Text))  //open file
    {
        qDebug() << "Failed to open file for writing: " << xml.fileName();
        return;
    }
    QTextStream out(&xml);
    appendRootXMLProcessingInstruction(doc); //The xml "comment" on the first line
    doc.save(out, IndentSize);
    xml.close();
}



void AppearanceData::setTypeName(QString name)
{
    mTypeName = name;
}

void AppearanceData::setName(QString name)
{
    mName = name;
}

void AppearanceData::setBasePath(QString path)
{
    mBasePath = path;
}

void AppearanceData::setIconPathUser(QString path)
{
    mIconPathUser = path;
}

void AppearanceData::setIconPathISO(QString path)
{
    mIconPathISO = path;
}

bool AppearanceData::haveIsoIcon()
{
    return !mIconPathISO.isEmpty();
}

bool AppearanceData::haveUserIcon()
{
    return !mIconPathUser.isEmpty();
}
