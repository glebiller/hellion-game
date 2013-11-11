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

#include <map>

#include "Proto/Common.pb.h"

/**
 * An interface for environment runtime functionality.
 */
class SettingService {
public:
    /**
     * @inheritDoc
     */
    void add(Proto::Property propertyValue);
    
    /**
     * @inheritDoc
     */
    Proto::Property get(std::string propertyName);
    
    /**
     * @inheritDoc
     */
    std::string getString(std::string propertyName);
    
    /**
     * @inheritDoc
     */
    int getInt(std::string propertyName);
    
    /**
     * @inheritDoc
     */
    bool getBool(std::string propertyName);

private:
    std::map<std::string, Proto::Property>         m_settings;

};