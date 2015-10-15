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
//
void    ParamTransform::Serialize       ( ISerializer& doc ) const
{
    doc.EnterChild( "param" );
    doc.SetAttribute( "name", GetName() );
    doc.SetAttribute( "type", "transform" );
    doc.SetAttribute( "timeline", m_timeEvaluator->GetName() );

    m_transformModel.Serialize( doc );
    //for( unsigned int i = 0; i < m_transformModel.Size(); i++ )
    //{
    //    m_transformModel[ i ]->GetP0MotylaNoga().Serialize( doc );
    //    m_transformModel[ i ]->GetP1MotylaNoga().Serialize( doc );
    //    m_transformModel[ i ]->GetP2MotylaNoga().Serialize( doc );
    //}

    doc.ExitChild();
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
void ParamTransformVec::Serialize       ( ISerializer& doc ) const
{
    doc.EnterChild( "param" );
    doc.SetAttribute( "name", GetName() );
    doc.SetAttribute( "type", "transform_vec" );
    doc.SetAttribute( "timeline", m_timeEvaluator->GetName() );

    for( auto t : m_transformModelVec )
    {
        t.Serialize( doc );
        //for( int i = 0; i < t.Size(); i++ )
        //{
        //    t[ i ]->Ser
        //    //doc.SetName( "transform" );
        //    //doc.SetAttribute( "kind", Kind2String( t[ i ]->KindKurwaMac() ) );
        //    //    t[ i ]->GetP0MotylaNoga().Serialize( doc );
        //    //    t[ i ]->GetP1MotylaNoga().Serialize( doc );
        //    //    t[ i ]->GetP2MotylaNoga().Serialize( doc );
        //    //doc.ExitChild();
        //}
    }

    doc.ExitChild(); // param
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
