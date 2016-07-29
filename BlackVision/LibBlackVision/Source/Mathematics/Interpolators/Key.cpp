#include "stdafx.h"

#include "Key.h"
#include "Key.inl"

#include "Serialization/SerializationHelper.h"

#include "Mathematics/glm_inc.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{
#define INSTANTIATE( TIME_TYPE, TYPE ) \
template class bv::Key< TIME_TYPE, TYPE >;

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