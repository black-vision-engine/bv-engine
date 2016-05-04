#pragma once

#include <fbxsdk.h>

#include "Assets/Mesh/MeshAsset.h"
#include "Assets/Mesh/MeshAssetDescriptor.h"


namespace bv {

struct MeshUtils
{

	static MeshAssetConstPtr		    LoadMesh			( MeshAssetDescConstPtr desc );
    static MeshAssetConstPtr            ConvertToMesh       ( FbxNode * node );

private:

    static void                         ProcessNormals      ( std::vector< glm::vec3 > & normals, FbxMesh * fbxMesh, int vertexIdx );
    static void                         ProcessUVs          ( std::vector< glm::vec2 > & uvs, FbxMesh * fbxMesh, int i, int j, int vertexIdx );

};

}  // bv