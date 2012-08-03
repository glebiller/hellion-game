
#include "TerrainShape.h"

TerrainShape::TerrainShape(const hkpSampledHeightFieldBaseCinfo& ci, hkUint16* data)
    : hkpSampledHeightFieldShape(ci),
      m_data(data) {
}