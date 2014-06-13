#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "System/BasicTypes.h"


namespace bv {

class RectNodeBuilder
{
private:

    model::ParamFloat       m_w;
    model::ParamFloat       m_h;
    model::ParamTransform   m_transform;

protected:

    RectBuilder             ( float w = 1.0f, float h = 1.0f, TimeType t = TimeType( 0.0 ) );

public:

    ~RectBuilder            ();

    void        SetW        ( float w, TimeType t = TimeType( 0.0 ) );
    void        SetH        ( float w, TimeType t = TimeType( 0.0 ) );

    void        SetPosition ( const glm::vec3 & pos, TimeType t = TimeType( 0.0 ) );
    void        SetRotation ( const glm::vec3 & axis, float angle, TimeType t = TimeType( 0.0 ) );
    void        SetScale    ( const glm::vec3 & scl, TimeType t = TimeType( 0.0 ) );

};

class SolidRectNodeBuilder : public RectNodeBuilder
{
private:

    model::ParamVec4        m_color;

public:

    SolidRectNodeBuilder ( const glm::vec4 & color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), float w = 1.0f, float h = 1.0f, TimeType t = TimeType( 0.0 ) );
    SolidRectNodeBuilder ( const glm::vec4 & color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), float w = 1.0f, float h = 1.0f, TimeType t = TimeType( 0.0 ) );
};


} //bv
