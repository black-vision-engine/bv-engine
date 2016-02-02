#include "CompositeTypedParameters.h"

namespace bv { namespace model {

// ******************************************************************************************

// *******************************
//
ParamTransform::ParamTransform  ( const std::string & name, const CompositeTransform & transform, ITimeEvaluatorPtr evaluator )
    : AbstractModelParameter( name, ModelParamType::MPT_TRANSFORM, evaluator )
    , m_transformModel( transform )
{
}


// *******************************
// FIXME: reimplement with SQT paramter model
void                ParamTransform::SetGlobalCurveType    ( CurveType type ) 
{ 
    m_transformModel.SetCurveType( type ); 
}

// *******************************
// FIXME: reimplement with SQT paramter model
void                ParamTransform::SetAddedKeyCurveType    ( CurveType type )
{
    m_transformModel.SetAddedKeyCurveType( type );
}

// *******************************
// FIXME: reimplement with SQT paramter model
CurveType           ParamTransform::GetCurveType    ()
{
    assert( false );
    return CurveType::CT_TOTAL;
}

// *******************************
// FIXME: reimplement with SQT paramter model
void        ParamTransform::SetWrapPostMethod ( WrapMethod method )
{
    m_transformModel.SetWrapPostMethod( method );
}

// *******************************
// FIXME: reimplement with SQT paramter model
void        ParamTransform::SetWrapPreMethod ( WrapMethod method )
{
    m_transformModel.SetWrapPreMethod( method );
}

// *******************************
// FIXME: reimplement with SQT paramter model
WrapMethod  ParamTransform::GetWrapPostMethod ()
{
    assert( false );
    return WrapMethod( -1 );
}

// *******************************
// FIXME: reimplement with SQT paramter model
WrapMethod  ParamTransform::GetWrapPreMethod ()
{
    assert( false );
    return WrapMethod( -1 );
}

// *******************************
// FIXME: reimplement with SQT paramter model
int         ParamTransform::GetNumKeys      ()
{
    assert( false );
    return -1;
}

// *******************************
//
void    ParamTransform::Serialize       ( ISerializer& ser ) const
{
    ser.EnterChild( "param" );
    ser.SetAttribute( "name", GetName() );
    ser.SetAttribute( "type", "transform" );

    auto timeline = TimelineManager::GetInstance()->GetTimelinePath( m_timeEvaluator );
    ser.SetAttribute( "timeline", timeline );

    m_transformModel.Serialize( ser );

    ser.ExitChild();
}

// *******************************
//
VoidPtr    ParamTransform::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

// *************************************
//
const CompositeTransform &      ParamTransform::GetTransform   () const
{
    return m_transformModel;
}

// *************************************
//
ModelParamType                  ParamTransform::Type            ()
{
    return ModelParamType::MPT_TRANSFORM;
}

} //model
} //bv
