#pragma once

#include "Mathematics/glm_inc.h"




namespace bv {

class SQTTransform
{
public:
	static inline glm::mat4x4 Evaluate( const glm::vec3 & translation, const glm::quat & rotation, const glm::vec3 & scale, const glm::vec3 & center )
	{
		//glm::detail::fmat4x4SIMD translationM = glm::detail::fmat4x4SIMD( 1.f );
		//translationM[ 3 ] = glm::detail::fvec4SIMD( translation.x, translation.y, translation.z, 1.f );

		//glm::detail::fmat4x4SIMD centerM = glm::detail::fmat4x4SIMD( 1.f ); 
		//centerM[ 3 ] = glm::detail::fvec4SIMD( center.x, center.y, center.z, 1.f );

		//glm::detail::fmat4x4SIMD centerInvM = glm::detail::fmat4x4SIMD( 1.f );
		//centerInvM[ 3 ] = glm::detail::fvec4SIMD( -center.x, -center.y, -center.z, 1.f );

		//glm::detail::fmat4x4SIMD rotationM = glm::detail::fmat4x4SIMD( glm::mat4_cast( rotation ) );

		//glm::detail::fmat4x4SIMD scaleM = glm::detail::fmat4x4SIMD( scale.x,	0.f,		0.f,		0.f,
		//															0.f,		scale.y,	0.f,		0.f,
		//															0.f,		0.f,		scale.z,	0.f,
		//															0.f,		0.f,		0.f,		1.f );

		//return glm::mat4_cast( translationM * centerM * rotationM * scaleM * centerInvM );

		return	glm::translate( glm::mat4( 1.0f ), translation ) *
			glm::translate( glm::mat4( 1.0f ), center ) *
			glm::mat4_cast( rotation ) *
			glm::scale( glm::mat4( 1.0f ), scale ) *
			glm::translate( glm::mat4( 1.0f ), -center );
	}

};

}