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

// ***********************
//
bool    RemoveRotationKey       ( ParamTransformVecPtr parameter, unsigned int idx, TimeType t )
{
    if( parameter == nullptr )
        return false;

    parameter->RemoveRotation( idx, t );
    return true;
}

// ***********************
//
bool    RemoveScaleKey          ( ParamTransformVecPtr parameter, unsigned int idx, TimeType t )
{
    if( parameter == nullptr )
        return false;

    parameter->RemoveScale( idx, t );
    return true;
}

// ***********************
//
bool    RemoveTranslationKey    ( ParamTransformVecPtr parameter, unsigned int idx, TimeType t )
{
    if( parameter == nullptr )
        return false;

    parameter->RemoveTranslation( idx, t );
    return true;
}

// ***********************
//
bool    RemoveCenterMassKey     ( ParamTransformVecPtr parameter, unsigned int idx, TimeType t )
{
    if( parameter == nullptr )
        return false;

    parameter->RemoveCenter( idx, t );
    return true;
}

// ***********************
//
bool    RemoveRotationKey       ( ParamTransformPtr parameter, TimeType t )
{
    if( parameter == nullptr )
        return false;

    parameter->RemoveRotation( t );

    return true;
}

// ***********************
//
bool    RemoveScaleKey          ( ParamTransformPtr parameter, TimeType t )
{
    if( parameter == nullptr )
        return false;

    parameter->RemoveScale( t );
    return true;
}

// ***********************
//
bool    RemoveTranslationKey    ( ParamTransformPtr parameter, TimeType t )
{
    if( parameter == nullptr )
        return false;

    parameter->RemoveTranslation( t );
    return true;
}

// ***********************
//
bool    RemoveCenterMassKey     ( ParamTransformPtr parameter, TimeType t )
{
    if( parameter == nullptr )
        return false;

    parameter->RemoveCenter( t );
    return true;
}

//
//
//

// ***********************
//
bool    MoveRotationKey       ( ParamTransformVecPtr parameter, unsigned int idx, TimeType t, TimeType newTime )
{
    if( parameter == nullptr )
        return false;

    parameter->MoveRotation( idx, t, newTime );
    return true;
}

// ***********************
//
bool    MoveScaleKey          ( ParamTransformVecPtr parameter, unsigned int idx, TimeType t, TimeType newTime )
{
    if( parameter == nullptr )
        return false;

    parameter->MoveScale( idx, t, newTime );
    return true;
}

// ***********************
//
bool    MoveTranslationKey    ( ParamTransformVecPtr parameter, unsigned int idx, TimeType t, TimeType newTime )
{
    if( parameter == nullptr )
        return false;

    parameter->MoveTranslation( idx, t, newTime );
    return true;
}

// ***********************
//
bool    MoveCenterMassKey     ( ParamTransformVecPtr parameter, unsigned int idx, TimeType t, TimeType newTime )
{
    if( parameter == nullptr )
        return false;

    parameter->MoveCenter( idx, t, newTime );
    return true;
}

// ***********************
//
bool    MoveRotationKey       ( ParamTransformPtr parameter, TimeType t, TimeType newTime )
{
    if( parameter == nullptr )
        return false;

    parameter->MoveRotation( t, newTime );

    return true;
}

// ***********************
//
bool    MoveScaleKey          ( ParamTransformPtr parameter, TimeType t, TimeType newTime )
{
    if( parameter == nullptr )
        return false;

    parameter->MoveScale( t, newTime );
    return true;
}

// ***********************
//
bool    MoveTranslationKey    ( ParamTransformPtr parameter, TimeType t, TimeType newTime )
{
    if( parameter == nullptr )
        return false;

    parameter->MoveTranslation( t, newTime );
    return true;
}

// ***********************
//
bool    MoveCenterMassKey     ( ParamTransformPtr parameter, TimeType t, TimeType newTime )
{
    if( parameter == nullptr )
        return false;

    parameter->MoveCenter( t, newTime );
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

// ========================================================================= //
// Remove transformation keys
// ========================================================================= //

// ***********************
//
bool    RemoveRotationKey       ( IParameterPtr parameter, unsigned int idx, TimeType t )
{    return RemoveRotationKey( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t );  }

// ***********************
//
bool    RemoveScaleKey          ( IParameterPtr parameter, unsigned int idx, TimeType t )
{    return RemoveScaleKey( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t );  }
// ***********************
//
bool    RemoveTranslationKey    ( IParameterPtr parameter, unsigned int idx, TimeType t )
{    return RemoveTranslationKey( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t );  }

// ***********************
//
bool    RemoveCenterMassKey     ( IParameterPtr parameter, unsigned int idx, TimeType t )
{    return RemoveCenterMassKey( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t );  }



// ***********************
//
bool    RemoveRotationKey       ( IParameterPtr parameter, TimeType t )
{    return RemoveRotationKey( QueryTypedParam< ParamTransformPtr >( parameter ), t );  }

// ***********************
//
bool    RemoveScaleKey          ( IParameterPtr parameter, TimeType t )
{    return RemoveScaleKey( QueryTypedParam< ParamTransformPtr >( parameter ), t );  }
// ***********************
//
bool    RemoveTranslationKey    ( IParameterPtr parameter, TimeType t )
{    return RemoveTranslationKey( QueryTypedParam< ParamTransformPtr >( parameter ), t );  }

// ***********************
//
bool    RemoveCenterMassKey     ( IParameterPtr parameter, TimeType t )
{    return RemoveCenterMassKey( QueryTypedParam< ParamTransformPtr >( parameter ), t );  }


// ========================================================================= //
// Move transformation key
// ========================================================================= //

// ***********************
//
bool    MoveRotationKey         ( IParameterPtr parameter, unsigned int idx, TimeType t, TimeType newTime )
{    return MoveRotationKey( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t, newTime );  }

// ***********************
//
bool    MoveScaleKey          ( IParameterPtr parameter, unsigned int idx, TimeType t, TimeType newTime )
{    return MoveScaleKey( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t, newTime );  }
// ***********************
//
bool    MoveTranslationKey    ( IParameterPtr parameter, unsigned int idx, TimeType t, TimeType newTime )
{    return MoveTranslationKey( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t, newTime );  }

// ***********************
//
bool    MoveCenterMassKey     ( IParameterPtr parameter, unsigned int idx, TimeType t, TimeType newTime )
{    return MoveCenterMassKey( QueryTypedParam< ParamTransformVecPtr >( parameter ), idx, t, newTime );  }



// ***********************
//
bool    MoveRotationKey       ( IParameterPtr parameter, TimeType t, TimeType newTime )
{    return MoveRotationKey( QueryTypedParam< ParamTransformPtr >( parameter ), t, newTime );  }

// ***********************
//
bool    MoveScaleKey          ( IParameterPtr parameter, TimeType t, TimeType newTime )
{    return MoveScaleKey( QueryTypedParam< ParamTransformPtr >( parameter ), t, newTime );  }
// ***********************
//
bool    MoveTranslationKey    ( IParameterPtr parameter, TimeType t, TimeType newTime )
{    return MoveTranslationKey( QueryTypedParam< ParamTransformPtr >( parameter ), t, newTime );  }

// ***********************
//
bool    MoveCenterMassKey     ( IParameterPtr parameter, TimeType t, TimeType newTime )
{    return MoveCenterMassKey( QueryTypedParam< ParamTransformPtr >( parameter ), t, newTime );  }


// ========================================================================= //
// Curve types and wrap methods
// ========================================================================= //

// *******************************
//
bool                                                BezierSetCurveType( IParameterPtr parameter, CurveType type )
{
    auto abstract_parameter = std::dynamic_pointer_cast< AbstractModelParameter >( parameter ); // FIXME

    if( abstract_parameter )
    {
        abstract_parameter->SetCurveType( type );
        return true;
    }
    else
        return false;
}

// *******************************
//
CurveType                                           BezierGetCurveType( IParameterPtr parameter )
{
    auto abstract_parameter = std::dynamic_pointer_cast< AbstractModelParameter >( parameter ); // FIXME

    if( abstract_parameter )
        return abstract_parameter->GetCurveType();
    else
        return CurveType::CT_TOTAL;
}

// *******************************
//
bool                                                SetWrapPostMethod  ( IParameterPtr parameter, WrapMethod method )
{
    auto abstract_parameter = std::dynamic_pointer_cast< AbstractModelParameter >( parameter ); // FIXME

    if( abstract_parameter )
    {
        abstract_parameter->SetWrapPostMethod( method );
        return true;
    }
    else
        return false;
}

// *******************************
//
bool                                                SetWrapPreMethod   ( IParameterPtr parameter, WrapMethod method )
{
    auto abstract_parameter = std::dynamic_pointer_cast< AbstractModelParameter >( parameter ); // FIXME

    if( abstract_parameter )
    {
        abstract_parameter->SetWrapPreMethod( method );
        return true;
    }
    else
        return false;
}

// *******************************
//
WrapMethod                                          GetWrapPostMethod  ( IParameterPtr parameter )
{
    auto abstract_parameter = std::dynamic_pointer_cast< AbstractModelParameter >( parameter ); // FIXME

    if( abstract_parameter )
        return abstract_parameter->GetWrapPostMethod();
    else
        return WrapMethod( -1 );
}

// *******************************
//
WrapMethod                                          GetWrapPreMethod   ( IParameterPtr parameter )
{
    auto abstract_parameter = std::dynamic_pointer_cast< AbstractModelParameter >( parameter ); // FIXME

    if( abstract_parameter )
        return abstract_parameter->GetWrapPreMethod();
    else
        return WrapMethod( -1 );
}

// *******************************
//
int                                                 BezierParameterGetNumKeys( IParameterPtr parameter )
{
    auto abstract_parameter = std::dynamic_pointer_cast< AbstractModelParameter >( parameter ); // FIXME

    if( abstract_parameter )
        return abstract_parameter->GetNumKeys();
    else
        return -1;
}

// ***********************
//
bool RemoveParameterKey ( IParameterPtr parameter, TimeType t )
{
    auto paramType = parameter->GetType();
    switch( paramType )
    {
        case ModelParamType::MPT_FLOAT:
            return RemoveTypedParamKey<ModelParamType::MPT_FLOAT>( parameter, t );
        case ModelParamType::MPT_VEC2:
            return RemoveTypedParamKey<ModelParamType::MPT_VEC2>( parameter, t );
        case ModelParamType::MPT_VEC3:
            return RemoveTypedParamKey<ModelParamType::MPT_VEC3>( parameter, t );
        case ModelParamType::MPT_VEC4:
            return RemoveTypedParamKey<ModelParamType::MPT_VEC4>( parameter, t );
        case ModelParamType::MPT_WSTRING:
            return RemoveTypedParamKey<ModelParamType::MPT_WSTRING>( parameter, t );
        case ModelParamType::MPT_STRING:
            return RemoveTypedParamKey<ModelParamType::MPT_STRING>( parameter, t );
        case ModelParamType::MPT_INT:
            return RemoveTypedParamKey<ModelParamType::MPT_INT>( parameter, t );
        case ModelParamType::MPT_BOOL:
            return RemoveTypedParamKey<ModelParamType::MPT_BOOL>( parameter, t );
        case ModelParamType::MPT_ENUM:
            return RemoveTypedParamKey<ModelParamType::MPT_ENUM>( parameter, t );
        case ModelParamType::MPT_MAT2:
            return RemoveTypedParamKey<ModelParamType::MPT_MAT2>( parameter, t );
    }
    return false;
}

// ***********************
//
bool MoveParameterKey   ( IParameterPtr parameter, TimeType t, TimeType newTime )
{
    auto paramType = parameter->GetType();
    switch( paramType )
    {
        case ModelParamType::MPT_FLOAT:
            return MoveTypedParamKey<ModelParamType::MPT_FLOAT>( parameter, t, newTime );
        case ModelParamType::MPT_VEC2:
            return MoveTypedParamKey<ModelParamType::MPT_VEC2>( parameter, t, newTime );
        case ModelParamType::MPT_VEC3:
            return MoveTypedParamKey<ModelParamType::MPT_VEC3>( parameter, t, newTime );
        case ModelParamType::MPT_VEC4:
            return MoveTypedParamKey<ModelParamType::MPT_VEC4>( parameter, t, newTime );
        case ModelParamType::MPT_WSTRING:
            return MoveTypedParamKey<ModelParamType::MPT_WSTRING>( parameter, t, newTime );
        case ModelParamType::MPT_STRING:
            return MoveTypedParamKey<ModelParamType::MPT_STRING>( parameter, t, newTime );
        case ModelParamType::MPT_INT:
            return MoveTypedParamKey<ModelParamType::MPT_INT>( parameter, t, newTime );
        case ModelParamType::MPT_BOOL:
            return MoveTypedParamKey<ModelParamType::MPT_BOOL>( parameter, t, newTime );
        case ModelParamType::MPT_ENUM:
            return MoveTypedParamKey<ModelParamType::MPT_ENUM>( parameter, t, newTime );
        case ModelParamType::MPT_MAT2:
            return MoveTypedParamKey<ModelParamType::MPT_MAT2>( parameter, t, newTime );
    }
    return false;
}

} //model
} //bv
