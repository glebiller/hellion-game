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

#pragma once

#include <boost/log/sources/logger.hpp>

#include "Generic/IObserver.h"
#include "System/ISystemScene.h"
#include "schema/scene_generated.h"

class UObject;
class ChangeManager;
class SystemService;

/**
 * Implements a universal scene for holding all the scenes of the different systems and acts as
 * an interface into the CMM.
 *
 * @sa  IObserver
 */
class UScene : public IObserver {
public:

    // TODO move
    typedef std::map<std::string, Schema::SceneEntity*>                       Templates;
    typedef std::map<Schema::SystemType, ISystem*>                       Systems;
    typedef std::map<Schema::SystemType, ISystemScene*>                  SystemScenes;
    typedef std::list<UObject*>                                         Objects;

    struct ObjectLinkData {
        ISubject*               pSubject;
        IObserver*              pObserver;
    };
    typedef std::list<ObjectLinkData>       ObjectLinks;

public:

    UScene(ChangeManager* pUSceneCCM, ChangeManager* pUObjectCCM, std::map<Schema::SystemType, ISystem*>& systems);

    ~UScene();

    void init();

    void update();

    /**
     * Creates a new ISystemScene for the passed in ISystem.
     *
     * @param [in,out]  pSystem The system that the scene will be created from.
     *
     * @return  The newly ISystemScene.
     */
    ISystemScene* Extend(ISystem& system, const Schema::SystemScene* pScene);

    /**
     * Destroys a ISystemScene removing it from the UScene.
     *
     * @param [in,out]  pScene  The system scene to destroy.
     *
     * @return  An error code.
     */
    Error Unextend(ISystemScene* pScene);

    /**
     * Returns a collection of system scenes.
     *
     * @return  The system scene collection.
     */
    const SystemScenes& GetSystemScenes() const {
        return m_SystemScenes;
    }

    /**
     * Adds the templates.
     *
     * @param   objects If non-null, the objects.
     */
    void addTemplates(const flatbuffers::Vector<flatbuffers::Offset<Schema::SceneEntity>>* objects);

    /**
     * Creates a new UObject that gets attached to this scene.
     * The object will be extended for each systems.
     *
     * @param   objectProto The object definition to create.
     *
     * @return  The newly created object that has consequently been added to the scene.
     */
    UObject* createSceneEntity(const Schema::SceneEntity& objectProto);

    /**
     * Destroys a UObject removing it from the scene.  It also deletes it's CCM.
     *
     * @param [in,out]  pObject The object to remove from the scene and destroy.
     *
     * @return  An error code.
     */
    Error DestroyObject(UObject* pObject);

    /**
     * Returns a collection of system scenes.
     *
     * @return  The system scene collection.
     */
    const Objects& GetObjects() const {
        return m_Objects;
    }

    /**
     * Searches for the first object.
     *
     * @param   id The unique identifier.
     *
     * @return  null if it fails, else the found object.
     */
    UObject* FindObject(std::string id);

    /**
     * Creates object link.
     *
     * @param [in,out]  pSubject    If non-null, the subject.
     * @param [in,out]  pObserver   If non-null, the observer.
     */
    void CreateObjectLink(ISystemObject* pSubject, ISystemObject* pObserver);

    /**
     * @inheritDoc
     */
    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask SystemChanges);
    
    /**
     * @inheritDoc
     */
    inline System::Changes::BitMask GetDesiredSystemChanges() {
        return System::Changes::Generic::All;
    }

    /**
     * Gets object ccm.
     *
     * @return  null if it fails, else the object ccm.
     */
    inline ChangeManager* getObjectCCM() {
        return m_pObjectCCM;
    }

private:

    void createSystemObject(UObject* pObject, const Schema::SystemComponent* objectProto);

protected:
    boost::log::sources::logger             logger_;

    ChangeManager*                         m_pSceneCCM;
    ChangeManager*                         m_pObjectCCM;

    std::string                             universalSceneData_;
    const Schema::UniversalScene*           universalSceneSchema_;

    SystemScenes                            m_SystemScenes;
    Templates                               m_templates;
    Objects                                 m_Objects;
    ObjectLinks                             m_ObjectLinks;


};
