#pragma once

#include "Assets/AssetDescriptor.h"

namespace bv {

class SVGAssetDescriptor;
DEFINE_PTR_TYPE( SVGAssetDescriptor );

class SVGAssetDescriptor : public AssetDesc,  public std::enable_shared_from_this< AssetDesc >
{
    std::string                     m_path;

    explicit                        SVGAssetDescriptor  ( const std::string & path );
protected:
    virtual VoidConstPtr            QueryThis	() const;

public:
    static const std::string &      UID         ();
    virtual const std::string &		GetUID		() const;

    virtual bool					IsCacheable	() const;

    virtual std::string				GetKey		() const;
//    virtual std::string             GetProposedShortKey () const;

    virtual SizeType                EstimateMemoryUsage () const;

    virtual void                    Serialize       ( ISerializer & ser ) const;

    static SVGAssetDescriptorPtr    Create          ( const std::string & path);

    //template< typename DescTypeConstPtr >
    //friend DescTypeConstPtr  QueryTypedDesc( AssetDescConstPtr desc );
};

}
