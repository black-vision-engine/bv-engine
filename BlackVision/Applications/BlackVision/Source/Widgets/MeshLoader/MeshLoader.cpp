#include "MeshLoader.h"

#include "Engine/Types/Values/ValuesFactory.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include <string>
#include <iostream>
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Assets/Assets.h"


namespace bv { namespace nodelogic {


const std::string           MeshLoader::m_type = "mesh_loader";

	
// *******************************
//
MeshLoaderPtr	            MeshLoader::Create				( model::BasicNodePtr parent, const std::string & assetPath )
{
	return std::make_shared< MeshLoader >( parent, assetPath );
}

// ****************************
//
bv::model::IParameterPtr    MeshLoader::GetValueParam       ()
{
    return m_param;
}

// *******************************
//
MeshLoader::MeshLoader( model::BasicNodePtr parent, const std::string & assetPath )
    : m_parentNode( parent )
    , m_textureEnabled( false )
    , m_materialEnabled( false )
{
    m_assetDesc = MeshAssetDesc::Create( assetPath, "", true );
    m_asset = LoadTypedAsset< MeshAsset >( m_assetDesc );
}

// *******************************
//
MeshLoader::~MeshLoader()
{
}

// *******************************
//
void		MeshLoader::Update				( TimeType )
{
}

// ***********************
//
void                MeshLoader::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );

    ser.SetAttribute( "type", m_type );

    if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
    {
        m_param->Serialize( ser );
        ser.SetAttribute( "assetPath", m_assetDesc->GetPath() );
    }

    ser.ExitChild();
}

// ***********************
//
MeshLoaderPtr           MeshLoader::Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parent )
{
    auto assetPath = deser.GetAttribute( "assetPath" );

    return MeshLoader::Create( parent, assetPath );
}

// ***********************
//
bool                    MeshLoader::HandleEvent     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * /*editor*/ )
{
    std::string action = eventSer.GetAttribute( "Action" );

    if( action == "Load" ) 
    {
        Load();
    }
    else if( action == "GetAssetPath" )
    {
        response.SetAttribute( "assetPath", m_assetDesc->GetPath() );
        return true;
    }
    else if( action == "SetAssetPath" )
    {
        m_assetDesc = MeshAssetDesc::Create( eventSer.GetAttribute( "assetPath" ), "", true );
        m_asset = LoadTypedAsset< MeshAsset >( m_assetDesc );
        
        //send info event

        return true;
    }
    else if( action == "GetTextureEnabled" )
    {
        response.SetAttribute( "textureEnabled", SerializationHelper::T2String( m_textureEnabled ) );
        return true;
    }
    else if( action == "SetTextureEnabled" )
    {
        m_textureEnabled = SerializationHelper::String2T( eventSer.GetAttribute( "textureEnabled" ), false );
        return true;
    }
    else if( action == "GetMaterialEnabled" )
    {
        response.SetAttribute( "materialEnabled", SerializationHelper::T2String( m_materialEnabled ) );
        return true;
    }
    else if( action == "SetMaterialEnabled" )
    {
        m_materialEnabled = SerializationHelper::String2T( eventSer.GetAttribute( "materialEnabled" ), false );
        return true;
    }
    else 
    {
        response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only 'Replicate' command." );
    }

    return false;
}

// ***********************
//
model::IParameterPtr                     MeshLoader::GetParameter        ( const std::string & ) const
{
    return nullptr;
}

// ***********************
//
const std::vector< model::IParameterPtr > & MeshLoader::GetParameters    () const
{
    static std::vector< model::IParameterPtr > ret;

    return ret;
}

// ***********************
//
const std::string &         MeshLoader::Type                  ()
{
    return m_type;
}

// ***********************
//
const std::string &         MeshLoader::GetType               () const
{
    return Type();
}

// ***********************
//
void                        MeshLoader::Load                  ()
{
    auto timeEval = m_parentNode->GetTimelines( false )[ 0 ];
    Load( m_asset, m_parentNode, timeEval );
}

// ***********************
//
void                        MeshLoader::Load                  ( MeshAssetConstPtr asset, model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEval )
{
    auto node = model::BasicNode::Create( "", timeEval );

    auto geometry = asset->GetGeometry();
    if( geometry )
    {
        node->AddPlugin( "DEFAULT_TRANSFORM", "transform", timeEval );
        node->AddPlugin( "DEFAULT_MESH", "mesh", timeEval );
        auto meshPlugin = node->GetPlugin( "mesh" );

        auto meshAssetDesc = MeshAssetDesc::Create( m_assetDesc->GetPath(), asset->GetKey(), false );
        meshPlugin->LoadResource( meshAssetDesc );

        auto material = asset->GetMaterial();
        if( material )
        {
            node->AddPlugin( "DEFAULT_MATERIAL", "material", timeEval );
            //FIXME
        }

        auto texture = asset->GetTexture();
        if( texture )
        {
            node->AddPlugin( "DEFAULT_TEXTURE", "texture", timeEval );
            auto txPlugin = node->GetPlugin( "texture" );
            auto texDesc = TextureAssetDesc::Create( texture->diffuseTexturePath, true );
            txPlugin->LoadResource( texDesc );
        }
    }

    for( UInt32 i = 0; i < asset->NumChildren(); ++i )
    {
        auto childAsset = asset->GetChild( i );
        Load( childAsset, node, timeEval );
    }

    parent->AddChildToModelOnly( node );
}

} //nodelogic
} //bv