#pragma once

#include "Engine/Models/Interfaces/INodeLogic.h"
#include "Engine/Models/BasicNode.h"

namespace bv { namespace model
{

class TextEffects;

DEFINE_PTR_TYPE( TextEffects )
DEFINE_CONST_PTR_TYPE( TextEffects )

class TextEffects : public model::INodeLogic, public std::enable_shared_from_this< TextEffects >
{
private:
    static const std::string        m_type;

    BasicNodePtr                    m_node;

    explicit                        TextEffects     ( const BasicNodePtr & node );

public:
    static const std::string &      Type            ();


	virtual void					Initialize		() override;
	virtual void					Update			( TimeType t ) override;
	virtual void					Deinitialize	() override;

    virtual const std::string &     GetType         () const override;
    virtual bool                    HandleEvent     ( IDeserializer & eventStr, ISerializer & response, BVProjectEditor * editor ) override;

    virtual void                    Serialize       ( ISerializer& ser ) const override;

    static TextEffectsPtr           Create          ( const BasicNodePtr & node );

};

} // model
} // bv