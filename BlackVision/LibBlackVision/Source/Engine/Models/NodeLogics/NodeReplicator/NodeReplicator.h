#pragma once

#include "Engine/Models/Interfaces/INodeLogic.h"
#include "IReplicationModifier.h"
#include "Engine/Models/BasicNode.h"


namespace bv { namespace nodelogic {

class NodeReplicator;

DEFINE_PTR_TYPE( NodeReplicator )
DEFINE_CONST_PTR_TYPE( NodeReplicator )




/**@brief Replicates node and it's parameters. Allows to add value and time offset to chosen params.
@ingroup LogicsList*/
class NodeReplicator : public model::INodeLogic, public std::enable_shared_from_this< NodeReplicator >
{
private:

    static const std::string        m_type;

private:

    explicit                        NodeReplicator  ( model::BasicNodeWeakPtr node, SizeType repNum, const IReplicationModifierPtr & modifier = nullptr );

    model::BasicNodeWeakPtr         m_node;
    IReplicationModifierPtr         m_repModifier;
    SizeType                        m_repNum;
    bool                            m_initialized;


public:

    virtual void					Initialize		()				override;
    virtual void					Deinitialize	()				override;

    virtual void                PreNodeUpdate       ( TimeType )    override    {};
    virtual void                Update              ( TimeType )    override    {};
    virtual void                PostChildrenUpdate  ( TimeType )    override    {};


    static NodeReplicatorPtr        Create          ( model::BasicNodeWeakPtr node, SizeType repNum, const IReplicationModifierPtr & modifier );

    virtual const std::string &     GetType         () const override;
    static const std::string &      Type            ();

    virtual model::IParameterPtr                           GetParameter        ( const std::string & name ) const override;
    virtual const std::vector< model::IParameterPtr > &    GetParameters       () const override;

    virtual void                    Serialize       ( ISerializer & ser ) const override;
    static NodeReplicatorPtr        Create          ( const IDeserializer & deser, model::BasicNodeWeakPtr parentNode );

    virtual bool                    HandleEvent     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor ) override;


private:

    bool                            AddParamShift   ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );
    bool                            RemoveParamShift( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );
    bool                            ClearShifts     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor );
};

} // nodelogic
} // bv