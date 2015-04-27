#include "Engine/Models/Plugins/HelperUVGenerator.h"
//#include "../Dep/Common/glm/glm/core/func_geometric.hpp"



namespace bv{
namespace model{
namespace Helper{


	void UVGenerator::generateUV( const glm::vec3 * pos, unsigned int verts_num, Float2AttributeChannelPtr uvs, glm::vec3 versorU, glm::vec3 versorV, bool scale )
	{
		float min[2] = { 100000.0f, 100000.0f };
		float max[2] = { 0.0f, 0.0f };

		std::vector<glm::vec2> temp_uvs;
		temp_uvs.resize( verts_num );

		for( unsigned int j = 0; j < verts_num; ++j )
		{
			glm::vec3 position = pos[j];
			glm::vec2 new_UV( glm::dot( position, versorU ), glm::dot( position, versorV ) );
			temp_uvs[j] = new_UV;

			if( scale )
			{
				min[0] = std::min( min[0], new_UV.x );
				min[1] = std::min( min[1], new_UV.y );
				max[0] = std::max( max[0], new_UV.x );
				max[1] = std::max( max[1], new_UV.y );
			}
		}

		if( scale )
			scaleVec2( min, max, temp_uvs );

		for( unsigned int j = 0; j < verts_num; ++j )
			uvs->AddAttribute( temp_uvs[j] );
	}

	void UVGenerator::scaleVec2( float* min, float* max, std::vector<glm::vec2>& uvs )
	{
		glm::vec2 range( max[0] - min[0], max[1] - min[1] );
		glm::vec2 min_value( min[0], min[1] );

		for( unsigned int j = 0; j < uvs.size(); ++j )
		{
			uvs[j] = ( uvs[j] - min_value ) / range;
		}
	}

	void UVGenerator::scaleVec3( float* min, float* max, std::vector<glm::vec3>& uvws )
	{
		glm::vec3 range( max[0] - min[0], max[1] - min[1], max[2] - min[2] );
		glm::vec3 min_value( min[0], min[1], min[2] );

		for( unsigned int j = 0; j < uvws.size(); ++j )
		{
			uvws[j] = ( uvws[j] - min_value ) / range;
		}
	}

	void UVGenerator::generateUVW( const glm::vec3* pos, unsigned int verts_num , Float3AttributeChannelPtr uvs, glm::vec3 versorU, glm::vec3 versorV, glm::vec3 versorW, bool scale )
	{
		float min[3] = { 100000.0f, 100000.0f, 100000.0f };
		float max[3] = { 0.0f, 0.0f, 0.0f };

		std::vector<glm::vec3> temp_uvs;
		temp_uvs.resize( verts_num );

		for( unsigned int j = 0; j < verts_num; ++j )
		{
			glm::vec3 position = pos[j];
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
			scaleVec3( min, max, temp_uvs );

		for( unsigned int j = 0; j < verts_num; ++j )
			uvs->AddAttribute( temp_uvs[j] );
	}


}}}