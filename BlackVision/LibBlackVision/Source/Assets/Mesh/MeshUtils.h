#pragma once

#define FBXSDK_SHARED
#include <fbxsdk.h>

#include "Assets/Assets.h"


namespace bv {

struct MeshUtils
{

    static MeshAssetConstPtr		    LoadMesh			    ( MeshAssetDescConstPtr desc );
    static MeshAssetConstPtr            ConvertToMesh           ( FbxNode * node );

    static ThumbnailConstPtr            LoadThumbnail           ( const MeshAssetDescConstPtr & desc );

private:

    static void                         ProcessTransform        ( MeshAssetPtr meshAsset, FbxNode * fbxNode );
    static void                         ProcessGeometry         ( MeshAssetPtr meshAsset, FbxMesh * fbxMesh );
    static void                         ProcessMaterial         ( MeshAssetPtr meshAsset, const FbxSurfaceMaterial * fbxMaterial );

    static void                         ProcessNormals          ( std::vector< glm::vec3 > & normals, FbxMesh * fbxMesh, Int32 vertexIdx, Int32 ctrlIdx );
    static void                         ProcessUVs              ( std::vector< glm::vec2 > & uvs, FbxMesh * fbxMesh, Int32 vertexIdx, Int32 ctrlIdx );
    static void                         ProcessTangents         ( std::vector< glm::vec4 > & tangents, FbxMesh * fbxMesh, Int32 vertexIdx );
    static void                         ProcessBinormals        ( std::vector< glm::vec3 > & binormals, FbxMesh * fbxMesh, Int32 vertexIdx );

    static glm::vec4                    ProcessMaterialProperty ( const FbxSurfaceMaterial * material, const char * propertyName, const char * factorPropertyName );
    static std::string                  ProcessTextureProperty  ( const FbxSurfaceMaterial * material, const char * propertyName );

    template< typename T >
    static T                            FbxToGlm                ( fbxsdk::FbxVector2 value );
    template< typename T >
    static T                            FbxToGlm                ( fbxsdk::FbxVector4 value );

};

// ******************************
//
template<> glm::vec2        MeshUtils::FbxToGlm< glm::vec2 >    ( fbxsdk::FbxVector2 value );
template<> glm::vec3        MeshUtils::FbxToGlm< glm::vec3 >    ( fbxsdk::FbxVector4 value );
template<> glm::vec4        MeshUtils::FbxToGlm< glm::vec4 >    ( fbxsdk::FbxVector4 value );


}  // bv