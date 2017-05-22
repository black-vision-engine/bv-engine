#pragma once

#include "Mathematics/glm_inc.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"


namespace bv{
namespace model{
namespace Helper
{

class UVGenerator
{
private:
	static void         ScaleVec2       ( float* min, float* max, std::vector<glm::vec2>& uvs );
	static void         ScaleVec3       ( float* min, float* max, std::vector<glm::vec3>& uvws );
public:
	//static void generateU( Float3AttributeChannelPtr verts, Float1AttributeChannelPtr uvs );
	static void         GenerateUV      ( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs, glm::vec3 versorU, glm::vec3 versorV, bool scale );
	static void         GenerateUVW     ( Float3AttributeChannelPtr verts, Float3AttributeChannelPtr uvs, glm::vec3 versorU, glm::vec3 versorV, glm::vec3 versorW, bool scale );

    static void         GenerateUV      ( const std::vector< glm::vec3 > & verts, std::vector< glm::vec2 > & uvs, glm::vec3 versorU, glm::vec3 versorV, bool scale );
    static void         GenerateUVW     ( const std::vector< glm::vec3 > & verts, std::vector< glm::vec3 > & uvs, glm::vec3 versorU, glm::vec3 versorV, glm::vec3 versorW, bool scale );

    static void         GenerateUV      ( const std::vector< glm::vec3 > & verts, SizeType startRange, SizeType endRange, std::vector< glm::vec2 > & uvs, glm::vec3 versorU, glm::vec3 versorV, bool scale );
};

}}}