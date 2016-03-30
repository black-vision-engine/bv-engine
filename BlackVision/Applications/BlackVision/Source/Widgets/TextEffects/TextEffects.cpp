#include "stdafx.h"

#include "TextEffects.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "EndUserAPI/EventHandlers/EventHandlerHelpers.h"
#include "Engine/Models/BVProjectEditor.h"

namespace bv { namespace model
{

namespace 
{

const std::string SHADOW_TEXT_NODE_EFFECT_NAME = "text_shadow_node_effect";

}

const std::string   TextEffects::m_type = "text_effects";

// ***********************
//
TextEffects::TextEffects     ( const BasicNodePtr & node )
    : m_node( node )
{}

// ***********************
//
TextEffects::~TextEffects    ()
{}

// ***********************
//
const std::string &      TextEffects::Type              ()
{
    return m_type;
}

// ***********************
//
void					TextEffects::Initialize		    ()
{

}

// ***********************
//
void					TextEffects::Update			    ( TimeType t )
{
    { t; }
}

// ***********************
//
void					TextEffects::Deinitialize	    ()
{}

// ***********************
//
const std::string &     TextEffects::GetType            () const
{
    return Type();
}

// ***********************
//
IParameterPtr                           TextEffects::GetParameter        ( const std::string & name ) const
{
    auto p = m_paramValModel->GetParameter( name );

    if( p != nullptr )
    {
        return p;
    }
    else
    {
        return GetShadowTextPlugin()->GetParameter( name );
    }
}

// ***********************
//
const std::vector< IParameterPtr > &    TextEffects::GetParameters       () const
{
    return m_paramValModel->GetParameters();
}

// ***********************
//
bool                    TextEffects::HandleEvent        ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
{
    { editor; }

    std::string action = eventSer.GetAttribute( "Action" );
    
    if( action == "Initialize" ) 
    {

        {
            response.SetAttribute( ERROR_INFO_STRING, "Node has no child. Cannot i" );
        }
    }
    else 
    {
        response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only 'Initialize' command." );
    }

    return false;
}

// ***********************
//
void                    TextEffects::Serialize          ( ISerializer & ser ) const
{
    { ser; }
}

// ***********************
//
TextEffectsPtr          TextEffects::Create             ( const BasicNodePtr & node )
{
    return TextEffectsPtr( new TextEffects( node ) );
}

// ***********************
//
TextEffectsPtr          TextEffects::Create             ( const IDeserializer & deser, const BasicNodePtr & node )
{
    { deser; node; }
    return nullptr;
}

// ***********************
//
IPluginPtr              TextEffects::GetTextPlugin      () const
{
    return m_node->GetPlugin( "text" );
}

// ***********************
//
IPluginPtr              TextEffects::GetShadowTextPlugin() const
{
    return m_node->GetChild( SHADOW_TEXT_NODE_EFFECT_NAME )->GetPlugin( "text" );
}


} // model
} // bv