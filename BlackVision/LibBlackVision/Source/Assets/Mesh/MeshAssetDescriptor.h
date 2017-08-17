#pragma once

#include "Assets/AssetDescriptor.h"
#include "Assets/Mesh/MeshAsset.h"

#include "CoreDEF.h"


namespace bv
{

class MeshAssetDesc;
DEFINE_CONST_PTR_TYPE( MeshAssetDesc )

class MeshAssetDesc : public AssetDesc,  public std::enable_shared_from_this< AssetDesc >
{
private:

    static const std::string			uid;

    std::string						    m_path;
    std::string                         m_groupName;
    bool                                m_recursive;
    
    mutable std::string                 m_key;

private:

    explicit							MeshAssetDesc	    ( const std::string & path, const std::string & groupName, bool recursive );

protected:

    virtual const std::string &			GetUID				() const override;

public:

    virtual void                        Serialize           ( ISerializer & ser ) const;
    static ISerializableConstPtr        Create              ( const IDeserializer & deser );

    virtual bool						IsCacheable			() const override;

    virtual VoidConstPtr				QueryThis			() const override;

    virtual std::string					GetKey				() const override;
    
    std::string					        GetPath				() const;
    std::string					        GetGroupName		() const;
    bool				                IsRecursive			() const;

    virtual SizeType                    EstimateMemoryUsage () const override;

    static MeshAssetDescConstPtr		Create				( const std::string & path, const std::string & groupName, bool recursive );

    static const std::string &			UID					();


private:

    std::string                         ComputeKey          () const;
};

// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string & GetAssetDescUID< MeshAsset >()
{
    return MeshAssetDesc::UID();
}



} // bv