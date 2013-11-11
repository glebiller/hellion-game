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

#include <string>
#include <list>

/**
 * The <c>IEntity</c> interface supplies id & name management.
 */
class IEntity {
public:
    
    /**
     * Default constructor.
     */
    IEntity(std::string id, std::string name, IEntity* parent);

    /**
     * Destructor.
     * All interfaces must have virtual destructors
     */
    virtual ~IEntity();
    
    /**
     * Get the entity ID.
     */
    inline std::string getId() {
        return m_id;
    }
    
    /**
     * Get the entity name.
     */
    inline std::string getName() {
        return m_name;
    }
    
    /**
     * Get the entity parent.
     */
    inline IEntity* getParent() {
        return m_parent;
    }

    /**
     * Gets the children of this item.
     *
     * @return  null if it fails, else the children.
     */
    inline std::list<IEntity*> getChildren() {
        return m_children;
    }

    /**
     * Adds a children.
     *
     * @param [in,out]  entity  If non-null, the entity.
     */
    void addChildren(IEntity* entity);

protected:
    std::string			    m_id;
    std::string			    m_name;
    IEntity*                m_parent;
    std::list<IEntity*>     m_children;
    
};