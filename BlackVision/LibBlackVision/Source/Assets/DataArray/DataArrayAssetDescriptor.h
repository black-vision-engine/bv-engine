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
enum class DataArrayDescType
{
    Row,
    Strings
};


// ***********************
//
class DataArrayAssetBaseDesc : public AssetDesc, public std::enable_shared_from_this< AssetDesc >
{
private:

    static const std::string            uid;

    DataArrayDescType                   m_type;

public:

    virtual bool						IsCacheable			() const override;
    virtual VoidConstPtr				QueryThis			() const override;
    
    static const std::string &			UID					();


    DataArrayDescType                   GetDescType         () const { return m_type; }

protected:

    virtual const std::string &			GetUID				() const override;

protected:

    DataArrayAssetBaseDesc    ( DataArrayDescType type );
};


// ***********************
//
class DataArrayAssetDesc : public DataArrayAssetBaseDesc
{
private:

    std::vector< std::string >          m_rowNames;
    std::vector< std::string >          m_rows;
    std::vector< ModelParamType >       m_rowTypes;

    mutable std::string                 m_key;

public:
    virtual void                        Serialize           ( ISerializer& ser ) const;
    virtual void                        Deserialize         ( const IDeserializer& deser );
	static ISerializableConstPtr        Create              ( const IDeserializer& deser );

	virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;

    virtual SizeType                    EstimateMemoryUsage () const override;


    const std::vector< std::string > &      GetRowNames     () const;
    const std::vector< std::string > &      GetRows         () const;
    const std::vector< ModelParamType > &   GetRowTypes     () const;

    static DataArrayAssetDescConstPtr   Create              ( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types );

private:

    std::string                         ComputeKey          () const;

    DataArrayAssetDesc    ( std::vector< std::string > && rowNames, std::vector< std::string > && rows, std::vector< ModelParamType > && types );
};


// ***********************
//
class DataArrayRowAssetDesc : public DataArrayAssetBaseDesc
{
private:

    std::vector< DataArrayRowBase * >   m_rows;

    mutable std::string                 m_key;

public:

    virtual void                        Serialize           ( ISerializer& ser ) const;
    virtual void                        Deserialize         ( const IDeserializer& deser );
    static ISerializableConstPtr        Create              ( const IDeserializer& deser );

    virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;

    virtual SizeType                    EstimateMemoryUsage () const override;

    const std::vector< DataArrayRowBase * > GetRows         () const { return m_rows; }


    static DataArrayRowAssetDescConstPtr    Create          ( std::vector< DataArrayRowBase * > && rows );

private:

    std::string                         ComputeKey          () const;

    explicit        DataArrayRowAssetDesc             ( std::vector< DataArrayRowBase * > && rows );

};


}	// bv