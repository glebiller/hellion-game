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

#include <cstring>
#include "boost/algorithm/string.hpp"

#include "BaseTypes.h"
#include "Interface.h"

#include "EnvironmentManager.h"


EnvironmentManager::EnvironmentManager(
    void
)
    : m_RuntimeStatus(IEnvironment::IRuntime::Status::Unknown) {
}


EnvironmentManager::~EnvironmentManager(
    void
) {
}


EnvironmentManager::Variables::Variables(
    void
) {
}


void
EnvironmentManager::Variables::Add(
    In char* pszName,
    In char* pszValue
) {
    Variables::Pair vp;
    vp.sName = pszName;
    vp.sValue = pszValue;
    m_Pairs.push_back(vp);
}

// Set the value of an environment variable that already exists.
// Environment variables are defined in Smoke.gdf.
void
EnvironmentManager::Variables::SetValue(
    In char* pszName,
    In char* pszValue
) {
    for (PairsIt it = m_Pairs.begin(); it != m_Pairs.end(); it++) {
        Pair& vp = *it;

        if (vp.sName == pszName) {
            vp.sValue = pszValue;
            break;
        }
    }
}


const char*
EnvironmentManager::Variables::GetAsString(
    In char* pszName,
    In char* pszDefaultValue
) {
    const char* pszValue = pszDefaultValue;
    GetValue(pszName, pszValue);
    return pszValue;
}


bool
EnvironmentManager::Variables::IsString(
    In char* pszName,
    In char* pszCompareValue
) {
    bool bValue = false;
    const char* pszValue = NULL;
    GetValue(pszName, pszValue);

    if (pszValue != NULL) {
        bValue = boost::iequals(pszValue, pszCompareValue);
    }

    return bValue;
}


bool
EnvironmentManager::Variables::GetAsBool(
    In char* pszName,
    In bool bDefaultValue
) {
    bool bValue = bDefaultValue;
    const char* pszValue;

    if (GetValue(pszName, pszValue)) {
        ASSERT(!boost::iequals(pszValue, "1") || !boost::iequals(pszValue, "0"));
        bValue = boost::iequals(pszValue, "1");
    }

    return bValue;
}


i32
EnvironmentManager::Variables::GetAsInt(
    In char* pszName,
    In i32 DefaultValue
) {
    i32 Value = DefaultValue;
    const char* pszValue;

    if (GetValue(pszName, pszValue)) {
        Value = atoi(pszValue);
    }

    return Value;
}


f32
EnvironmentManager::Variables::GetAsFloat(
    In char* pszName,
    In f32 DefaultValue
) {
    f32 Value = DefaultValue;
    const char* pszValue;

    if (GetValue(pszName, pszValue)) {
        Value = static_cast<f32>(atof(pszValue));
    }

    return Value;
}


bool
EnvironmentManager::Variables::GetValue(
    In char* pszName,
    Out const char*& pszValue
) {
    bool bFound = false;

    for (PairsIt it = m_Pairs.begin(); it != m_Pairs.end(); it++) {
        const Pair& vp = *it;

        if (vp.sName == pszName) {
            pszValue = vp.sValue.c_str();
            bFound = true;
            break;
        }
    }

    return bFound;
}


IEnvironment::IRuntime::Status
EnvironmentManager::GetStatus(
    void
) {
    return m_RuntimeStatus;
}


void EnvironmentManager::SetStatus(IEnvironment::IRuntime::Status Status) {
#if defined(MSC_COMPILER)
    ::InterlockedExchange((i32*)&m_RuntimeStatus, Status);
#endif
}
