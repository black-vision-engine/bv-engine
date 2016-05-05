#pragma once

#include "Assets/Asset.h"

#include "Mathematics/glm_inc.h"
#include "CoreDEF.h"


namespace bv
{

class MeshAsset;
DEFINE_PTR_TYPE( MeshAsset )
DEFINE_CONST_PTR_TYPE( MeshAsset )


class MeshAsset : public Asset, public std::enable_shared_from_this< MeshAsset >
{

public:

    struct MeshGeometry
    {
        std::vector< glm::vec3 >    positions;
        std::vector< glm::vec3 >    normals;
        std::vector< glm::vec2 >    uvs;
    };

    struct MeshTexture
    {
        std::string                 diffuseTexturePath;
    };
    
    struct MeshMaterial
    {
        glm::vec4                   diffuse;
        glm::vec4                   ambient;
        glm::vec4                   emissive;
        glm::vec4                   specular;
        Float32                     shininess;
    };

    DEFINE_PTR_TYPE( MeshGeometry )
    DEFINE_CONST_PTR_TYPE( MeshGeometry )
    DEFINE_PTR_TYPE( MeshTexture )
    DEFINE_CONST_PTR_TYPE( MeshTexture )
    DEFINE_PTR_TYPE( MeshMaterial )
    DEFINE_CONST_PTR_TYPE( MeshMaterial )

private:

	static const std::string	        uid;
    std::string			                m_key;

    MeshGeometryPtr                     m_geometry;
    MeshTexturePtr                      m_texture;
    MeshMaterialPtr                     m_material;

    std::vector< MeshAssetConstPtr >    m_children;

protected:

	virtual VoidConstPtr		QueryThis		() const override;

public:

	static const std::string &	UID			    ();
	const std::string &			GetUID		    () const override;

	const std::string &			GetKey		    () const;

    void                        SetGeometry     ( MeshGeometryPtr geometry );
    void                        SetTexture      ( MeshTexturePtr texture );
    void                        SetMaterial     ( MeshMaterialPtr material );

    MeshGeometryConstPtr        GetGeometry     () const;
    MeshTextureConstPtr         GetTexture      () const;
    MeshMaterialConstPtr        GetMaterial     () const;


    void                        AddChild        ( MeshAssetConstPtr child );

    MeshAssetConstPtr           GetChild        ( const std::string & key ) const;
    MeshAssetConstPtr           GetChild        ( UInt32 idx ) const;
    UInt32                      NumChildren     () const;

	static MeshAssetPtr	        Create		    ( const std::string & key );
	
private:

	explicit					MeshAsset       ( const std::string & key );

};



} // bv