#include "stdafx.h"

#include "MeshUtils.h"

#include "ProjectManager.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Assets/Thumbnail/Impl/MeshAssetThumbnail.h"

#include "DataTypes/Hash.h"

#include "Mathematics/glm_inc.h"


#include "Memory/MemoryLeaks.h"



namespace bv {

// ******************************
//
void                MeshUtils::ProcessNormals       ( std::vector< glm::vec3 > & normals, FbxMesh * fbxMesh, Int32 vertexIdx, Int32 ctrlIdx )
{
    for( Int32 l = 0; l < fbxMesh->GetElementNormalCount(); ++l )
    {
        auto elNormal = fbxMesh->GetElementNormal( l );
        switch ( elNormal->GetMappingMode() )
        {
            case FbxGeometryElement::eByPolygonVertex:
            {
                switch ( elNormal->GetReferenceMode() )
                {
                    case FbxGeometryElement::eDirect:
                    {
                        normals.push_back( FbxToGlm< glm::vec3 >( elNormal->GetDirectArray().GetAt( vertexIdx ) ) );
                    }
                    break;

                    case FbxGeometryElement::eIndexToDirect:
                    {
                        int id = elNormal->GetIndexArray().GetAt( vertexIdx );
                        normals.push_back( FbxToGlm< glm::vec3 >( elNormal->GetDirectArray().GetAt( id ) ) );
                    }
                    break;

                    default:
                        break;
                }
            }
            break;

            case FbxGeometryElement::eByControlPoint:
            {
                switch ( elNormal->GetReferenceMode() )
                {
                    case FbxGeometryElement::eDirect:
                    {
                        normals.push_back( FbxToGlm< glm::vec3 >( elNormal->GetDirectArray().GetAt( ctrlIdx ) ) );
                    }
                    break;

                    case FbxGeometryElement::eIndexToDirect:
                    {
                        int id = elNormal->GetIndexArray().GetAt( ctrlIdx );
                        normals.push_back( FbxToGlm< glm::vec3 >( elNormal->GetDirectArray().GetAt( id ) ) );
                    }
                    break;

                    default:
                        break;
                }
            }
            break;
             
            default:
                break;
        }
    }
}

// ******************************
//
void                MeshUtils::ProcessUVs           ( std::vector< glm::vec2 > & uvs, FbxMesh * fbxMesh, Int32 vertexIdx, Int32 ctrlIdx )
{
    auto countElements = fbxMesh->GetElementUVCount();
    for( Int32 l = 0; l < countElements; ++l )
    {
        auto elUV = fbxMesh->GetElementUV( l );
        switch( elUV->GetMappingMode() )
        {
            case FbxGeometryElement::eByControlPoint:
            {
                switch ( elUV->GetReferenceMode() )
                {
                    case FbxGeometryElement::eDirect:
                    {
                        uvs.push_back( FbxToGlm< glm::vec2 >( elUV->GetDirectArray().GetAt( ctrlIdx ) ) );
                    }
                    break;

                    case FbxGeometryElement::eIndexToDirect:
                    {
                        int id = elUV->GetIndexArray().GetAt( ctrlIdx );
                        uvs.push_back( FbxToGlm< glm::vec2 >( elUV->GetDirectArray().GetAt( id ) ) );
                    }
                    break;

                    default:
                        break;
                }
            }
            break;

            case FbxGeometryElement::eByPolygonVertex:
            {
                switch ( elUV->GetReferenceMode() )
                {
                    case FbxGeometryElement::eDirect:
                    {
                        uvs.push_back( FbxToGlm< glm::vec2 >( elUV->GetDirectArray().GetAt( vertexIdx ) ) );
                    }
                    case FbxGeometryElement::eIndexToDirect:
                    {
                        int id = elUV->GetIndexArray().GetAt( vertexIdx );
                        uvs.push_back( FbxToGlm< glm::vec2 >( elUV->GetDirectArray().GetAt( id ) ) );
                    }
                    break;
                    default:
                        break;
                }
            }
            break;

            default:
                break;
        }
    }

    if( countElements == 0 )
        uvs.push_back( glm::vec2( 0.0, 0.0 ) );
}

// ******************************
//
void                MeshUtils::ProcessTangents      ( std::vector< glm::vec4 > & tangents, FbxMesh * fbxMesh, Int32 vertexIdx )
{
    auto countElements = fbxMesh->GetElementTangentCount();
    for( Int32 l = 0; l < countElements; ++l )
    {
        auto elTangent = fbxMesh->GetElementTangent( l );

        if( elTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
        {
            switch ( elTangent->GetReferenceMode() )
            {
                case FbxGeometryElement::eDirect:
                {
                    tangents.push_back( FbxToGlm< glm::vec4 >( elTangent->GetDirectArray().GetAt( vertexIdx ) ) );
                }
                break;

                case FbxGeometryElement::eIndexToDirect:
                {
                    int id = elTangent->GetIndexArray().GetAt( vertexIdx );
                    tangents.push_back( FbxToGlm< glm::vec4 >( elTangent->GetDirectArray().GetAt( id ) ) );
                }
                break;

                default:
                    break;
            }
        }
    }

    if( countElements == 0 )
        tangents.push_back( glm::vec4( 1.0, 0.0, 0.0, 1.0 ) );
}

// ******************************
//
void                MeshUtils::ProcessBinormals      ( std::vector< glm::vec3 > & binormals, FbxMesh * fbxMesh, Int32 vertexIdx )
{
    for( Int32 l = 0; l < fbxMesh->GetElementBinormalCount(); ++l )
    {
        auto elBinormal = fbxMesh->GetElementBinormal( l );

        if( elBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
        {
            switch ( elBinormal->GetReferenceMode() )
            {
            case FbxGeometryElement::eDirect:
                {
                    binormals.push_back( FbxToGlm< glm::vec3 >( elBinormal->GetDirectArray().GetAt( vertexIdx ) ) );
                }
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = elBinormal->GetIndexArray().GetAt( vertexIdx );
                    binormals.push_back( FbxToGlm< glm::vec3 >( elBinormal->GetDirectArray().GetAt( id ) ) );
                }
                break;
            default:
                break;
            }
        }
    }
}

// ******************************
//
void                            MeshUtils::ProcessTransform       ( MeshAssetPtr meshAsset, FbxNode * fbxNode )
{
    auto transform = std::make_shared< MeshAsset::MeshTransform >();

    auto translation = fbxNode->LclTranslation.Get();
    auto rotation = fbxNode->LclRotation.Get();
    auto scale = fbxNode->LclScaling.Get();
    auto center = fbxNode->RotationPivot.Get();
    auto localTransform = fbxNode->EvaluateLocalTransform();

    //assert( fbxNode->PostRotation.Get() == fbxsdk::FbxDouble3( 0.0, 0.0, 0.0 ) );
    //assert( fbxNode->PreRotation.Get() == fbxsdk::FbxDouble3( 0.0, 0.0, 0.0 ) );
    //assert( fbxNode->ScalingOffset.Get() == fbxsdk::FbxDouble3( 0.0, 0.0, 0.0 ) );
    //assert( fbxNode->ScalingPivot.Get() == fbxNode->RotationPivot.Get() );

    transform->translation = glm::vec3( translation[ 0 ], translation[ 1 ], translation[ 2 ] );
    transform->rotation = glm::vec3( rotation[ 0 ], rotation[ 1 ], rotation[ 2 ] );
    transform->scale = glm::vec3( scale[ 0 ], scale[ 1 ], scale[ 2 ] );
    transform->center = glm::vec3( center[ 0 ], center[ 1 ], center[ 2 ] );
    transform->transform = glm::mat4(
        localTransform.mData[ 0 ][ 0 ], localTransform.mData[ 0 ][ 1 ], localTransform.mData[ 0 ][ 2 ], localTransform.mData[ 0 ][ 3 ],
        localTransform.mData[ 1 ][ 0 ], localTransform.mData[ 1 ][ 1 ], localTransform.mData[ 1 ][ 2 ], localTransform.mData[ 1 ][ 3 ],
        localTransform.mData[ 2 ][ 0 ], localTransform.mData[ 2 ][ 1 ], localTransform.mData[ 2 ][ 2 ], localTransform.mData[ 2 ][ 3 ], 
        localTransform.mData[ 3 ][ 0 ], localTransform.mData[ 3 ][ 1 ], localTransform.mData[ 3 ][ 2 ], localTransform.mData[ 3 ][ 3 ] );

    meshAsset->SetTransform( transform );
}

// ******************************
//
void                            MeshUtils::ProcessGeometry        ( MeshAssetPtr meshAsset, FbxMesh * fbxMesh )
{
    auto polyCount = fbxMesh->GetPolygonCount();

    if( polyCount > 0 )
    {
        auto geometry = std::make_shared< MeshAsset::MeshGeometry >();
        
        auto & positions = geometry->positions;
        auto & normals = geometry->normals;
        auto & uvs = geometry->uvs;
        auto & tangents = geometry->tangents;

        fbxMesh->GenerateNormals();
        fbxMesh->GenerateTangentsDataForAllUVSets();

        UInt32 vertexIdx = 0;

        for( Int32 i = 0; i < polyCount; ++i )
        {
            auto polySize = fbxMesh->GetPolygonSize( i );
            for ( Int32 j = 0; j < polySize; ++j )
            {
                auto ctrlIdx = fbxMesh->GetPolygonVertex( i, j );
                
                positions.push_back( FbxToGlm< glm::vec3 >( fbxMesh->GetControlPointAt( ctrlIdx ) ) );

                ProcessNormals( normals, fbxMesh, vertexIdx, ctrlIdx );
                ProcessUVs( uvs, fbxMesh, vertexIdx, ctrlIdx );
                ProcessTangents( tangents, fbxMesh, vertexIdx );

                vertexIdx++;
            }
        }

        meshAsset->SetGeometry( geometry );
    }
}

// ******************************
//
void                            MeshUtils::ProcessMaterial        ( MeshAssetPtr meshAsset, const FbxSurfaceMaterial * fbxMaterial )
{
    if( fbxMaterial )
    {
        auto material = std::make_shared< MeshAsset::MeshMaterial >();

        // get material
        material->diffuse = ProcessMaterialProperty( fbxMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor );
        material->ambient = ProcessMaterialProperty( fbxMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor );
        material->emissive = ProcessMaterialProperty( fbxMaterial, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor );
        material->specular = ProcessMaterialProperty( fbxMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor );
        
        auto shininessProp = fbxMaterial->FindProperty( FbxSurfaceMaterial::sShininess );
        if( shininessProp.IsValid() )
        {
            material->shininess = shininessProp.Get< Float32 >();
        }
        meshAsset->SetMaterial( material );


        auto texture = std::make_shared< MeshAsset::MeshTexture >();

        auto diffuseTex = ProcessTextureProperty( fbxMaterial, FbxSurfaceMaterial::sDiffuse );
        if( !diffuseTex.empty() )
        {
            texture->diffuseTexturePath = diffuseTex;
            meshAsset->SetTexture( texture );
        }

        auto heightMapTex = ProcessTextureProperty( fbxMaterial, FbxSurfaceMaterial::sBump );
        if( !heightMapTex.empty() )
        {
            texture->heightMapTexturePath = heightMapTex;
            meshAsset->SetTexture( texture );
        }

        auto normalMapTex = ProcessTextureProperty( fbxMaterial, FbxSurfaceMaterial::sNormalMap );
        if( !normalMapTex.empty() )
        {
            texture->normalMapTexturePath = normalMapTex;
            meshAsset->SetTexture( texture );
        }
    }
}

// ******************************
//
glm::vec4           MeshUtils::ProcessMaterialProperty  ( const FbxSurfaceMaterial * material, const char * propertyName, const char * factorPropertyName )
{
    glm::vec4 ret( 0.f );

    auto prop = material->FindProperty( propertyName );
    auto factorProp = material->FindProperty( factorPropertyName );

    if ( prop.IsValid() && factorProp.IsValid() )
    {
        auto val = prop.Get< FbxDouble3 >();
        auto valFactor = factorProp.Get< FbxDouble >();
        ret = glm::vec4( val[ 0 ], val[ 1 ], val[ 2 ], valFactor );
    }

    return ret;
}

// ******************************
//
std::string         MeshUtils::ProcessTextureProperty   ( const FbxSurfaceMaterial * material, const char * propertyName )
{
    auto prop = material->FindProperty( propertyName );
    if( prop.IsValid() )
    {
        auto textureCount = prop.GetSrcObjectCount< FbxFileTexture >();
        if( textureCount )
        {
            auto fbxTexture = prop.GetSrcObject< FbxFileTexture >();
            if( fbxTexture )
            {
                return fbxTexture->GetFileName();
            }
        }
    }

    return std::string();
}

// ******************************
//
MeshAssetConstPtr   MeshUtils::ConvertToMesh            ( FbxNode * node )
{
    if( node )
    {
        std::string meshName = node->GetNameOnly().Buffer();
        auto mesh = MeshAsset::Create( meshName );
        
        ProcessTransform( mesh, node );

        for( auto attIdx = 0; attIdx < node->GetNodeAttributeCount(); ++attIdx )
        {
            auto att = node->GetNodeAttributeByIndex( attIdx );
            if( att )
            {
                auto type = att->GetAttributeType();
                if( FbxNodeAttribute::eMesh == type )
                {
                    auto fbxMesh = static_cast< FbxMesh * >( att );
                    
                    std::string attName = att->GetNameOnly().Buffer();
                    if( attName.empty() )
                    {
                        attName = meshName + SerializationHelper::T2String( attIdx );
                    }

                    auto childMesh = MeshAsset::Create( attName );

                    ProcessGeometry( childMesh, fbxMesh );
                    
                    ProcessMaterial( childMesh, node->GetMaterial( attIdx ) );

                    mesh->AddChild( childMesh );
                }
            }
        }

        for( Int32 i = 0; i < node->GetChildCount(); ++i )
        {
            auto childMesh = ConvertToMesh( node->GetChild( i ) );
            mesh->AddChild( childMesh );
        }

        return mesh;
    }

    return nullptr;
}

// ******************************
//
MeshAssetConstPtr   MeshUtils::LoadMesh             ( MeshAssetDescConstPtr desc )
{
    auto path = ProjectManager::GetInstance()->ToAbsPath( desc->GetPath() ).Str();

    auto manager = FbxManager::Create();
    manager->SetIOSettings( FbxIOSettings::Create( manager, IOSROOT ) );

    auto importer = FbxImporter::Create( manager, "" );
    if( !importer->Initialize( path.c_str(), -1, manager->GetIOSettings() ) )
    {
        //FIXME: failed to load mesh
        return nullptr;
    }

    auto scene = FbxScene::Create( manager, "" );
    importer->Import( scene );
    importer->Destroy();

    FbxGeometryConverter converter( manager );
    converter.Triangulate( scene, true );
    converter.SplitMeshesPerMaterial( scene, true );

    auto mesh = ConvertToMesh( scene->GetRootNode() );

    manager->Destroy();

    return mesh;
}

// ******************************
//
ThumbnailConstPtr   MeshUtils::LoadThumbnail             ( const MeshAssetDescConstPtr & desc )
{
    auto texPath = desc->GetPath();

    auto absTexPath = ProjectManager::GetInstance()->ToAbsPath( texPath );

    auto thumbPath = AssetAccessor::GetThumbnailPath( absTexPath );

    if( Path::Exists( thumbPath ) )
    {
        JsonDeserializeObject deser;
        deser.LoadFile( thumbPath.Str() );

        return MeshAssetThumbnail::Create( deser );
    }

    return nullptr;
}

// ******************************
//
template<> 
glm::vec2           MeshUtils::FbxToGlm< glm::vec2 >    ( fbxsdk::FbxVector2 value )
{
    return glm::vec2( ( Float32 )value[ 0 ], ( Float32 )value[ 1 ] );
}

// ******************************
//
template<> 
glm::vec3           MeshUtils::FbxToGlm< glm::vec3 >    ( fbxsdk::FbxVector4 value )
{
    return glm::vec3( ( Float32 )value[ 0 ], ( Float32 )value[ 1 ], ( Float32 )value[ 2 ] );
}

// ******************************
//
template<> 
glm::vec4           MeshUtils::FbxToGlm< glm::vec4 >    ( fbxsdk::FbxVector4 value )
{
    return glm::vec4( ( Float32 )value[ 0 ], ( Float32 )value[ 1 ], ( Float32 )value[ 2 ], ( Float32 )value[ 3 ] );
}

}  // bv