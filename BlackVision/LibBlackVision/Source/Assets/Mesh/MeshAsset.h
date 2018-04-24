#pragma once

#include "Assets/Asset.h"

#include "Mathematics/glm_inc.h"
#include "CoreDEF.h"


namespace bv
{

class MeshAsset;
DEFINE_PTR_TYPE( MeshAsset )
DEFINE_CONST_PTR_TYPE( MeshAsset )




/**@brief Mesh asset.
@ingroup Assets*/
class MeshAsset : public Asset, public std::enable_shared_from_this< MeshAsset >
{

public:

    struct MeshTransform
    {
        glm::vec3                   translation;
        glm::vec3                   rotation;
        glm::vec3                   scale;
        glm::vec3                   center;

        glm::mat4                   transform;
    };

    struct MeshGeometry
    {
        std::vector< glm::vec3 >    positions;
        std::vector< glm::vec3 >    normals;
        std::vector< glm::vec2 >    uvs;
        std::vector< glm::vec4 >    tangents;
    };

    struct MeshTexture
    {
        std::string                 diffuseTexturePath;
        std::string                 heightMapTexturePath;
        std::string                 normalMapTexturePath;
    };
    
    struct MeshMaterial
    {
        glm::vec4                   diffuse;
        glm::vec4                   ambient;
        glm::vec4                   emissive;
        glm::vec4                   specular;
        Float32                     shininess;
    };

    DEFINE_PTR_TYPE( MeshTransform )
    DEFINE_CONST_PTR_TYPE( MeshTransform )

    DEFINE_PTR_TYPE( MeshGeometry )
    DEFINE_CONST_PTR_TYPE( MeshGeometry )

    DEFINE_PTR_TYPE( MeshTexture )
    DEFINE_CONST_PTR_TYPE( MeshTexture )

    DEFINE_PTR_TYPE( MeshMaterial )
    DEFINE_CONST_PTR_TYPE( MeshMaterial )

private:

    static const std::string	        uid;
    std::string			                m_key;

    MeshTransformConstPtr               m_transform;
    MeshGeometryConstPtr                m_geometry;
    MeshTextureConstPtr                 m_texture;
    MeshMaterialConstPtr                m_material;

    std::vector< MeshAssetConstPtr >    m_children;

protected:

    virtual VoidConstPtr		QueryThis		() const override;

public:

    static const std::string &	UID			    ();
    const std::string &			GetUID		    () const override;

    const std::string &			GetKey		    () const;

    void                        SetTransform    ( MeshTransformConstPtr transform );
    void                        SetGeometry     ( MeshGeometryConstPtr geometry );
    void                        SetTexture      ( MeshTextureConstPtr texture );
    void                        SetMaterial     ( MeshMaterialConstPtr material );

    MeshTransformConstPtr       GetTransform    () const;
    MeshGeometryConstPtr        GetGeometry     () const;
    MeshTextureConstPtr         GetTexture      () const;
    MeshMaterialConstPtr        GetMaterial     () const;


    void                        AddChild        ( MeshAssetConstPtr child );

    MeshAssetConstPtr           GetChild        ( const std::string & key ) const;
    MeshAssetConstPtr           GetChild        ( UInt32 idx ) const;
    UInt32                      NumChildren     () const;

    static MeshAssetPtr	        Create		    ( const std::string & key );
    
protected:

    explicit					MeshAsset       ( const std::string & key );

};



} // bv