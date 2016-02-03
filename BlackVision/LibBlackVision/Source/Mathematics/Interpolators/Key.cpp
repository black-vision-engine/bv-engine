#include "stdafx.h"

#include "Key.inl"

#include "Serialization/SerializationHelper.h"

namespace bv
{
#define INSTANTIATE( TIME_TYPE, TYPE ) \
template bv::Key< TIME_TYPE, TYPE >;

INSTANTIATE( TimeType, Float32 )
INSTANTIATE( TimeType, glm::vec2 )
INSTANTIATE( TimeType, glm::vec3 )
INSTANTIATE( TimeType, glm::vec4 )
INSTANTIATE( TimeType, Int32 )
INSTANTIATE( TimeType, bool )
INSTANTIATE( TimeType, std::string )
INSTANTIATE( TimeType, std::wstring )

#undef INSTANTIATE 

} // bv