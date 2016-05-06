#include "stdafx.h"

#include "MeshUtils.h"

#include "ProjectManager.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"


namespace bv {

// ******************************
//
void                MeshUtils::ProcessNormals       ( std::vector< glm::vec3 > & normals, FbxMesh * fbxMesh, int vertexIdx )
{
    for( Int32 l = 0; l < fbxMesh->GetElementNormalCount(); ++l )
	{
		auto elNormal = fbxMesh->GetElementNormal( l );
        if( elNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex )
		{
            switch ( elNormal->GetReferenceMode() )
			{
                case FbxGeometryElement::eDirect:
                    {
                        glm::vec3 norm;
                        norm.x = ( Float32 )elNormal->GetDirectArray().GetAt( vertexIdx ).mData[ 0 ];
                        norm.y = ( Float32 )elNormal->GetDirectArray().GetAt( vertexIdx ).mData[ 1 ];
                        norm.z = ( Float32 )elNormal->GetDirectArray().GetAt( vertexIdx ).mData[ 2 ];

                        normals.push_back( norm );
                    }
                    break;
				case FbxGeometryElement::eIndexToDirect:
                    {
                        int id = elNormal->GetIndexArray().GetAt( vertexIdx );
                        glm::vec3 norm;
                        norm.x = ( Float32 )elNormal->GetDirectArray().GetAt( id ).mData[ 0 ];
                        norm.y = ( Float32 )elNormal->GetDirectArray().GetAt( id ).mData[ 1 ];
                        norm.z = ( Float32 )elNormal->GetDirectArray().GetAt( id ).mData[ 2 ];

                        normals.push_back( norm );
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
void                MeshUtils::ProcessUVs           ( std::vector< glm::vec2 > & uvs, FbxMesh * fbxMesh, int i, int j, int vertexIdx )
{
    for( Int32 l = 0; l < fbxMesh->GetElementUVCount(); ++l )
	{
		auto elUV = fbxMesh->GetElementUV( l );
        switch( elUV->GetMappingMode() )
		{
        case FbxGeometryElement::eByControlPoint:
            switch ( elUV->GetReferenceMode() )
			{
                case FbxGeometryElement::eDirect:
                    {
                        glm::vec2 uv;
                        uv.x = ( Float32 )elUV->GetDirectArray().GetAt( vertexIdx ).mData[ 0 ];
                        uv.y = ( Float32 )elUV->GetDirectArray().GetAt( vertexIdx ).mData[ 1 ];
                        
                        uvs.push_back( uv );
                    }
                    break;
				case FbxGeometryElement::eIndexToDirect:
                    {
                        int id = elUV->GetIndexArray().GetAt( vertexIdx );
                        glm::vec2 uv;
                        uv.x = ( Float32 )elUV->GetDirectArray().GetAt( id ).mData[ 0 ];
                        uv.y = ( Float32 )elUV->GetDirectArray().GetAt( id ).mData[ 1 ];
                        
                        uvs.push_back( uv );
                    }
                    break;
                default:
                    break;
            }
            break;

        case FbxGeometryElement::eByPolygonVertex:
            {
                auto id = fbxMesh->GetTextureUVIndex( i, j );
                switch ( elUV->GetReferenceMode() )
				{
                    case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
                        {
                            glm::vec2 uv;
                            uv.x = ( Float32 )elUV->GetDirectArray().GetAt( id ).mData[ 0 ];
                            uv.y = ( Float32 )elUV->GetDirectArray().GetAt( id ).mData[ 1 ];

                            uvs.push_back( uv );
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
        }
    }
}

// ******************************
//
void                MeshUtils::ProcessGeometry        ( MeshAssetPtr meshAsset, FbxMesh * fbxMesh )
{
    auto polyCount = fbxMesh->GetPolygonCount();

    if( polyCount > 0 )
    {
        auto geometry = std::make_shared< MeshAsset::MeshGeometry >();
        
        auto & positions = geometry->positions;
        auto & normals = geometry->normals;
        auto & uvs = geometry->uvs;

        UInt32 vertexIdx = 0;

        for( Int32 i = 0; i < polyCount; ++i )
        {
            auto polySize = fbxMesh->GetPolygonSize( i );
            for ( Int32 j = 0; j < polySize; ++j )
		    {
                auto idx = fbxMesh->GetPolygonVertex( i, j );

                glm::vec3 pos;
                pos.x = ( Float32 )fbxMesh->GetControlPointAt( idx ).mData[ 0 ];
                pos.y = ( Float32 )fbxMesh->GetControlPointAt( idx ).mData[ 1 ];
                pos.z = ( Float32 )fbxMesh->GetControlPointAt( idx ).mData[ 2 ];

                positions.push_back( pos );

                fbxMesh->GenerateNormals();
                ProcessNormals( normals, fbxMesh, vertexIdx );
                ProcessUVs( uvs, fbxMesh, i, j, vertexIdx );

                vertexIdx++;
            }
        }

        meshAsset->SetGeometry( geometry );
    }
}

// ******************************
//
void                MeshUtils::ProcessMaterial        ( MeshAssetPtr meshAsset, const FbxSurfaceMaterial * fbxMaterial )
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

        // get diffuse texture
        auto diffuseProp = fbxMaterial->FindProperty( FbxSurfaceMaterial::sDiffuse );
        if( diffuseProp.IsValid() )
        {
            auto textureCount = diffuseProp.GetSrcObjectCount< FbxFileTexture >();
            if( textureCount )
            {
                auto fbxTexture = diffuseProp.GetSrcObject< FbxFileTexture >();
                if( fbxTexture )
                {
                    auto texture = std::make_shared< MeshAsset::MeshTexture >();
                    texture->diffuseTexturePath = fbxTexture->GetFileName();
                    meshAsset->SetTexture( texture );
                }
            }
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
MeshAssetConstPtr   MeshUtils::ConvertToMesh        ( FbxNode * node )
{
    if( node )
    {
        auto mesh = MeshAsset::Create( node->GetName() );

        auto c = node->GetNodeAttributeCount(); c;
        for( auto idx = 0; idx < node->GetNodeAttributeCount(); ++idx )
        {
            auto att = node->GetNodeAttributeByIndex( idx );
            if( att )
            {
                auto type = att->GetAttributeType();
                if( FbxNodeAttribute::eMesh == type )
                {
                    auto fbxMesh = static_cast< FbxMesh * >( att );
                    
                    auto childMesh = MeshAsset::Create( att->GetName() );

                    ProcessGeometry( childMesh, fbxMesh );

                    auto fbxMaterial = node->GetMaterial( idx );
                    ProcessMaterial( childMesh, fbxMaterial );

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

}  // bv