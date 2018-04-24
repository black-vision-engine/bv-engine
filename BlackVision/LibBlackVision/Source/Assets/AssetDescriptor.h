#pragma once

#include "CoreDEF.h"
#include "Assets/FwdDecls.h"

#include "Serialization/ISerializable.h"

#include <string>

namespace bv
{



/**@brief Base Asset Descriptor class
@ingroup Model*/
class AssetDesc : public ISerializable
{
protected:
    virtual VoidConstPtr            QueryThis	() const = 0;

public:
    virtual const std::string &		GetUID		() const = 0;

    virtual bool					IsCacheable	() const = 0;

    virtual std::string				GetKey		() const;
    virtual std::string             GetProposedShortKey () const;

    virtual SizeType                EstimateMemoryUsage () const = 0;

    template< typename DescTypeConstPtr >
    friend DescTypeConstPtr  QueryTypedDesc( AssetDescConstPtr desc );
};

// *********************************
//
template< typename DescTypeConstPtr >
DescTypeConstPtr  QueryTypedDesc( AssetDescConstPtr desc )
{
    if( desc->GetUID() != DescTypeConstPtr::element_type::UID() )
    {
        return nullptr;
    }

    return std::static_pointer_cast< DescTypeConstPtr::element_type >( desc->QueryThis() );
}


// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
///
/// Specialize this template for every asset.
template<typename AssetType>
inline const std::string& GetAssetDescUID()
{
    return "";
}

} // bv
