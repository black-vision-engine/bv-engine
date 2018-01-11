#include "stdafx.h"

#include "MeshAssetDescriptor.h"
#include "Serialization/SerializationHelper.h"

#include "System/Path.h"

#include "ProjectManager.h"


namespace bv
{

const std::string   MeshAssetDesc::uid = "MESH_ASSET_DESC";


// ***********************
//
void                MeshAssetDesc::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "asset" );

    ser.SetAttribute( "type", UID() );

    ser.SetAttribute( "path", m_path );
    ser.SetAttribute( "groupName", m_groupName );
    ser.SetAttribute( "recursive", Convert::T2String( m_recursive ) );

    ser.ExitChild();
}

// ***********************
//
ISerializableConstPtr MeshAssetDesc::Create          ( const IDeserializer & deser )
{
    auto path = deser.GetAttribute( "path" );
    auto groupName = deser.GetAttribute( "groupName" );
    auto recursive = Convert::String2T< bool >( deser.GetAttribute( "recursive" ), true );

    return Create( path, groupName, recursive );
}

// ***********************
//
MeshAssetDescConstPtr MeshAssetDesc::Create         ( const std::string & path, const std::string & groupName, bool recursive )
{
    struct make_shared_enabler_MeshAssetDesc : public MeshAssetDesc
    {
        make_shared_enabler_MeshAssetDesc( const std::string & path, const std::string & groupName, bool recursive )
            : MeshAssetDesc( path, groupName, recursive )
        {
        }
    };

    return std::make_shared< make_shared_enabler_MeshAssetDesc >( path, groupName, recursive );
}

// ***********************
//
const std::string &	        MeshAssetDesc::GetUID() const
{
    return MeshAssetDesc::UID();
}

// ***********************
//
const std::string &         MeshAssetDesc::UID()
{
    return MeshAssetDesc::uid;
}

// ***********************
//
bool                        MeshAssetDesc::IsCacheable() const
{
    return true;
}

// ***********************
//
VoidConstPtr                MeshAssetDesc::QueryThis() const
{
    return shared_from_this();
}

// ***********************
//
MeshAssetDesc::MeshAssetDesc( const std::string & path, const std::string & groupName, bool recursive )
    : m_path( path )
    , m_groupName( groupName )
    , m_recursive( recursive )
{}


// ***********************
//
std::string	            MeshAssetDesc::GetKey	() const
{
    if( m_key.empty() )
        m_key = ComputeKey();

    return m_key;
}

// ***********************
//
std::string             MeshAssetDesc::ComputeKey() const
{
    return m_path + std::to_string( Path::GetTimestamp( ProjectManager::GetInstance()->ToAbsPath( m_path ) ) );
}

// ***********************
//
std::string				MeshAssetDesc::GetPath		() const
{
    return m_path;
}

// ***********************
//
std::string				MeshAssetDesc::GetGroupName	() const
{
    return m_groupName;
}

// ***********************
//
bool				    MeshAssetDesc::IsRecursive	() const
{
    return m_recursive;
}

// ***********************
//
SizeType                MeshAssetDesc::EstimateMemoryUsage () const
{
    //FIXME
    SizeType assetSize = 0;
    return assetSize;
}

} // bv

