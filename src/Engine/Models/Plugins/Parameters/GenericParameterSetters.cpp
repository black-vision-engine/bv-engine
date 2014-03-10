#include "GenericParameterSetters.h"


namespace bv { namespace model {

namespace {

// *******************************
//
bool SetRotation                ( ParamTransform * pt, TimeType t, const glm::vec3 & rotAxis, float angle )
{
    if( pt == nullptr )
    {
        return false;
    }

    pt->SetRotation( rotAxis, angle, t );

    return true;
}

// *******************************
//
bool SetScale                   ( ParamTransform * pt, TimeType t, const glm::vec3 & scale )
{
    if( pt == nullptr )
    {
        return false;
    }

    pt->SetScale( scale, t );

    return true;
}

// *******************************
//
bool SetTranslation             ( ParamTransform * pt, TimeType t, const glm::vec3 & translation )
{
    if( pt == nullptr )
    {
        return false;
    }

    pt->SetTranslation( translation, t );

    return true;
}

// *******************************
//
bool SetRotation                ( ParamTransformVec * pt, unsigned int idx, TimeType t, const glm::vec3 & rotAxis, float angle )
{
    if( pt == nullptr || pt->NumTransforms() <= idx )
    {
        return false;
    }

    pt->SetRotation( idx, rotAxis, angle, t );

    return true;
}

// *******************************
//
bool SetScale                   ( ParamTransformVec * pt, unsigned int idx, TimeType t, const glm::vec3 & scale )
{
    if( pt == nullptr || pt->NumTransforms() <= idx )
    {
        return false;
    }

    pt->SetScale( idx, scale, t );

    return true;
}

// *******************************
//
bool SetTranslation             ( ParamTransformVec * pt, unsigned int idx, TimeType t, const glm::vec3 & translation )
{
    if( pt == nullptr || pt->NumTransforms() <= idx )
    {
        return false;
    }

    pt->SetTranslation( idx, translation, t );

    return true;
}

} //anonymous


// *******************************
//
bool    SetParameterRotation    ( IParameter * parameter, TimeType t, const glm::vec3 & rotAxis, float angle )
{
    return SetRotation( QueryTypedParam< ParamTransform >( parameter ), t, rotAxis, angle );
}

// *******************************
//
bool    SetParameterScale       ( IParameter * parameter, TimeType t, const glm::vec3 & scale )
{
    return SetScale( QueryTypedParam< ParamTransform >( parameter ), t, scale );
}

// *******************************
//
bool    SetParameterTranslation ( IParameter * parameter, TimeType t, const glm::vec3 & translation )
{
    return SetTranslation( QueryTypedParam< ParamTransform >( parameter ), t, translation );
}


// *******************************
//
bool    SetParameterRotation    ( IParameter * parameter, unsigned int idx, TimeType t, const glm::vec3 & rotAxis, float angle )
{
    return SetRotation( QueryTypedParam< ParamTransformVec >( parameter ), idx, t, rotAxis, angle );
}

// *******************************
//
bool    SetParameterScale       ( IParameter * parameter, unsigned int idx, TimeType t, const glm::vec3 & scale )
{
    return SetScale( QueryTypedParam< ParamTransformVec >( parameter ), idx, t, scale );
}

// *******************************
//
bool    SetParameterTranslation ( IParameter * parameter, unsigned int idx, TimeType t, const glm::vec3 & translation )
{
    return SetTranslation( QueryTypedParam< ParamTransformVec >( parameter ), idx, t, translation );
}

} //model
} //bv
