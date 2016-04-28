#include "stdafx.h"

#include "MeshAssetDescriptor.h"
#include "Tools/Utils.h"
#include "ProjectManager.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVSerializeContext.h"


namespace bv
{

const std::string   MeshAssetDesc::uid = "MESH_ASSET_DESC";


// ***********************
//
void                MeshAssetDesc::Serialize       ( ISerializer & ser ) const
{
    { ser; }
}

// ***********************
//
ISerializableConstPtr MeshAssetDesc::Create          ( const IDeserializer & deser )
{
    { deser; }
    return nullptr;
}

// ***********************
//
MeshAssetDescConstPtr MeshAssetDesc::Create         ( const std::string & path )
{
    struct make_shared_enabler_MeshAssetDesc : public MeshAssetDesc
    {
        make_shared_enabler_MeshAssetDesc( const std::string & path )
            : MeshAssetDesc( path )
        {
        }
    };

    return std::make_shared< make_shared_enabler_MeshAssetDesc >( path );
}

// ***********************
//
const std::string &	MeshAssetDesc::GetUID() const
{
	return MeshAssetDesc::UID();
}

// ***********************
//
const std::string & MeshAssetDesc::UID()
{
	return MeshAssetDesc::uid;
}

// ***********************
//
bool MeshAssetDesc::IsCacheable() const
{
	return true;
}

// ***********************
//
VoidConstPtr MeshAssetDesc::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
MeshAssetDesc::MeshAssetDesc( const std::string & path )
    : m_path( path )
{
}

// ***********************
//
std::string				MeshAssetDesc::GetKey		() const
{
	return m_path;
}

// ***********************
//
std::string             MeshAssetDesc::GetProposedShortKey () const
{
    auto basename = AssetDesc::GetProposedShortKey();
    return basename.substr( 0, basename.find( '.' ) );
}

// ***********************
//
SizeType                MeshAssetDesc::EstimateMemoryUsage () const
{
    SizeType assetSize = 0;
    return assetSize;
}

} // bv

