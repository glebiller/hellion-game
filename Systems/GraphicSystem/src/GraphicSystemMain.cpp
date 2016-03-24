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

#include <boost/dll.hpp>

#include <Generic/Framework.h>
#include "System/ISystem.h"
#include "GraphicSystemSystem.h"

extern "C" ISystem* BOOST_SYMBOL_EXPORT CreateSystem(Framework* framework) {
    return new GraphicSystem(framework);
}

extern "C" void BOOST_SYMBOL_EXPORT DestroySystem(ISystem* pSystem) {
    delete reinterpret_cast<GraphicSystem*>(pSystem);
}

