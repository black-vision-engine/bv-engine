#pragma once

#include "Mathematics/glm_inc.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"

namespace bv { namespace model
{

class TextArranger
{
private:
    typedef glm::vec3 ( * FuncFloat2Vec3Type )( float );

    FuncFloat2Vec3Type m_position;
    FuncFloat2Vec3Type m_tangent;
    FuncFloat2Vec3Type m_norm;

    glm::vec3                           EvalutePosition ( float x, float xmin, float xmax );
    glm::mat4                           EvaluteRotation ( float x, float xmin, float xmax );
    glm::vec3                           EvaluteNormal   ( float x, float xmin, float xmax );

public:

    explicit                            TextArranger( FuncFloat2Vec3Type position, FuncFloat2Vec3Type tangent, FuncFloat2Vec3Type norm );
    explicit                            TextArranger( FuncFloat2Vec3Type position, FuncFloat2Vec3Type tangent );
    explicit                            TextArranger( FuncFloat2Vec3Type position );

    model::VertexAttributesChannel *    Arange( model::VertexAttributesChannel * in );
};

extern TextArranger CircleArranger;

} // model
} // bv