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

#include <list>
#include "Proto/Common.pb.h"

/**
 * <c>IGenericScene</c> is an interface for providing generic scene related functionality. Any
 *  objects that modify scene information are required to implement this class. Should be added to Scenes only.
 */
class ISceneObject {
public:

    typedef std::list<Proto::Object> ObjectProtoQueue;
    
    /**
     * Default constructor.
     */
    ISceneObject();

    /**
     * Destructor.
     */
    virtual ~ISceneObject();
    
    /**
     * Resets the create object lists.
     */
    void resetCreateObjectQueues(void);
    
    /**
     * Resets the delete object lists.
     */
    void resetDeleteObjectQueues(void);

    /**
     * Gets create objects.
     *
     * @return  null if it fails, else the create objects.
     */
    inline const ObjectProtoQueue* getCreateObjects(void) {
        return m_createObjectQueue;
    }

    /**
     * Gets destroy objects.
     *
     * @return  null if it fails, else the destroy objects.
     */
    inline const ObjectProtoQueue* getDeleteObjects(void) {
        return m_deleteObjectQueue;
    }
    
protected:
    ISceneObject::ObjectProtoQueue*          m_createObjectQueue;
    ISceneObject::ObjectProtoQueue*          m_deleteObjectQueue;

};
