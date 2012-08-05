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


///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   This is a chart extension for the basic window. It creates a line graph
///   that updates based on any new input received in a manner similar to the CPU
///   usage graph in Windows Task manager. It will also auto scale its y-axis so that
///   all of the data points are visible.
/// </summary>
///////////////////////////////////////////////////////////////////////////////
class GraphicObjectChart : public GraphicObjectWindow {
        friend OGREGraphicsScene;

    public:
        virtual void AddDataPoint(f32);
        void SetMaxRangeText(std::string);
        void SetMinRangeText(std::string);

    protected:

        GraphicObjectChart(ISystemScene* pSystemScene, const char* pszName);
        ~GraphicObjectChart(void);

        /// <summary cref="GraphicObjectChart::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObjectWindow::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectChart::GetDesiredSystemChanges">
        ///   Implementation of the <c>IGeometryObject::GetDesiredSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Types::BitMask - System changes desired by the object.</returns>
        /// <seealso cref="GraphicObjectWindow::GetSystemType"/>
        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        /// <summary cref="GraphicObjectChart::ChangeOccurred">
        ///   Implementation of the <c>IObserver::ChangeOccurred</c> function.
        /// </summary>
        /// <param name="pSubject">Subject of this notification.</param>
        /// <param name="ChangeType">Type of notification for this object.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObjectWindow::ChangeOccurred"/>
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /// <summary cref="GraphicObjectChart::ApplyChanges">
        ///   Update this window with the given data.
        /// </summary>
        /// <param name="WindowData">const WindowData * - New window data</param>
        /// <seealso cref="GraphicObjectWindow::ApplyChanges"/>
        virtual void ApplyChanges(const WindowData*);

        enum { m_numPoints = 101 };
        f32                                 m_xPos[m_numPoints];
        f32                                 m_yPos[m_numPoints];
        f32                                 m_data[m_numPoints];
        f32                                 m_dataMax;
        f32                                 m_dataMin;
        f32                                 m_dataSamples;
        f32                                 m_dataAvg;
        f32                                 m_yAxisMax;

        f32                                 m_chartWidth;
        f32                                 m_chartHeight;
        f32                                 m_chartLeft;
        f32                                 m_chartTop;

        Ogre::TextAreaOverlayElement*       m_pTextAreaYAxisMax;
        Ogre::TextAreaOverlayElement*       m_pTextAreaYAxisMin;

        /// <summary cref="GraphicObjectChart::UpdateChart">
        ///   Update the chart (call this when new data is added).
        /// </summary>
        virtual void UpdateChart(void);
};
