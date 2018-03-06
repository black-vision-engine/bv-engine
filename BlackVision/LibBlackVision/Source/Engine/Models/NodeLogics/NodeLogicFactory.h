#pragma once

#include "Engine/Models/Builder/NodeLogicHolder.h"
#include "Engine/Models/Interfaces/INodeLogic.h"

#include "Engine/Models/NodeLogics/NodeLogicDesc.h"

#include <map>



namespace bv {


class NodeLogicFactory : public INodeLogicFactory
{
private:

    std::map< std::string, model::NodeLogicDescPtr >        m_logics;
    std::map< std::string, model::GizmoLogicDescPtr >       m_gizmoLogics;

public:

    explicit NodeLogicFactory();
    virtual ~NodeLogicFactory() {}

    virtual model::INodeLogicPtr        CreateLogic         ( const IDeserializer & deser, model::BasicNodeWeakPtr logicParent )                                                                                override;
    virtual model::IGizmoLogicPtr       CreateGizmoLogic    ( const std::string & gizmoName, model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner, model::ITimeEvaluatorPtr timeEvaluator )    override;

    virtual std::string                 QueryGizmoName      ( const std::string & logicName, const std::string & function ) override;

public:

    model::NodeLogicDesc &              RegisterLogic       ( const std::string & logicType, model::LogicCreateFun creator );
    model::GizmoLogicDesc &             RegisterGizmo       ( const std::string & logicType, model::GizmoCreateFun creator );

public:

    void            RegisterDefaultLogics       ();
    void            RegisterRamainigLogics      ();
    void            RegisterAllLogics           ();

};

} //bv
