#include "GenericParameterSetters.h"


namespace bv { namespace model {

namespace {

// *******************************
//
bool SetRotation                ( ParamTransformPtr pt, TimeType t, const glm::vec3 & rotAxis, float angle )
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
bool SetScale                   ( ParamTransformPtr pt, TimeType t, const glm::vec3 & scale )
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
bool SetTranslation             ( ParamTransformPtr pt, TimeType t, const glm::vec3 & translation )
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
bool SetCenterMass              ( ParamTransformPtr pt, TimeType t, const glm::vec3 & center )
{
    if( pt == nullptr )
    {
        return false;
    }

    pt->SetCenter( center, t );

    return true;
}

// *******************************
//
bool SetRotation                ( ParamTransformVecPtr pt, unsigned int idx, TimeType t, const glm::vec3 & rotAxis, float angle )
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
bool SetScale                   ( ParamTransformVecPtr pt, unsigned int idx, TimeType t, const glm::vec3 & scale )
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
bool SetTranslation             ( ParamTransformVecPtr pt, unsigned int idx, TimeType t, const glm::vec3 & translation )
{
    if( pt == nullptr || pt->NumTransforms() <= idx )
    {
        return false;
    }

    pt->SetTranslation( idx, translation, t );

    return true;
}

// *******************************
//
bool SetCenterMass              ( ParamTransformVecPtr pt, unsigned int idx, TimeType t, const glm::vec3 & center )
{
    if( pt == nullptr || pt->NumTransforms() <= idx )
    {
        return false;
    }

    pt->SetCenter( idx, center, t );

    return true;
}

} //anonymous


// *******************************
//
bool    SetParameterRotation    ( IParameterPtr parameter, TimeType t, const glm::vec3 & rotAxis, float angle )
{
    return SetRotation( QueryTypedParam< ParamTransformPtr >( parameter ), t, rotAxis, angle );
}

// *******************************
//
bool    SetParameterScale       ( IParameterPtr parameter, TimeType t, const glm::vec3 & scale )
{
    return SetScale( QueryTypedParam< ParamTransformPtr >( parameter ), t, scale );
}

// *******************************
//
bool    SetParameterTranslation ( IParameterPtr parameter, TimeType t, const glm::vec3 & translation )
{
    return SetTranslation( QueryTypedParam< ParamTransformPtr >( parameter ), t, translation );
}

// *******************************
//
bool    SetParameterCenterMass  ( IParameterPtr parameter, TimeType t, const glm::vec3 & center )
{
    return SetCenterMass( QueryTypedParam< ParamTransformPtr >( parameter ), t, center );
}

// *******************************
//
bool    SetParameterRotation    ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & rotAxis, float angle )
{
    return SetRotation( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t, rotAxis, angle );
}

// *******************************
//
bool    SetParameterScale       ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & scale )
{
    return SetScale( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t, scale );
}

// *******************************
//
bool    SetParameterTranslation ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & translation )
{
    return SetTranslation( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t, translation );
}

// *******************************
//
bool    SetParameterCenterMass  ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & center )
{
    return SetCenterMass( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t, center );
}

} //model
} //bv
