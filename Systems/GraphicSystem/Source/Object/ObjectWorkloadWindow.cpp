// Copyright � 2008-2009 Intel Corporation
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
#include <OgreBorderPanelOverlayElement.h>
#include <OgreHardwarePixelBuffer.h>
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
#include "ObjectWorkloadWindow.h"

extern ManagerInterfaces       g_Managers;

// Define update constants
const f32 GraphicObjectWorkloadWindow::m_secondsPerUpdate = 1.0f;

//-----------------------------------------------------------------------------
//
// GraphicObjectWorkloadWindow::GraphicObjectWorkloadWindow
//
// Constructor for workload window.  Used to show relative workloads between systems.
//
//-----------------------------------------------------------------------------
GraphicObjectWorkloadWindow::GraphicObjectWorkloadWindow(ISystemScene* pSystemScene, const char* pszName) :
    GraphicObjectWindow(pSystemScene, pszName),
    m_secondsSinceLastUpdate(0.0f),
    m_framesSinceLastUpdate(0) {
    m_Type = GraphicObject::Type_WorkloadWindow;
}

//-----------------------------------------------------------------------------
//
// GraphicObjectWorkloadWindow::~GraphicObjectWorkloadWindow
//
// Destructor for workload window.
//
//-----------------------------------------------------------------------------
GraphicObjectWorkloadWindow::~GraphicObjectWorkloadWindow(void) {
    // DONOTHING
}

//-----------------------------------------------------------------------------
//
// GraphicObjectWorkloadWindow::Update
//
// Get updates for this window, and apply them to the window.  Called every time there is an update.
//
//-----------------------------------------------------------------------------
void GraphicObjectWorkloadWindow::Update(f32 DeltaTime) {
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
        ApplyChanges( pWindowData );
        pWindowData->nFlags = WINDOW_CAPTION;
        ApplyChanges( pWindowData );
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
WindowData* GraphicObjectWorkloadWindow::GetWindowDataUpdate() {
    // Get instrumentation updates.
    IService::IInstrumentation& inst = g_Managers.pService->Instrumentation();
    // Get access to Audio system for audio data
    ISystem* pAudioSystem = (ISystem*) g_Managers.pService->SystemAccess().GetSystem(System::Types::Audio);
    // Format the start of the stream.
    std::stringstream outputStream;
    i32 cpuCount = inst.getCPUCount();
    i32 jobCount = inst.getJobCount();
    // Get the workload job ratios.
    f32* jobRatios = new f32[jobCount];
    inst.getJobRatios(jobRatios);

    if (jobRatios == NULL) {
        return (NULL);
    }

    inst.getJobRatios(jobRatios);

    for (int i = SystemProto::Type_MIN; i < jobCount; i++) {
        SystemProto::Type systemIndex = static_cast<SystemProto::Type>(i);
        // Do not display None type & Generic Type
        if (SystemProto::Type_IsValid(systemIndex) && systemIndex != SystemProto::None && systemIndex != SystemProto::Generic) {
            // Display name
            outputStream << "\n " << SystemProto::Type_Name(systemIndex) << ": ";

            // Job ratios show the amount of one available core that a job is using, so they're a kind
            // of utilization number.  On a 4 core machine, they can vary between 0 and 4.
            // Scale it here to a percent of the overall workload.
            f32 rawPercent;

            // Audio is a special case; it has its own internal thread not scheduled or tracked
            // through the job system, so we need to get stats differently (directly from the
            // audio system).
            if (i == SystemProto::Audio) {
                //rawPercent = pAudioSystem->GetCPUUsage();
                rawPercent = 0;
            } else {
                rawPercent = ((jobRatios[i] * 100.0f) / (f32)cpuCount);
            }

            // Round to nearest integer, then make sure each subsystem has something to report.
            i32 percent = (i32)(rawPercent + 0.5f);

            if (percent == 0) {
                // A zero makes it look like we're claiming a subsystem is totally idle, so just claim 1%.
                outputStream << "1%";  // In this font, "<" looks like an arrow and "~" looks like a "-".  Neither looks right.
            } else {
                outputStream << percent << "%";
            }
        }
    }

    // Clean up.
    delete [] jobRatios;
    // Arrange the fields for display; most fields are filled already.
    m_windowData.sBody = outputStream.str();
    m_windowData.sCaption = "Workload percentages";
    m_windowData.nFlags = WINDOW_BODY;
    // Set the unused fields, for completeness.
    m_windowData.sName = "";
    m_windowData.bShow = true;
    m_windowData.nDataPoint = 0.0f;
    return &m_windowData;
}
