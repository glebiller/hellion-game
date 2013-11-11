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

#include <boost/lexical_cast.hpp>

#include "Defines.h"
#include "Service/SettingService.h"

/**
 * @inhertiDoc
 */
void SettingService::add(Proto::Property propertyValue) {
    m_settings[propertyValue.name()] = propertyValue;
}

/**
 * @inhertiDoc
 */
Proto::Property SettingService::get(std::string propertyName) {
    auto iterator = m_settings.find(propertyName);
    if (iterator == m_settings.end()) {
        return Proto::Property();
    }
    return iterator->second;
}

/**
 * @inhertiDoc
 */
std::string SettingService::getString(std::string propertyName) {
    Proto::Property property = get(propertyName);
    if (property.value().size() == 0) {
        return "";
    }
    return property.value().Get(0);
}

/**
 * @inhertiDoc
 */
int SettingService::getInt(std::string propertyName) {
    Proto::Property property = get(propertyName);
    if (property.value().size() == 0) {
        return 0;
    }
    return boost::lexical_cast<int>(property.value().Get(0));
}


/**
 * @inhertiDoc
 */
bool SettingService::getBool(std::string propertyName) {
    Proto::Property property = get(propertyName);
    if (property.value().size() == 0) {
        return false;
    }
    return boost::lexical_cast<bool>(property.value().Get(0));
}
