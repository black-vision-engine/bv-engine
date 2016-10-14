#include "stdafx.h"

#include "TextEffects.h"

#include "Engine/Models/BVProjectEditor.h"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Engine/Models/Plugins/PluginUtils.h"
#include "Engine/Events/EventHandlerHelpers.h"


namespace bv { namespace model {

namespace {

const std::string SHADOW_TEXT_NODE_EFFECT_NAME = "text_shadow_node_effect";

}

const std::string   TextEffects::m_type = "text_effects";


// ***********************
//
TextEffects::TextEffects     ( BasicNodePtr & node )
    : m_node( node )
    , m_blurSize( 5 )
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

    if( !ShadowAssetIsValid() )
    {
        ReloadShadowNodeAsset();
    }
}

// ***********************
//
bool                    TextEffects::ShadowAssetIsValid  () const
{
    auto textFontAssetDesc          = GetFontAssetDesc();
    auto textShadowFontAssetDesc    = GetShadowFontAssetDesc();

    if( textFontAssetDesc != nullptr && textShadowFontAssetDesc != nullptr )
    {
        return  textFontAssetDesc->GetFontFileName() == textShadowFontAssetDesc->GetFontFileName() &&
            textFontAssetDesc->GetFontSize() == textShadowFontAssetDesc->GetFontSize() &&
            textFontAssetDesc->GetGenerateMipmaps() == textShadowFontAssetDesc->GetGenerateMipmaps() &&
            textFontAssetDesc->GetOutlineSize() == textShadowFontAssetDesc->GetOutlineSize() &&
            textShadowFontAssetDesc->GetBlurSize() == m_blurSize;
    }
    else
    {
        return true;
    }
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
    std::string action = eventSer.GetAttribute( "Action" );
    
    if( action == "Initialize" ) 
    {
        auto context = static_cast< BVDeserializeContext * >( eventSer.GetDeserializeContext() );
        auto scene = editor->GetModelScene( context->GetSceneName() );

        editor->AddNodeCopy( scene, m_node, scene, m_node );

        if( ReloadShadowNodeAsset() )
        {
            response.SetAttribute( COMMAND_SUCCESS_STRING, "Text Effect initialized" );
            return true;
        }
        else
        {
            response.SetAttribute( ERROR_INFO_STRING, "Cannot initialize text effects logic" );
        }
    }
    else if( action == "SetGlowSize" )
    {
        std::string valueStr = eventSer.GetAttribute( "NewSizeValue" );
        auto value = SerializationHelper::String2T< UInt32 >( valueStr );

        m_blurSize = value;
    }
    else 
    {
        response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only 'Initialize' command." );
    }

    return false;
}

// ***********************
//
bool                    TextEffects::ReloadShadowNodeAsset       () const
{
    auto shadowTextPlugin = GetShadowTextPlugin();

    auto desc = GetFontAssetDesc();

    if( shadowTextPlugin != nullptr && desc != nullptr )
    {
        LoadFont( shadowTextPlugin, desc->GetFontFileName(), desc->GetFontSize(), m_blurSize, desc->GetOutlineSize(), desc->GetGenerateMipmaps() );

        return true;
    }
    else
    {
        return false;
    }
}

// ***********************
//
void                    TextEffects::Serialize          ( ISerializer & ser ) const
{
    { ser; }
}

// ***********************
//
TextEffectsPtr          TextEffects::Create             ( BasicNodePtr & node )
{
    return TextEffectsPtr( new TextEffects( node ) );
}

// ***********************
//
TextEffectsPtr          TextEffects::Create             ( const IDeserializer & deser, BasicNodePtr & node )
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

// ***********************
//
FontAssetDescConstPtr   TextEffects::GetFontAssetDesc            () const
{
    auto lassets = GetTextPlugin()->GetLAssets();

    if( lassets.size() > 0 )
    {
        return std::dynamic_pointer_cast< const FontAssetDesc >( lassets[ 0 ].assetDesc );
    }
    else
    {
        return nullptr;
    }
}

// ***********************
//
FontAssetDescConstPtr   TextEffects::GetShadowFontAssetDesc   () const
{
    auto lassets = GetShadowTextPlugin()->GetLAssets();

    if( lassets.size() > 0 )
    {
        return std::dynamic_pointer_cast< const FontAssetDesc >( lassets[ 0 ].assetDesc );
    }
    else
    {
        return nullptr;
    }
}

} // model
} // bv