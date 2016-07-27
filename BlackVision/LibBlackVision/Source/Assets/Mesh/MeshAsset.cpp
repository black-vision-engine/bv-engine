#include "stdafx.h"

#include "MeshAsset.h"


namespace bv
{

const std::string MeshAsset::uid = "MESH_ASSET";

// *******************************
// 
MeshAssetPtr	    MeshAsset::Create           ( const std::string & key )
{
    struct make_shared_enabler_MeshAssetDesc : public MeshAsset
    {
        make_shared_enabler_MeshAssetDesc( const std::string & key )
            : MeshAsset( key )
        {
        }
    };

    return std::make_shared< make_shared_enabler_MeshAssetDesc >( key );
}

// *******************************
// 
MeshAsset::MeshAsset                                ( const std::string & key )
    : m_key( key )
    , m_transform( nullptr )
    , m_geometry( nullptr )
    , m_texture( nullptr )
    , m_material( nullptr )
{
}

// ******************************
//
const std::string &	    MeshAsset::GetKey           () const
{
    return m_key;
}

// *******************************
// 
const std::string &	    MeshAsset::GetUID           () const
{
    return MeshAsset::uid;
}

// *******************************
// 
const std::string &	    MeshAsset::UID              ()
{
    return MeshAsset::uid;
}

// *******************************
// 
void	                MeshAsset::SetTransform      ( MeshAsset::MeshTransformConstPtr transform )
{
    m_transform = transform;
}

// *******************************
// 
void	                MeshAsset::SetGeometry      ( MeshAsset::MeshGeometryConstPtr geometry )
{
    m_geometry = geometry;
}

// *******************************
// 
void	                MeshAsset::SetTexture      ( MeshAsset::MeshTextureConstPtr texture )
{
    m_texture = texture;
}

// *******************************
// 
void	                MeshAsset::SetMaterial      ( MeshAsset::MeshMaterialConstPtr material )
{
    m_material = material;
}

// *******************************
// 
MeshAsset::MeshTransformConstPtr MeshAsset::GetTransform     () const
{
    return m_transform;
}

// *******************************
// 
MeshAsset::MeshGeometryConstPtr	MeshAsset::GetGeometry      () const
{
    return m_geometry;
}

// *******************************
// 
MeshAsset::MeshTextureConstPtr	MeshAsset::GetTexture      () const
{
    return m_texture;
}

// *******************************
// 
MeshAsset::MeshMaterialConstPtr	MeshAsset::GetMaterial      () const
{
    return m_material;
}

// *******************************
// 
void	                MeshAsset::AddChild         ( MeshAssetConstPtr child )
{
    m_children.push_back( child );
}

// *******************************
// 
MeshAssetConstPtr	    MeshAsset::GetChild         ( const std::string & key ) const
{
    if( key.empty() )
    {
        return shared_from_this();
    }

    for( auto child : m_children )
    {
        if( child->GetKey() == key )
        {
            return child;
        }
        
        auto ret = child->GetChild( key );
        if( ret )
        {
            return ret;
        }
    }

    return nullptr;
}

// *******************************
// 
MeshAssetConstPtr	    MeshAsset::GetChild         ( UInt32 idx ) const
{
    assert( idx < m_children.size() );
    return m_children[ idx ];
}

// *******************************
// 
UInt32	    MeshAsset::NumChildren      () const
{
    return ( UInt32 )m_children.size();
}

// *******************************
// 
VoidConstPtr            MeshAsset::QueryThis        () const
{
    return shared_from_this();
}

} // bv