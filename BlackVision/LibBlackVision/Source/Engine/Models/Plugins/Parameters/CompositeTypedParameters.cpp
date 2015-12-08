#include "CompositeTypedParameters.h"

namespace bv { namespace model {

// ******************************************************************************************

// *******************************
//
ParamTransform::ParamTransform  ( const std::string & name, const TransformF & transform, ITimeEvaluatorPtr evaluator )
    : AbstractModelParameter( name, ModelParamType::MPT_TRANSFORM, evaluator )
    , m_transformModel( transform )
{
}


// *******************************
// FIXME: reimplement with SQT paramter model
void                ParamTransform::SetCurveType    ( CurveType type ) 
{ 
    m_transformModel.SetCurveType( type ); 
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


// ******************************************************************************************

// *******************************
//
ParamTransformVec::ParamTransformVec                ( const std::string & name, const TransformF & transform, ITimeEvaluatorPtr evaluator )
    : AbstractModelParameter( name, ModelParamType::MPT_TRANSFORM_VEC, evaluator )
{
    AppendTransform( transform );
}

// *******************************
//
ParamTransformVec::ParamTransformVec                ( const std::string & name, ITimeEvaluatorPtr evaluator )
    : AbstractModelParameter( name, ModelParamType::MPT_TRANSFORM_VEC, evaluator )
{
}

// *******************************
//
void ParamTransformVec::Serialize       ( ISerializer& ser ) const
{
    ser.EnterChild( "param" );
    ser.SetAttribute( "name", GetName() );
    ser.SetAttribute( "type", "transform_vec" );

    auto timeline = TimelineManager::GetInstance()->GetTimelinePath( m_timeEvaluator );
    ser.SetAttribute( "timeline", timeline );

    for( auto t : m_transformModelVec )
        t.Serialize( ser );

    ser.ExitChild(); // param
}

// *******************************
//
void ParamTransformVec::SetCurveType        ( CurveType type ) 
{ 
    for( auto& trans : m_transformModelVec ) 
        trans.SetCurveType( type ); 
}

// *******************************
//
void ParamTransformVec::SetWrapPostMethod        ( WrapMethod method ) 
{ 
    for( auto& trans : m_transformModelVec ) 
        trans.SetWrapPostMethod( method ); 
}

// *******************************
//
void ParamTransformVec::SetWrapPreMethod        ( WrapMethod method ) 
{ 
    for( auto& trans : m_transformModelVec ) 
        trans.SetWrapPreMethod( method ); 
}


// *******************************
// FIXME: reimplement with SQT paramter model
CurveType           ParamTransformVec::GetCurveType    ()
{
    assert( false );
    return CurveType::CT_TOTAL;
}

// *******************************
// FIXME: reimplement with SQT paramter model
WrapMethod  ParamTransformVec::GetWrapPostMethod ()
{
    assert( false );
    return WrapMethod( -1 );
}

// *******************************
// FIXME: reimplement with SQT paramter model
WrapMethod  ParamTransformVec::GetWrapPreMethod ()
{
    assert( false );
    return WrapMethod( -1 );
}

// *******************************
// FIXME: reimplement with SQT paramter model
int         ParamTransformVec::GetNumKeys      ()
{
    assert( false );
    return -1;
}

// *******************************
//
void    ParamTransformVec::AppendTransform          ( const TransformF & transform )
{
    m_transformModelVec.push_back( transform );
}

// *******************************
//
void                ParamTransformVec::InsertTransform     ( unsigned int transformNum, const TransformF & transform )
{
    if( transformNum < m_transformModelVec.size() )
        m_transformModelVec[ transformNum ] = transform;
    else if( transformNum == m_transformModelVec.size() )
        AppendTransform( transform );
    else
        assert( false );
}

// *******************************
//
VoidPtr         ParamTransformVec::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

} //model
} //bv
