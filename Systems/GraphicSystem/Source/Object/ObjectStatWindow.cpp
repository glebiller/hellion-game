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

#ifdef SAFE_DELETE
#undef SAFE_DELETE
#endif
#ifdef SAFE_DELETE_ARRAY
#undef SAFE_DELETE_ARRAY
#endif

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
#include "ObjectStatWindow.h"


extern ManagerInterfaces       g_Managers;

// Define update constants
const f32 GraphicObjectStatWindow::m_secondsPerUpdate = 1.0f;



//-----------------------------------------------------------------------------
//
// GraphicObjectStatWindow::GraphicObjectStatWindow
//
// Stat window constructor.
//
//-----------------------------------------------------------------------------
GraphicObjectStatWindow::GraphicObjectStatWindow(ISystemScene* pSystemScene, const char* pszName) :
    GraphicObjectWindow(pSystemScene, pszName),
    m_secondsSinceLastUpdate(0.0f),
    m_framesSinceLastUpdate(0) {
    m_Type = GraphicObject::Type_StatWindow;
}


//-----------------------------------------------------------------------------
//
// GraphicObjectStatWindow::~GraphicObjectStatWindow
//
// Stat window destructor.
//
//-----------------------------------------------------------------------------
GraphicObjectStatWindow::~GraphicObjectStatWindow() {
    // DONOTHING
}


//-----------------------------------------------------------------------------
//
// GraphicObjectStatWindow::Update
//
// Get updates for this window, and apply them to the window.  Called every time there is an update.
//
//-----------------------------------------------------------------------------
void GraphicObjectStatWindow::Update(f32 DeltaTime) {
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


//-----------------------------------------------------------------------------
//
// GraphicObjectStatWindow::GetWindowDataUpdate
//
// This method gets the latest performance stats, and formats them for the caller.
//
//-----------------------------------------------------------------------------
WindowData* GraphicObjectStatWindow::GetWindowDataUpdate() {
    // Get instrumentation updates.
    IService::IInstrumentation& inst = g_Managers.pService->Instrumentation();
    // Get the basic stats: average FPS of recent frames, and thread count.
    f32 frameRate = inst.getCurrentFPS();
    i32 activeThreads = inst.getActiveThreadCount();
    // Format the start of the stream.
    std::stringstream outputStream;
    outputStream << " " << (int)frameRate << " FPS\n";
    outputStream << " " << activeThreads << " thread";

    if (activeThreads != 1) {
        outputStream << "s";
    }

    // One counter per processor, plus one for _Total.
    i32 numCounters = inst.getNumCounters();
    f64* CPUPercent = new f64[numCounters];

    if (CPUPercent == NULL) {
        return NULL;
    }

    inst.getCPUCounters(CPUPercent);

    // Get all individual CPU counters; don't show the last one ("Total") because it's shown elsewhere.
    for (int i = 0; i < numCounters - 1; i++) {
        outputStream << "\n";
        // This code "guesses" at the order of the counters, and figures that counters are passed
        // in the order 0-n, Total.  This could be made more automatic, but we'd need
        // instrumentation code to tell us the actual order.
        //
        // Processors are numbered from 0.  Number them from 1 here to be a bit more user friendly.
        outputStream << " CPU " << i + 1 << ": " << (int)CPUPercent[i] << "%";
    }

    delete [] CPUPercent;
    // Arrange the data for display.
    m_windowData.sBody = outputStream.str();
    m_windowData.sCaption = "Performance";
    m_windowData.nFlags = WINDOW_BODY;
    // Set the unused fields, for completeness.
    m_windowData.sName = "";
    m_windowData.bShow = true;
    m_windowData.nDataPoint = 0.0f;
    return &m_windowData;
}
