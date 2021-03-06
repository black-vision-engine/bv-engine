#include "stdafx.h"

#include "VacAABB.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannelDescriptor.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// ******************************
//
bool				AABB( const IVertexAttributesChannel * vac, const glm::mat4 & trans, mathematics::Rect * rect )
{
	auto desc = vac->GetDescriptor();

	if( desc->GetNumVertexChannels() > 0 )
		if( desc->GetAttrChannelDescriptor( 0 )->GetSemantic() == AttributeSemantic::AS_POSITION )
			if( desc->GetAttrChannelDescriptor( 0 )->GetType() == AttributeType::AT_FLOAT3 )
			{
				//auto numOfVerts = vac->TotalNumVertices();

				auto ccs = vac->GetComponents();

				for( auto cc : ccs )
				{
					auto numVerts = cc->GetNumVertices();

					const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( cc->GetAttributeChannels()[ 0 ]->GetData() );

					for( unsigned int i = 0; i < numVerts; ++i )
					{
						auto transformed = trans * glm::vec4( pos[ i ].x, pos[ i ].y, pos[ i ].z, 1.f );
						rect->Include( glm::vec2( transformed ) );
					}
				}

				return true;
			}

	return false;
}

} // model
} // bv