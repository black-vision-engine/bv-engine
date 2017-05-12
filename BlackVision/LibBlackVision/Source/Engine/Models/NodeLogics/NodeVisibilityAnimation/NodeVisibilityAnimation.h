#pragma once

#include "CoreDEF.h"
#include "Engine/Models/NodeLogics/NodeLogicBase.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Events/Interfaces/IEvent.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

namespace bv
{

namespace model
{

class BasicNode;
DEFINE_PTR_TYPE( BasicNode );
DEFINE_WEAK_PTR_TYPE( BasicNode );

} // model




namespace nodelogic
{

class NodeVisibilityAnimation;

DEFINE_PTR_TYPE( NodeVisibilityAnimation );
DEFINE_CONST_PTR_TYPE( NodeVisibilityAnimation );



class NodeVisibilityAnimation : public model::NodeLogicBase, public std::enable_shared_from_this< NodeVisibilityAnimation >
{
private:
    static const std::string            m_type;

    struct ACTION
    {
        static const std::string        REGISTER_NODE_VISIBILITY_PARAM;
        static const std::string        UNREGISTER_NODE_VISIBILITY_PARAM;
    };

private:
    bv::model::BasicNodeWeakPtr			m_parentNode;

    bv::model::ITimeEvaluatorPtr        m_defaultTimeEvaluator;

    std::vector< std::pair< model::ValueParamState< bool >, model::IModelNodeWeakPtr > > m_paramNodes;

public:
    explicit    NodeVisibilityAnimation			( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator );
    ~NodeVisibilityAnimation		();

    virtual void                        Initialize		()				override;
    virtual void                        Update			( TimeType t )	override;
    virtual void                        Deinitialize	()				override;


    virtual const std::string &         GetType         () const override;
    static const std::string &          Type            ();

    virtual void                        Serialize       ( ISerializer & ser ) const override;
    static NodeVisibilityAnimationPtr	Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode );

    virtual bool                        HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor ) override;

private:
    void            NodeRemovedHandler      ( IEventPtr evt );
    void            NodeMovedHandler        ( IEventPtr evt );

    void            RemoveNodeParam         ( const model::IModelNodePtr & node );

    bool            RegisterNodeVisibilityParam( const std::string & sceneName, const std::string & nodePath, BVProjectEditor * editor );

};


}   // nodelogic
}	// bv
