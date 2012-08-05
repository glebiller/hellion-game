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

//
// Ogre system includes
//
#include "System.h"
#include "Scene.h"
#include "Object.h"
#include "ObjectWindow.h"
#include "ObjectChart.h"
#include "ObjectCPUChart.h"

extern ManagerInterfaces       g_Managers;

// Define update constants
const f32 GraphicObjectCPUChart::m_secondsPerUpdate = 1.0f;


//-----------------------------------------------------------------------------
//
// GraphicObjectCPUChart::GraphicObjectCPUChart
//
// CPU utilization charting object.  Constructor.
//
//-----------------------------------------------------------------------------
GraphicObjectCPUChart::GraphicObjectCPUChart(ISystemScene* pSystemScene, const char* pszName) :
    GraphicObjectChart(pSystemScene, pszName),
    m_secondsSinceLastUpdate(0.0f),
    m_framesSinceLastUpdate(0) {
    m_Type = GraphicObject::Type_CPUChart;
    m_yAxisMax = 100.0f;
}

//-----------------------------------------------------------------------------
//
// GraphicObjectCPUChart::~GraphicObjectCPUChart
//
// Destructor.
//
//-----------------------------------------------------------------------------
GraphicObjectCPUChart::~GraphicObjectCPUChart(void) {
    // DONOTHING
}

//-----------------------------------------------------------------------------
//
// GraphicObjectCPUChart::GetWindowDataUpdate
//
// Get a WindowData object for the chart to show.  This gets its data from the Instrumentation
// object.
//
//-----------------------------------------------------------------------------
WindowData* GraphicObjectCPUChart::GetWindowDataUpdate() {
    // Get instrumentation updates.
    IService::IInstrumentation& inst = g_Managers.pService->Instrumentation();
    // One counter per processor, plus one for _Total.
    i32 numCounters = inst.getNumCounters();
    f64* CPUPercent = new f64[numCounters];

    if (CPUPercent == NULL) {
        return NULL;
    }

    inst.getCPUCounters(CPUPercent);
    // Arrange the data for display.  We have new data and a new caption.
    m_windowData.nDataPoint = (f32)CPUPercent[numCounters - 1];
    std::stringstream captionStream;
    captionStream << "Total CPU: " << (int)CPUPercent[numCounters - 1] << "%";
    m_windowData.sCaption = captionStream.str();
    m_windowData.nFlags = WINDOW_DATA;
    // Set the unused fields, for completeness.
    m_windowData.sName = "";
    m_windowData.bShow = true;
    m_windowData.sBody = "";
    // Free up locally-allocated stuff.
    delete [] CPUPercent;
    return &m_windowData;
}


//-----------------------------------------------------------------------------
//
// GraphicObjectCPUChart::AddDataPoint
//
// Adds a new data point to be graphed.  Overrides the dynamic min/max function
// of the superclass, to limit max to 100.
//
//-----------------------------------------------------------------------------
void GraphicObjectCPUChart::AddDataPoint(f32 data) {
    int i;

    if (!m_pManualForeground) {
        return;
    }

    //-------------------------------------------------------------------------
    // Get rid of our oldest piece of data and add in the newest.
    //
    for (i = 0; i < m_numPoints - 1; i++) {
        m_data[i] = m_data[i + 1];
    }

    m_data[i] = data;
    UpdateChart();
    return;
}



//-----------------------------------------------------------------------------
//
// GraphicObjectCPUChart::Update
//
// Update gets called when the change manager has a new time slice for us.  DeltaTime tells
// how long it has been since we were last called.  Pull new data when it's needed here.
//
//-----------------------------------------------------------------------------
void GraphicObjectCPUChart::Update(f32 DeltaTime) {
    // Don't do anything if this window is not visible.
    if (!this->m_pWindow->isVisible()) {
        return;
    }

    // Only get it if it has been a while.
    m_framesSinceLastUpdate++;
    m_secondsSinceLastUpdate += DeltaTime;

    if (m_secondsSinceLastUpdate > m_secondsPerUpdate) {
        // Get whatever data we need for this window and apply it; do it again
        // to make sure the caption is shown, too.
        WindowData* pWindowData = GetWindowDataUpdate();
        ApplyChanges(pWindowData);
        pWindowData->nFlags = WINDOW_CAPTION;
        ApplyChanges(pWindowData);
        // Start counting again toward the next update.
        m_secondsSinceLastUpdate = 0.0f;
        m_framesSinceLastUpdate = 0;
    }
}
