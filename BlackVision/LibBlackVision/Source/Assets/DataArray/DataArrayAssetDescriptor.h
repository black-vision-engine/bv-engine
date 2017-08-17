#pragma once

#include "Assets/AssetDescriptor.h"

#include "CoreDEF.h"
#include "Engine/Types/Enums.h"

#include <string>
#include <vector>




namespace bv
{

class DataArrayAssetDescriptor : public AssetDesc, public std::enable_shared_from_this< AssetDesc >
{
private:
    static const std::string            uid;

    std::vector< std::string >          m_rowNames;
    std::vector< std::string >          m_rows;
    std::vector< ModelParamType >       m_rowTypes;

    mutable std::string                 m_key;

public:
    virtual void                        Serialize           ( ISerializer& ser ) const;
    virtual void                        Deserialize         ( const IDeserializer& deser );
	static ISerializableConstPtr        Create              ( const IDeserializer& deser );

	virtual bool						IsCacheable			() const override;

	virtual VoidConstPtr				QueryThis			() const override;

	virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;

    virtual SizeType                    EstimateMemoryUsage () const override;

    static const std::string &			UID					();


    const std::vector< std::string > &      GetRowNames     () const;
    const std::vector< std::string > &      GetRows         () const;
    const std::vector< ModelParamType > &   GetRowTypes     () const;

    static DataArrayAssetDescriptorConstPtr Create          ( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types );

protected:

	virtual const std::string &			GetUID				() const override;

private:

    std::string                         ComputeKey          () const;

    DataArrayAssetDescriptor    ( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types );
};



}	// bv