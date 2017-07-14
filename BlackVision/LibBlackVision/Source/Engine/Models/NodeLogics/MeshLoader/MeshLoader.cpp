#include "stdafx.h"

#include "MeshLoader.h"

#include "Engine/Events/EventHandlerHelpers.h"

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultMeshPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultMaterialPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultNormalMapPlugin.h"
#include "Engine/Models/Plugins/Simple/ShaderPlugins/DefaultParallaxMapPlugin.h"

#include "Engine/Editors/BVProjectEditor.h"

#include "System/Path.h"

namespace bv { namespace nodelogic {


const std::string           MeshLoader::m_type = "mesh_loader";

const std::string           MeshLoader::ACTION::LOAD                 = "Load";
const std::string           MeshLoader::ACTION::MESH_INFO            = "MeshInfo";
const std::string           MeshLoader::ACTION::GET_ASSET_PATH       = "GetAssetPath";
const std::string           MeshLoader::ACTION::SET_ASSET_PATH       = "SetAssetPath";
    
// *******************************
//
MeshLoaderPtr               MeshLoader::Create              ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEval, const std::string & assetPath )
{
    return std::make_shared< MeshLoader >( parent, timeEval, assetPath );
}

// *******************************
//
MeshLoader::MeshLoader      ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEval, const std::string & assetPath )
    : m_parentNode( parent )
    , m_timeEval( timeEval )
    , m_textureEnabled( true )
    , m_materialEnabled( true )
{
    m_assetDesc = MeshAssetDesc::Create( assetPath, "", true );
    m_asset = LoadTypedAsset< MeshAsset >( m_assetDesc );
}

// *******************************
//
MeshLoader::~MeshLoader     ()
{
}

// *******************************
//
void                MeshLoader::Update          ( TimeType )
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
        ser.SetAttribute( "textureEnabled", SerializationHelper::T2String( m_textureEnabled ) );
        ser.SetAttribute( "materialEnabled", SerializationHelper::T2String( m_materialEnabled ) );

        ser.SetAttribute( "assetPath", m_assetDesc->GetPath() );

        auto timeline = model::TimelineManager::GetInstance()->GetTimelinePath( m_timeEval );
        ser.SetAttribute( "timelinePath", timeline );
    }

    ser.ExitChild();
}

// ***********************
//
MeshLoaderPtr           MeshLoader::Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parent )
{
    auto assetPath = deser.GetAttribute( "assetPath" );
    auto timelinePath = deser.GetAttribute( "timelinePath" );

    if( !timelinePath.empty() )
    {
        auto deserContext = static_cast< BVDeserializeContext * >( deser.GetDeserializeContext() );
        if( deserContext )
        {
            model::ITimeEvaluatorPtr sceneTimeline = deserContext->GetSceneTimeline();
            if( !sceneTimeline )
            {
                sceneTimeline = model::TimelineManager::GetInstance()->GetRootTimeline();
            }
            auto timeEval = bv::model::TimelineHelper::GetTimeEvaluator( timelinePath, sceneTimeline );
        
            return MeshLoader::Create( parent, timeEval, assetPath );
        }
    }
    return nullptr;
}

// ***********************
//
bool                    MeshLoader::HandleEvent     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
{
    std::string action = eventSer.GetAttribute( "Action" );

    if( action == ACTION::LOAD ) 
    {
        m_textureEnabled = SerializationHelper::String2T( eventSer.GetAttribute( "textureEnabled" ), false );
        m_materialEnabled = SerializationHelper::String2T( eventSer.GetAttribute( "materialEnabled" ), false );
        
        Load( eventSer, editor );
    }
    else if( action == ACTION::GET_ASSET_PATH )
    {
        response.SetAttribute( "assetPath", m_assetDesc->GetPath() );
        return true;
    }
    else if( action == ACTION::SET_ASSET_PATH )
    {
        m_assetDesc = MeshAssetDesc::Create( eventSer.GetAttribute( "AssetPath" ), "", true );
        if( m_assetDesc )
        {
            m_asset = LoadTypedAsset< MeshAsset >( m_assetDesc );
            return ( m_asset != nullptr );
        }
        return false;
    }
    else if( action == ACTION::MESH_INFO )
    {
        return MeshInfo( response );
    }
    else 
    {
        response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only 'Replicate' command." );
    }

    return false;
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
void                        MeshLoader::Load                  ( IDeserializer & eventSer, BVProjectEditor * editor )
{
    auto context = static_cast< BVDeserializeContext * >( eventSer.GetDeserializeContext() );
    auto scene = editor->GetModelScene( context->GetSceneName() );

    Load( scene, editor );
}

// ***********************
//
void                        MeshLoader::Load                  ( model::SceneModelPtr scene, BVProjectEditor * editor )
{
    if( auto parentNode = m_parentNode.lock() )
    {
        // add nodes directly to parentNode, because 'RootNode' from mesh asset is always empty
        auto rootNode = Load( m_asset, m_timeEval );
        for( UInt32 i = 0; i < rootNode->GetNumChildren(); ++i )
        {
            editor->AddChildNode( scene, parentNode, rootNode->GetChild( i ) );
        }
    }
}

// ***********************
//
model::BasicNodePtr         MeshLoader::Load                  ( MeshAssetConstPtr asset, model::ITimeEvaluatorPtr timeEval )
{
    auto node = model::BasicNode::Create( asset->GetKey(), timeEval );
    node->AddPlugin( model::DefaultTransformPluginDesc::UID(), "transform", timeEval );
    
    auto transform = asset->GetTransform();
    if( transform )
    {
        auto transformPlugin = node->GetPlugin( "transform" );

        model::SetParameterTranslation( transformPlugin->GetParameter( model::DefaultTransformPlugin::PARAM::SIMPLE_TRANSFORM ), 0.f, transform->translation );
        model::SetParameterRotation( transformPlugin->GetParameter( model::DefaultTransformPlugin::PARAM::SIMPLE_TRANSFORM ), 0.f, transform->rotation );
        model::SetParameterScale( transformPlugin->GetParameter( model::DefaultTransformPlugin::PARAM::SIMPLE_TRANSFORM ), 0.f, transform->scale );
        model::SetParameterCenterMass( transformPlugin->GetParameter( model::DefaultTransformPlugin::PARAM::SIMPLE_TRANSFORM ), 0.0f, transform->center );
    }

    auto geometry = asset->GetGeometry();
    if( geometry )
    {
        node->AddPlugin( model::DefaultMeshPluginDesc::UID(), "mesh", timeEval );
        auto meshPlugin = node->GetPlugin( "mesh" );

        auto meshAssetDesc = MeshAssetDesc::Create( m_assetDesc->GetPath(), asset->GetKey(), false );
        meshPlugin->LoadResource( meshAssetDesc );

        auto material = asset->GetMaterial();
        if( m_materialEnabled && material )
        {
            node->AddPlugin( model::DefaultMaterialPluginDesc::UID(), "material", timeEval );
            auto materialPlugin = node->GetPlugin( "material" );
            
            model::SetParameter( materialPlugin->GetParameter( model::DefaultMaterialPlugin::PARAM::DIFFUSE ), 0.f, material->diffuse );
            model::SetParameter( materialPlugin->GetParameter( model::DefaultMaterialPlugin::PARAM::AMBIENT ), 0.f, material->ambient );
            model::SetParameter( materialPlugin->GetParameter( model::DefaultMaterialPlugin::PARAM::SPECULAR ), 0.f, material->specular );
            model::SetParameter( materialPlugin->GetParameter( model::DefaultMaterialPlugin::PARAM::EMISSION ), 0.f, material->emissive );
            model::SetParameter( materialPlugin->GetParameter( model::DefaultMaterialPlugin::PARAM::SHININESS ), 0.f, material->shininess );
        }

        auto texture = asset->GetTexture();
        if( m_textureEnabled && texture )
        {
            LoadTexture( texture->diffuseTexturePath, model::DefaultTexturePluginDesc::UID(), "texture", node, timeEval );
            LoadTexture( texture->normalMapTexturePath, model::DefaultNormalMapPluginDesc::UID(), "normal map", node, timeEval );
            LoadTexture( texture->heightMapTexturePath, model::DefaultParallaxMapPluginDesc::UID(), "parallax map", node, timeEval );
        }
    }

    for( UInt32 i = 0; i < asset->NumChildren(); ++i )
    {
        auto childAsset = asset->GetChild( i );
        auto childNode = Load( childAsset, timeEval );
        node->AddChildToModelOnly( childNode );
    }

    return node;
}

// ***********************
//
bool                        MeshLoader::MeshInfo                  ( ISerializer & response )
{
    if( m_asset )
    {
        std::vector< MeshAssetConstPtr > meshes;
        GetMeshes( m_asset, meshes );

        response.SetAttribute( "meshNum", SerializationHelper::T2String( meshes.size() ) );

        response.EnterArray( "meshes" );
        
        for( auto & mesh : meshes )
        {
            response.EnterChild( "mesh" );

            response.SetAttribute( "name", mesh->GetKey() );

            auto material = mesh->GetMaterial();
            if( material )
            {
                response.SetAttribute( "material", "true" );
            }

            auto texture = mesh->GetTexture();
            if( texture )
            {
                if( !texture->diffuseTexturePath.empty() )
                {
                    response.SetAttribute( "texture", texture->diffuseTexturePath );
                }

                if( !texture->normalMapTexturePath.empty() )
                {
                    response.SetAttribute( "normal map", texture->normalMapTexturePath );
                }

                if( !texture->heightMapTexturePath.empty() )
                {
                    response.SetAttribute( "height map", texture->heightMapTexturePath );
                }
            }

            response.ExitChild(); //mesh
        }

        response.ExitChild(); //meshes

        return true;
    }
    return false;
}

// ***********************
//
MeshAssetConstPtr           MeshLoader::GetMeshAsset                () const
{
    return m_asset;
}

// ***********************
//
void                        MeshLoader::EnableTextures              ( bool enabled )
{
    m_textureEnabled = enabled;
}

// ***********************
//
void                        MeshLoader::EnableMaterials              ( bool enabled )
{
    m_materialEnabled = enabled;
}

// ***********************
//
void                        MeshLoader::GetMeshes                  ( MeshAssetConstPtr asset, std::vector< MeshAssetConstPtr > & meshes ) const
{
    if( asset->GetGeometry() )
    {
        meshes.push_back( asset );
    }

    for( UInt32 i = 0; i < asset->NumChildren(); ++i )
    {
        auto childAsset = asset->GetChild( i );
        GetMeshes( childAsset, meshes );
    }
}

// ***********************
//
void                        MeshLoader::LoadTexture                 ( const std::string & txPath, const std::string & pluginUID, const std::string & pluginName, model::BasicNodePtr node, model::ITimeEvaluatorPtr timeEval )
{
    if( !txPath.empty() )
    {
        std::string texturePath = TranslatePath( txPath );

        node->AddPlugin( pluginUID, pluginName, timeEval );
        auto txPlugin = node->GetPlugin( pluginName );
        auto texDesc = TextureAssetDesc::Create( texturePath, true );
        if( texDesc )
        {
            txPlugin->LoadResource( texDesc );
        }
    }
}

// ***********************
//
std::string                 MeshLoader::TranslatePath               ( const std::string & txPath )
{
    auto assetDir = File::GetDirName( m_assetDesc->GetPath() );
    auto textureFile = File::GetFileName( txPath, true );

    auto assetPathVec = Path( assetDir ).Split();
    if( assetPathVec.size() > 0 && assetPathVec[ 0 ] == "meshes" )
    {
        assetPathVec[ 0 ] = "textures";
        
        Path texturePath;
        for( auto & elem : assetPathVec )
        {
            texturePath = texturePath / elem;
        }

        texturePath = texturePath / textureFile;
        return texturePath.Str();
    }
    else
    {
        return "";
    }
}

} //nodelogic
} //bv