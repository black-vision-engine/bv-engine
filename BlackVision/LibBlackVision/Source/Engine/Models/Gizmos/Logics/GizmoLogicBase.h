#pragma once

#include "Engine/Models/Interfaces/IGizmoLogic.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv {
namespace model
{


class BasicNode;
DEFINE_PTR_TYPE( BasicNode );
DEFINE_WEAK_PTR_TYPE( BasicNode );




// ***********************
//
class GizmoLogicBase : public IGizmoLogic
{
protected:

    model::BasicNodeWeakPtr         m_gizmoOwner;
    model::BasicNodeWeakPtr         m_gizmoRoot;

    DefaultParamValModelPtr         m_paramValModel;

public:

    explicit        GizmoLogicBase  ( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner );
    virtual         ~GizmoLogicBase ();

    virtual IParameterPtr                           GetParameter        ( const std::string & name ) const override;
    virtual const std::vector< IParameterPtr > &    GetParameters       () const override;


    virtual void                Initialize      ()              override {}
    virtual void                Deinitialize    ()              override {}

    virtual void                PreNodeUpdate       ( TimeType )    override {}
    virtual void                Update              ( TimeType )    override;
    virtual void                PostChildrenUpdate  ( TimeType )    override {}

    virtual void                PreOwnerUpdate      ( TimeType ) override {}
    virtual void                PostOwnerUpdate     ( TimeType ) override {}

    virtual void                Serialize       ( ISerializer & ser ) const;
    virtual void                Deserialize     ( const IDeserializer & ser );

    virtual bool                HandleEvent     ( IDeserializer & /*eventDeser*/, ISerializer & /*response*/, BVProjectEditor * /*editor */ ) override;

protected:

    template< typename EnumType >
    std::shared_ptr< ParamEnum< EnumType > >        QueryTypedEnum  ( const std::string & name );

    bool                        ParameterChanged    ( const std::string & name ) const;
};


// ========================================================================= //
// Inline implementation
// ========================================================================= //


// ***********************
//
template< typename EnumType >
inline std::shared_ptr< ParamEnum< EnumType > >     GizmoLogicBase::QueryTypedEnum   ( const std::string & name )
{
    return QueryTypedParam< std::shared_ptr< ParamEnum< EnumType > > >( m_paramValModel->GetParameter( name ) );
}

}   // model
}   // bv
