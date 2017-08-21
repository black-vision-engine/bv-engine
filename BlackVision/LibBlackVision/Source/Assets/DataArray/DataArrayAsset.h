#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Assets/DataArray/DataArrayAssetDescriptor.h"
#include "DataArrayRow.h"
#include "DataArrayTypedRows.h"


namespace bv
{



class DataArrayAsset : public Asset, public std::enable_shared_from_this< DataArrayAsset >
{
private:
    static const std::string		    uid;


    std::vector< DataArrayRowBase * >   m_rows;

public:
    explicit                        DataArrayAsset  ();
                                    ~DataArrayAsset ();

    static DataArrayAssetConstPtr	Create			();
    static const std::string &	    UID				();

    virtual const std::string &	    GetUID			() const override;

    bool                            IsEmpty         () const;
    
    DataArrayRowBase *              GetRow          ( const std::string & rowName ) const;
    DataArrayRowBase *              GetRow          ( int idx ) const;


    template< typename RowType >
    DataArrayRow< RowType > *       GetRow          ( const std::string & rowName ) const;

    template< typename RowType >
    DataArrayRow< RowType > *       GetRow          ( int idx ) const;


    static DataArrayAssetConstPtr	Create			( std::vector< DataArrayRowBase * > && rows );

protected:
    virtual VoidConstPtr            QueryThis		() const override;


private:

    explicit                        DataArrayAsset  ( std::vector< DataArrayRowBase * > && rows );
};

// ***********************
//
template< typename RowType >
DataArrayRow< RowType > *       DataArrayAsset::GetRow          ( const std::string & rowName ) const
{
    auto row = GetRow( rowName );
    if( row->GetType() == DataArrayRow::GetParamType< RowType >() )
        return row;
    return nullptr;
}

// ***********************
//
template< typename RowType >
DataArrayRow< RowType > *       DataArrayAsset::GetRow          ( int idx ) const
{
    auto row = GetRow( idx );
    if( row->GetType() == DataArrayRow::GetParamType< RowType >() )
        return row;
    return nullptr;
}


class DataArrayAsset;

// ***********************
/// Returns AssetDescriptor UID for Asset in template parameter.
/// @note AssetDescriptor uid and Asset uid are different strings.
template<> inline const std::string& GetAssetDescUID<DataArrayAsset>()
{
    return DataArrayAssetDesc::UID();
}



}	// bv