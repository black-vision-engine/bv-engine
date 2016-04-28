#pragma once

#include <fbxsdk.h>

#include "Assets/Mesh/MeshAsset.h"


namespace bv {

struct MeshUtils
{

	static MeshAssetConstPtr		    LoadMesh			( const std::string & path );
    static MeshAssetConstPtr            ConvertToMesh       ( FbxNode * node );

private:

    static void                         ProcessNormals      ( std::vector< glm::vec3 > & normals, FbxMesh * fbxMesh, int vertexIdx );
    static void                         ProcessUVs          ( std::vector< glm::vec2 > & uvs, FbxMesh * fbxMesh, int i, int j, int vertexIdx );

};

}  // bv