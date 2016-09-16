#include "stdafx.h"
#include "DataArrayAsset.h"





#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string DataArrayAsset::uid = "DATA_ARRAY_ASSET";

// ***********************
//
DataArrayAsset::DataArrayAsset  ( std::vector< DataArrayRowBase * > && rows )
    :   m_rows( std::move( rows ) )
{}

// ***********************
//
DataArrayAsset::~DataArrayAsset()
{
    for( auto row : m_rows )
        delete row;
}

// ***********************
//
DataArrayAssetConstPtr	DataArrayAsset::Create			( std::vector< DataArrayRowBase * > && rows )
{
    return DataArrayAssetConstPtr( new DataArrayAsset( std::move( rows ) ) );
}

// ***********************
//
DataArrayRowBase *      DataArrayAsset::GetRow          ( const std::string & rowName ) const
{
    for( auto row : m_rows )
    {
        if( row->GetName() == rowName )
            return row;
    }

    return nullptr;
}

// ***********************
//
DataArrayRowBase *      DataArrayAsset::GetRow          ( int idx ) const
{
    assert( idx < ( Int32 )m_rows.size() );
    return m_rows[ idx ];
}

// *******************************
// 
const std::string &     DataArrayAsset::GetUID() const
{
	return DataArrayAsset::uid;
}

// *******************************
// 
const std::string &     DataArrayAsset::UID()
{
	return DataArrayAsset::uid;
}

// *******************************
// 
VoidConstPtr            DataArrayAsset::QueryThis() const
{
	return shared_from_this();
}

// ***********************
//
bool                    DataArrayAsset::IsEmpty         () const
{
    return m_rows.size() == 0 ? true : false;
}

}	// bv