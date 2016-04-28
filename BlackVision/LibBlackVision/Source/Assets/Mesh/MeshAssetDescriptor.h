#pragma once

#include "Assets/AssetDescriptor.h"
#include "Serialization/Json/JsonSerializeObject.h"
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
	
	explicit							MeshAssetDesc	( const std::string & path );

protected:

	virtual const std::string &			GetUID				() const override;

public:

    virtual void                        Serialize       ( ISerializer& sob ) const;
	static ISerializableConstPtr        Create          ( const IDeserializer& dob );

	virtual bool						IsCacheable			() const override;

	virtual VoidConstPtr				QueryThis			() const override;

	virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;

    virtual SizeType                    EstimateMemoryUsage () const override;

    static MeshAssetDescConstPtr		Create				( const std::string & path );

	static const std::string &			UID					();

};

// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string & GetAssetDescUID< MeshAsset >()
{
    return MeshAssetDesc::UID();
}



} // bv