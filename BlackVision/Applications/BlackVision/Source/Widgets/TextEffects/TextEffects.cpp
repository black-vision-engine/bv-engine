#include "stdafx.h"

#include "TextEffects.h"

namespace bv { namespace model
{

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
bool                    TextEffects::HandleEvent        ( IDeserializer & eventStr, ISerializer & response, BVProjectEditor * editor )
{
    { eventStr; response; editor; }
    return true;
}

// ***********************
//
void                    TextEffects::Serialize          ( ISerializer& ser ) const
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

} // model
} // bv