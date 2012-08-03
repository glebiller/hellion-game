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

#pragma once

class OGREGraphicsScene;

/// <summary>
///     CPU chart.  Show chart of CPU loads.
/// </summary>
class GraphicObjectCPUChart : public GraphicObjectChart {

        friend OGREGraphicsScene;

    public:
        /// <summary>
        ///     Add a new data point to this CPU chart.  Recent data points are cached.
        ///     During the next screen update, the contents of the window will now show
        ///     this new data point plus all cached data points.
        /// </summary>
        /// <param name="fDataPoint">f32 - Data point to add to the chart.</param>
        virtual void AddDataPoint(f32 fDataPoint);

    protected:

        /// <summary cref="GraphicObjectCPUChart::GraphicObjectCPUChart">
        ///     Basic constructor; the only real one.
        /// </summary>
        /// <param name="pSystemScene">ISystemScene* - Pointer to the scene containing this window.</param>
        /// <param name="pszName">pcstr - Name of this window.</param>
        GraphicObjectCPUChart(ISystemScene* pSystemScene, pcstr pszName);

        /// <summary cref="GraphicObjectCPUChart::~GraphicObjectCPUChart">
        ///      Basic destructor; the only one.
        /// </summary>
        ~GraphicObjectCPUChart(void);

        /// <summary cref="GraphicObjectCPUChart::Update">
        ///     Called by the framework every frame, so this window can update itself.
        /// </summary>
        /// <param name="DeltaTime">f32 - Elapsed wall-clock time since the last call to this function.</param>
        virtual void Update(f32 DeltaTime);

    private:
        static const f32                    m_secondsPerUpdate;
        f32                                 m_secondsSinceLastUpdate;
        i32                                 m_framesSinceLastUpdate;
        WindowData                          m_windowData;

        /// <summary cref="GraphicObjectCPUChart::GetWindowDataUpdate">
        ///     Implementation function, used by the Update function.
        /// </summary>
        /// <returns>WindowData* - Result structure containing all data to update in the UI.</returns>
        virtual WindowData*                 GetWindowDataUpdate();
};
