#include "stdafx.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"
//#include "../Dep/Common/glm/glm/core/func_geometric.hpp"


#include <limits.h>


#include "Memory/MemoryLeaks.h"



namespace bv{
namespace model{
namespace Helper{

	/**It would be better if that function took Float3AttributeChannelPtr as a parameter than 2 first params instead, 
	but in texture plugin I couldn't cast AttributeChannelPtr to it.*/
    void UVGenerator::GenerateUV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, glm::vec3 versorU, glm::vec3 versorV, bool scale )
    {
        float min[ 2 ] = { std::numeric_limits< float >::max(), std::numeric_limits< float >::max() };
        float max[ 2 ] = { 0.0f, 0.0f };

        auto vertsNum = verts->GetNumEntries();

        std::vector< glm::vec2 > tmpUVs;
        tmpUVs.resize( vertsNum );

        for( unsigned int j = 0; j < vertsNum; ++j )
        {
            glm::vec3 position = verts->GetVertices()[ j ];
            glm::vec2 new_UV( glm::dot( position, versorU ), glm::dot( position, versorV ) );
            tmpUVs[ j ] = new_UV;

            if( scale )
            {
                min[ 0 ] = std::min( min[ 0 ], new_UV.x );
                min[ 1 ] = std::min( min[ 1 ], new_UV.y );
                max[ 0 ] = std::max( max[ 0 ], new_UV.x );
                max[ 1 ] = std::max( max[ 1 ], new_UV.y );
            }
        }

        if( scale )
            ScaleVec2( min, max, tmpUVs );

        uvs->ReplaceAttributes( std::move( tmpUVs ) );
    }

	void UVGenerator::ScaleVec2( float* min, float* max, std::vector<glm::vec2>& uvs )
	{
		glm::vec2 range( max[0] - min[0], max[1] - min[1] );
		glm::vec2 min_value( min[0], min[1] );

		for( unsigned int j = 0; j < uvs.size(); ++j )
		{
			uvs[j] = ( uvs[j] - min_value ) / range;
		}
	}

	void UVGenerator::ScaleVec3( float* min, float* max, std::vector<glm::vec3>& uvws )
	{
		glm::vec3 range( max[0] - min[0], max[1] - min[1], max[2] - min[2] );
		glm::vec3 min_value( min[0], min[1], min[2] );

		for( unsigned int j = 0; j < uvws.size(); ++j )
		{
			uvws[j] = ( uvws[j] - min_value ) / range;
		}
	}

	void UVGenerator::GenerateUVW( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr uvs, glm::vec3 versorU, glm::vec3 versorV, glm::vec3 versorW, bool scale )
	{
		float min[3] = { std::numeric_limits< float >::max(), std::numeric_limits< float >::max(), std::numeric_limits< float >::max() };
		float max[3] = { 0.0f, 0.0f, 0.0f };

        auto vertsNum = verts->GetNumEntries();

		std::vector<glm::vec3> temp_uvs;
		temp_uvs.resize( vertsNum );

		for( unsigned int j = 0; j < vertsNum; ++j )
		{
			glm::vec3 position = verts->GetVertices()[ j ];
			glm::vec3 new_UV( glm::dot( position, versorU ), glm::dot( position, versorV ), glm::dot( position, versorW ) );
			temp_uvs[j] = new_UV;

			if( scale )
			{
				min[0] = std::min( min[0], new_UV.x );
				min[1] = std::min( min[1], new_UV.y );
				min[2] = std::min( min[2], new_UV.z );
				max[0] = std::max( max[0], new_UV.x );
				max[1] = std::max( max[1], new_UV.y );
				max[2] = std::max( max[2], new_UV.z );
			}
		}

		if( scale )
			ScaleVec3( min, max, temp_uvs );

        uvs->ReplaceAttributes( std::move( temp_uvs ) );
	}


}}}