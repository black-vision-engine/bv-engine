#include "stdafx.h"
#include "NodeLogicDesc.h"



namespace bv {
namespace model
{


// ========================================================================= //
// NodeLogicDesc
// ========================================================================= //


// ***********************
//
NodeLogicDesc::NodeLogicDesc( LogicCreateFun fun )
    : m_creatorFun( fun )
{}

// ***********************
//
NodeLogicDesc &           NodeLogicDesc::RegisterFunctionality      ( const std::string & function, const std::string & logicName )
{
    m_gizmoMap.RegisterFunctionality( function, logicName );
    return *this;
}

// ***********************
//
std::vector< std::string >      NodeLogicDesc::ListGizmoFunctionalities   () const
{
    return m_gizmoMap.ListFunctionalities();
}

// ***********************
//
model::INodeLogicPtr            NodeLogicDesc::CreateLogic                ( const IDeserializer & deser, model::BasicNodeWeakPtr logicParent ) const
{
    return m_creatorFun( deser, logicParent );
}

// ========================================================================= //
// GizmoLogicDesc
// ========================================================================= //

// ***********************
//
GizmoLogicDesc::GizmoLogicDesc( GizmoCreateFun fun )
    : m_creatorFun( fun )
{}

// ***********************
//
model::IGizmoLogicPtr           GizmoLogicDesc::CreateLogic                     ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator ) const
{
    return m_creatorFun( gizmoRoot, gizmoOwner, timeEvaluator );
}

}
}	// bv

