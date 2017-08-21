#pragma once

#include "Assets/AssetDescriptor.h"

#include "CoreDEF.h"
#include "Engine/Types/Enums.h"

#include <string>
#include <vector>

#include "DataArrayRow.h"
#include "DataArrayTypedRows.h"




namespace bv
{


// ***********************
//
class DataArrayAssetDesc : public AssetDesc, public std::enable_shared_from_this< AssetDesc >
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

    static DataArrayAssetDescConstPtr   Create              ( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types );

protected:

	virtual const std::string &			GetUID				() const override;

private:

    std::string                         ComputeKey          () const;

    DataArrayAssetDesc    ( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types );
};


// ***********************
//
class DataArrayRowAssetDesc : public AssetDesc, public std::enable_shared_from_this< AssetDesc >
{
private:
    
    static const std::string            uid;

    std::vector< DataArrayRowBase * >   m_rows;

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

    const std::vector< DataArrayRowBase * > GetRows         () const { return m_rows; }


    static DataArrayRowAssetDescConstPtr    Create          ( std::vector< DataArrayRowBase * > && rows );

protected:

    virtual const std::string &			GetUID				() const override;

private:

    std::string                         ComputeKey          () const;

    explicit        DataArrayRowAssetDesc             ( std::vector< DataArrayRowBase * > && rows );

};


}	// bv