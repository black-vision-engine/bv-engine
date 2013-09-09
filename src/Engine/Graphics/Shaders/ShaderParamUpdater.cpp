#include "ShaderParamUpdater.h"
#include "ShaderParam.h"

#include "glm/glm.hpp"

namespace bv {

void FakeUpdater::Update( GenericShaderParam * param )
{
    glm::mat4 mat( 1.0f ); //evaluate model

    param->GenericSetVal( &mat[0][0] ); //evaluate engine
}

}
