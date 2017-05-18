#pragma once

#include "Assets/AssetDescriptor.h"

#include "SVGAsset.h"

namespace bv {

class SVGAssetDescriptor;
DEFINE_PTR_TYPE( SVGAssetDescriptor );
DEFINE_CONST_PTR_TYPE( SVGAssetDescriptor );

class SVGAssetDescriptor : public AssetDesc,  public std::enable_shared_from_this< AssetDesc >
{
private:
    
    std::string                     m_path;
    mutable std::string             m_key;

private:
    explicit                        SVGAssetDescriptor  ( const std::string & path );
protected:
    virtual VoidConstPtr            QueryThis	() const;

public:
    static const std::string &      UID         ();
    virtual const std::string &		GetUID		() const;

    virtual bool					IsCacheable	() const;

    const std::string &             GetPath     () const;
    virtual std::string				GetKey		() const;
//    virtual std::string             GetProposedShortKey () const;

    virtual SizeType                EstimateMemoryUsage () const;

    virtual void                    Serialize       ( ISerializer & ser ) const;
	ISerializableConstPtr			Create          ( const IDeserializer & deser );

    static SVGAssetDescriptorPtr    Create          ( const std::string & path);

private:

    std::string                         ComputeKey          () const;
};

// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string& GetAssetDescUID< SVGAsset >()
{
    return SVGAssetDescriptor::UID();
}

}
