#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetManager.h"		// Only for LoadTypedAsset template specialization

#include "Assets/DataArray/DataArrayAssetDescriptor.h"
#include "DataArrayRow.h"
#include "DataArrayTypedRows.h"


namespace bv
{



/**@brief Asset with data.
@details

@ref DataArrayAsset "DataArrayAsset" exist to pass data to @ref Plugins "Plugins" and @ref NodeLogics "Logics",
like for example set of points to @ref bv::model::DefaultLineChartPlugin.
DataArrayAsset isn't loaded from disk, instead data is passed directly in descriptor.

<b>Structure</b>

Data consists of multiple named and typed rows. Each row contains string <b>data</b> with serialized values separated by semicolon.
Note that name of row is very important for Plugin, to distinguish, which row should be used.

-                       | UID                       | Class
----------------------- | ------------------------- | -----------
Descriptor              | DATA_ARRAY_ASSET_DESC     | @ref bv::DataArrayAssetDesc
Asset                   | DATA_ARRAY_ASSET          | @ref bv::DataArrayAsset

<b>Example serialized descriptor:</b>

@code{.json}
{
    "asset" :
    {
        "type" : "DATA_ARRAY_ASSET_DESC",
        "dataRows" :
        [
            {
                "name" : "Plot",
                "type" : "vec2",
                "data" : "-3.0, 0.3; -0.4, 1.0; 0.0, 0.5; 0.3, -0.1; 0.6, -0.4; 0.7, 0.4;"
            }
        ]
    }
}
@endcode

@ingroup Assets*/
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