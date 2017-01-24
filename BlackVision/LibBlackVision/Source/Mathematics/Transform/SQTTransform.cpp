#include "stdafx.h"

#include "Mathematics/Transform/SQTTransform.h"




#include "Memory/MemoryLeaks.h"



namespace bv {


//// *************************************
////
//glm::mat4x4         SQTTransform::Evaluate        ( const glm::vec3 & translation, const glm::quat & rotation, const glm::vec3 & scale, const glm::vec3 & center ) const
//{
//    return	glm::translate( glm::mat4( 1.0f ), translation ) *
//			glm::translate( glm::mat4( 1.0f ), center ) *
//			glm::mat4_cast( rotation ) *
//			glm::scale( glm::mat4( 1.0f ), scale ) *
//			glm::translate( glm::mat4( 1.0f ), -center );
//}

} //bv