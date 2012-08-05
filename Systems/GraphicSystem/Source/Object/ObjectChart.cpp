// Copyright © 2008-2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

//
// extern includes
//
#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include "Ogre.h"
#pragma warning( pop )

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"
using namespace Interface;

//
// Ogre system includes
//
#include "System.h"
#include "Scene.h"
#include "Object.h"
#include "ObjectWindow.h"
#include "ObjectChart.h"

#define ASSERT_EXIST(x, y) if (!x){ASSERT(false); return y;}

#define max(a, b) (((a) > (b)) ? (a) : (b))


//-----------------------------------------------------------------------------
//
// GraphicObjectChart::GraphicObjectChart
//
// Generic charting object, constructor.
//
//-----------------------------------------------------------------------------
GraphicObjectChart::GraphicObjectChart(ISystemScene* pSystemScene, const char* pszName) :
    GraphicObjectWindow(pSystemScene, pszName) {
    m_Type        = GraphicObject::Type_Chart;
    m_dataMax     = 0.0;
    m_dataMin     = 0.0;
    m_dataSamples = m_numPoints;
    m_dataAvg     = 0.0;
    memset(m_data, 0, sizeof(m_data));
}

//-----------------------------------------------------------------------------
//
// GraphicObjectChart::!GraphicObjectChart
//
// Destructor.
//
//-----------------------------------------------------------------------------
GraphicObjectChart::~GraphicObjectChart(void) {
    // DONOTHING
}

//-----------------------------------------------------------------------------
//
// GraphicObjectChart::Initialize
//
// Create the two components of a window. The colored background (Panel) and the
// textual portion (TextArea). Attaches the panel element to the overlay and the
// text area element to the panel element.
//
//-----------------------------------------------------------------------------
Error GraphicObjectChart::Initialize(
    std::vector<Properties::Property> Properties
) {
    Error Err;
    ASSERT(!m_bInitialized);
    Err = Errors::Failure;
    GraphicObjectWindow::Initialize(Properties);
    m_chartWidth  = m_BodyWidth * 0.80f;
    m_chartLeft   = m_BodyLeft + (0.15f * m_BodyWidth);
    m_chartHeight = m_BodyHeight * 0.80f;
    m_chartTop    = m_BodyTop - (0.15f * m_BodyHeight);
    Ogre::OverlayManager* pOverlayManager = (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGREOverlayManager());
    ASSERT_EXIST(pOverlayManager, Err);
    std::string szElementName;
    szElementName = m_pszName;
    szElementName.append("TextYAxisMax");
    m_pTextAreaYAxisMax = static_cast<Ogre::TextAreaOverlayElement*>(pOverlayManager->createOverlayElement("TextArea", szElementName));
    ASSERT_EXIST(m_pTextAreaYAxisMax, Err);
    szElementName = m_pszName;
    szElementName.append("TextYAxisMin");
    m_pTextAreaYAxisMin = static_cast<Ogre::TextAreaOverlayElement*>(pOverlayManager->createOverlayElement("TextArea", szElementName));
    ASSERT_EXIST(m_pTextAreaYAxisMin, Err);
    m_pWindow->addChild(m_pTextAreaYAxisMax);
    m_pTextAreaYAxisMax->setMetricsMode(Ogre::GMM_RELATIVE);
    m_pTextAreaYAxisMax->setPosition((m_ManualWidth * 0.12f) / 2.0f, (m_CharHeight * .5f) + ((m_BodyTop - m_chartTop) / 2.0f));
    m_pTextAreaYAxisMax->setDimensions(1.0f, m_CharHeight * 1.0f);
    m_pTextAreaYAxisMax->setCharHeight(m_CharHeight);
    m_pTextAreaYAxisMax->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Right);
    m_pTextAreaYAxisMax->setFontName("MyFont");
    SetMaxRangeText("100");
    m_pWindow->addChild(m_pTextAreaYAxisMin);
    m_pTextAreaYAxisMin->setMetricsMode(Ogre::GMM_RELATIVE);
    m_pTextAreaYAxisMin->setPosition((m_ManualWidth * 0.12f) / 2.0f, (m_CharHeight * .5f) + (m_BodyTop - m_chartTop + m_chartHeight) / 2.0f);
    m_pTextAreaYAxisMin->setDimensions(1.0f, m_CharHeight * 1.0f);
    m_pTextAreaYAxisMin->setCharHeight(m_CharHeight);
    m_pTextAreaYAxisMin->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Right);
    m_pTextAreaYAxisMin->setFontName("MyFont");
    SetMinRangeText("0");
    m_pManualForeground->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
    m_pManualForeground->position(m_ManualLeft + (0.13f * m_ManualWidth), m_chartTop, 0.0f);
    m_pManualForeground->position(m_chartLeft, m_chartTop, 0.0f);
    m_pManualForeground->position(m_chartLeft, m_chartTop, 0.0f);
    m_pManualForeground->position(m_chartLeft, m_chartTop - m_chartHeight, 0.0f);
    m_pManualForeground->position(m_chartLeft, m_chartTop - m_chartHeight, 0.0f);
    m_pManualForeground->position(m_chartLeft + m_chartWidth, m_chartTop - m_chartHeight, 0.0f);
    m_pManualForeground->position(m_ManualLeft + (0.13f * m_ManualWidth), m_chartTop - m_chartHeight, 0.0f);
    m_pManualForeground->position(m_chartLeft, m_chartTop - m_chartHeight, 0.0f);
    m_pManualForeground->end();
    m_pManualForeground->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
    // Add vertices
    f32 xPos  = m_chartLeft;
    f32 yPos  = m_chartTop - m_chartHeight;
    f32 xStep = m_chartWidth / m_numPoints;

    for (int i = 0; i < m_numPoints; i++) {
        m_xPos[i] = m_chartLeft + (xStep * i);
        m_yPos[i] = yPos;
        m_pManualForeground->position(xPos, yPos, 0.0f);
    }

    // Add index list
    Ogre::uint16 nIndex = 0;

    for (int i = 0; i < m_numPoints; i++) {
        m_pManualForeground->index(nIndex);
        nIndex++;
    }

    // End this section of the object
    m_pManualForeground->end();
    Err = Errors::Success;
    return Err;
}


//-----------------------------------------------------------------------------
//
// GraphicObjectChart::AddDataPoint
//
// Adds a new data point to be graphed.
//
//-----------------------------------------------------------------------------
void GraphicObjectChart::AddDataPoint(f32 data) {
    int i;
    f32 avg;

    if (!m_pManualForeground)
    { return; }

    //-------------------------------------------------------------------------
    // Calculate our total data without the point we are losing
    //
    avg = m_dataAvg * m_dataSamples;
    avg -= m_data[0];
    //-------------------------------------------------------------------------
    // Get rid of our oldest piece of data and add in the newest.
    //
    m_dataMax = 0.0f;

    for (i = 0; i < (m_numPoints - 1); i++) {
        m_data[i] = m_data[i + 1];

        if (m_data[i] > m_dataMax) {
            m_dataMax = m_data[i];
        }
    }

    m_data[i] = data;

    if (m_data[i] > m_dataMax) {
        m_dataMax = m_data[i];
    }

    //-------------------------------------------------------------------------
    // Recalculate our average with the inclusion of the new data point.
    //
    avg += m_data[i];
    m_dataAvg = avg / m_dataSamples;
    //-------------------------------------------------------------------------
    // Recalculate our y-axis max.
    //
    m_yAxisMax = max(2.0f * m_dataAvg, 1.2f * m_dataMax);
    UpdateChart();
    return;
}

//-----------------------------------------------------------------------------
//
// GraphicObjectChart::ChangeOccurred
//
// When a change is made that we are interested in, the CCM calls this method.
// Here we need to get the needed information from the system that posted the
// change.
//
//-----------------------------------------------------------------------------
Error GraphicObjectChart::ChangeOccurred(
    ISubject* pSubject,
    System::Changes::BitMask ChangeType
) {
    Error Err = Errors::Failure;

    if (ChangeType & (System::Changes::Graphics::GUI)) {
        const WindowData* pWindowData;
        IGUIObject* pGUIObj = dynamic_cast<IGUIObject*>(pSubject);
        pWindowData = pGUIObj->GetWindowData();
        ApplyChanges(pWindowData);
    }

    Err = Errors::Success;
    return Err;
}

//-----------------------------------------------------------------------------
//
// GraphicObjectChart::GetDesiredSystemChanges
//
// Let's the CCM know what changes we would like to know about.
//
//-----------------------------------------------------------------------------
System::Types::BitMask GraphicObjectChart::GetDesiredSystemChanges(void) {
    return System::Changes::Graphics::GUI;
}

//-----------------------------------------------------------------------------
//
// GraphicObjectChart::SetMaxRangeText
//
// Changes the maximum y-axis range displayed.
//
//-----------------------------------------------------------------------------
void GraphicObjectChart::SetMaxRangeText(std::string szText) {
    if (m_pTextAreaYAxisMax) {
        m_pTextAreaYAxisMax->setCaption(szText);
    }

    return;
}

//-----------------------------------------------------------------------------
//
// GraphicObjectChart::SetMinRangeText
//
// Changes the minimum y-axis range displayed.
//
//-----------------------------------------------------------------------------
void GraphicObjectChart::SetMinRangeText(std::string szText) {
    if (m_pTextAreaYAxisMin) {
        m_pTextAreaYAxisMin->setCaption(szText);
    }

    return;
}

//-----------------------------------------------------------------------------
//
// GraphicObjectChart::UpdateChart
//
// Updates our chart with the current data.
//
//-----------------------------------------------------------------------------
void GraphicObjectChart::UpdateChart() {
    int i;
    m_pManualForeground->beginUpdate(2);

    for (i = 0; i < m_numPoints - 1; i++) {
        m_yPos[i] = (m_chartTop - m_chartHeight) + (m_chartHeight * (m_data[i] / m_yAxisMax));
        m_pManualForeground->position(m_xPos[i], m_yPos[i], 0.0f);
    }

    m_pManualForeground->end();
    char buf[20];
    sprintf_s(buf, sizeof(buf), "%.0f", m_yAxisMax);
    std::string yMax;
    yMax.assign(buf);
    SetMaxRangeText(yMax);
    return;
}

//-----------------------------------------------------------------------------
//
// GraphicObjectChart::ApplyChanges
//
// Makes changes to the chart.
//
//-----------------------------------------------------------------------------
void GraphicObjectChart::ApplyChanges(const WindowData* pData) {
    if (!pData)
    { return; }

    if (pData->nFlags & WINDOW_DATA) {
        AddDataPoint(pData->nDataPoint);
    }

    GraphicObjectWindow::ApplyChanges(pData);
    return;
}
