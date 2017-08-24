#pragma once

#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Models/Interfaces/IGizmoLogic.h"
#include "Engine/Models/Gizmos/GizmoFunMapper.h"

#include "Engine/Models/BasicNode.h"

#include "Serialization/IDeserializer.h"



namespace bv {
namespace model
{


typedef model::INodeLogicPtr        ( *LogicCreateFun )( const IDeserializer & deser, model::BasicNodeWeakPtr logicParent );
typedef model::IGizmoLogicPtr       ( *GizmoCreateFun )( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator );


// ***********************
//
class NodeLogicDesc
{
private:

    GizmoFunMapper      m_gizmoMap;
    LogicCreateFun      m_creatorFun;

public:

    explicit                    NodeLogicDesc ( LogicCreateFun fun );


    NodeLogicDesc &             RegisterFunctionality       ( const std::string & function, const std::string & logicName );
    const GizmoFunMapper &      AccessGizmoMapper           () const;

    model::INodeLogicPtr        CreateLogic                 ( const IDeserializer & deser, model::BasicNodeWeakPtr logicParent ) const;
};

DEFINE_PTR_TYPE( NodeLogicDesc );


// ***********************
//
class GizmoLogicDesc
{
private:

    GizmoCreateFun      m_creatorFun;

public:

    explicit                    GizmoLogicDesc      ( GizmoCreateFun fun );

    model::IGizmoLogicPtr       CreateLogic         ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator ) const;
};

DEFINE_PTR_TYPE( GizmoLogicDesc );


}
}	// bv

