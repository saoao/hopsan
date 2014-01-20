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
//! @file   PlotCurve.h
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010
//!
//! @brief Contains a class for plot curves
//!
//$Id: ModelHandler.cpp 5551 2013-06-20 08:54:16Z petno25 $

#ifndef PLOTCURVE_H
#define PLOTCURVE_H

#include "qwt_legend_data.h"
#include "qwt_plot_legenditem.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_marker.h"
#include "qwt_plot_intervalcurve.h"

#include <QLabel>
#include <QToolButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>

#include "LogVariable.h"
#include "common.h"

// Forward Declaration
class PlotArea;
class PlotCurve;

enum {AxisIdRole=QwtLegendData::UserRole+1};

class PlotLegend : public QwtPlotLegendItem
{
private:
    QwtPlot::Axis mAxis;
    int mnItems;

public:
    PlotLegend(QwtPlot::Axis axisId);
    void updateLegend( const QwtPlotItem *plotItem, const QList<QwtLegendData> &data );
};


enum HopsanPlotCurveTypeEnumT {PortVariableType, FrequencyAnalysisType, NyquistType, BodeGainType, BodePhaseType, GeneralType};

//! @brief Class describing a plot curve in plot window
class PlotCurve : public QObject, public QwtPlotCurve
{
    Q_OBJECT
    friend class CurveInfoBox;
    friend class PlotWindow;
    friend class PlotArea;
public:
    enum {LegendShowLineAndSymbol=QwtPlotCurve::LegendShowBrush+1};

    PlotCurve(SharedVariablePtrT pData, int axisY=QwtPlot::yLeft, HopsanPlotCurveTypeEnumT curveType=PortVariableType);
    ~PlotCurve();

    void setIncludeGenerationInTitle(bool doit);
    QString getCurveName() const;
    QString getCurveNameWithGeneration() const;
    HopsanPlotCurveTypeEnumT getCurveType();
    int getAxisY();

    SharedVariablePtrT getLogDataVariablePtr(); //! @todo is this needed
    const SharedVariablePtrT getLogDataVariablePtr() const;
    QVector<double> getDataVectorCopy() const;
    const SharedVariablePtrT getSharedTimeOrFrequencyVector() const;
    bool hasCustomXData() const;
    const SharedVariablePtrT getSharedCustomXData() const;

    int getGeneration() const;
    const QString &getComponentName() const;
    const QString &getPortName() const;
    const QString &getDataName() const;
    const QString &getDataOriginalUnit() const;
    const QString &getDataCustomPlotUnit() const;
    const QString &getCurrentUnit() const;
    VariableSourceTypeT getDataSource() const;

    void setCustomCurveDataUnit(const QString &rUnit);
    void setCustomCurveDataUnit(const QString &rUnit, double scale);
    void removeCustomCurveDataUnit();
    void setTimePlotScalingAndOffset(double scale, double offset);
    void setLocalCurvePlotScaleAndOffset(const double scale, const double offset);
    void setDataPlotOffset(const double offset);

    void setCustomData(const VariableDescription &rVarDesc, const QVector<double> &rvTime, const QVector<double> &rvData);
    void setCustomXData(const VariableDescription &rVarDesc, const QVector<double> &rvXdata);
    void setCustomXData(SharedVariablePtrT pData);
    void setCustomXData(const QString fullName);

    QColor getLineColor() const;
    void resetLegendSize();

    // Qwt overloaded function
    QList<QwtLegendData> legendData() const;

signals:
    void curveDataUpdated();
    void curveInfoUpdated();
    void colorChanged(QColor);
    void markedActive(bool);

public slots:
    bool setGeneration(const int generation);
    void setPreviousGeneration();
    void setNextGeneration();

    void setLineWidth(int);
    void setLineStyle(QString lineStyle);
    void setLineSymbol(QString lineSymbol);
    void setLineColor(QColor color);
    void setLineColor(QString colorName=QString());
    void openScaleDialog();
    void updateTimePlotScaleFromDialog();
    void updateLocalPlotScaleAndOffsetFromDialog();
    void updateDataPlotOffsetFromDialog();
    void updateToNewGeneration();
    void removeMe();

    void refreshCurveTitle();
    void setAutoUpdate(bool value);
    void openFrequencyAnalysisDialog();
    void markActive(bool value);

private slots:
    void updateCurve();
    void updateCurveName();

private:
    // Private member functions
    void deleteCustomData();
    void connectDataSignals();

    // Curve data
    HopsanPlotCurveTypeEnumT mCurveType;
    SharedVariablePtrT mpData;
    SharedVariablePtrT mpCustomXdata;
    bool mHaveCustomData;

    QString mCustomCurveDataUnit;
    double mCustomCurveDataUnitScale;
    double mLocalAdditionalCurveScale;
    double mLocalAdditionalCurveOffset;

    // Curve properties settings
    bool mAutoUpdate, mIsActive, mIncludeGenInTitle;
    int mAxisY;
    QComboBox *mpTimeScaleComboBox;
    QDoubleSpinBox *mpTimeOffsetSpinBox;
    QLineEdit *mpLocalCurveScaleLineEdit;
    QLineEdit *mpLocalCurveOffsetLineEdit;
    QLineEdit *mpDataPlotOffsetLineEdit;
    PlotArea *mpParentPlotArea;

    // Line properties
    QColor mLineColor;
    QString mLineStyle;
    QString mLineSymbol;
    QwtSymbol *mpCurveSymbol;
    int mLineWidth;
    int mCurveSymbolSize;
};



class PlotMarker : public QObject, public QwtPlotMarker
{
    Q_OBJECT
public:
    PlotMarker(PlotCurve *pCurve, PlotArea *pPlotTab);
    PlotCurve *getCurve();
    virtual bool eventFilter (QObject *, QEvent *);
    void setMovable(bool movable);

public slots:
    void refreshLabel(const double x, const double y);
    void refreshLabel(const QString &label);
    void setColor(QColor color);

private:
    PlotCurve *mpCurve;
    PlotArea *mpPlotArea;
    QwtSymbol *mpMarkerSymbol;
    Qt::Alignment mLabelAlignment;
    int mMarkerSize;
    bool mIsBeingMoved;
    bool mIsMovable;
};

#endif // PLOTCURVE_H
