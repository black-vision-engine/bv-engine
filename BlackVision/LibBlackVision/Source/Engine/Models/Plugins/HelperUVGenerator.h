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
	static void scaleVec2( float* min, float* max, std::vector<glm::vec2>& uvs );
	static void scaleVec3( float* min, float* max, std::vector<glm::vec3>& uvws );
public:
	//static void generateU( Float3AttributeChannelPtr verts, Float1AttributeChannelPtr uvs );
	static void generateUV( const glm::vec3 * pos, unsigned int verts_num, Float2AttributeChannelPtr uvs, glm::vec3 versorU, glm::vec3 versorV, bool scale );
	static void generateUVW( const glm::vec3* verts, unsigned int verts_num , Float3AttributeChannelPtr uvs, glm::vec3 versorU, glm::vec3 versorV, glm::vec3 versorW, bool scale );
};

}}}