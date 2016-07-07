#pragma once

#include "DataArrayRow.h"
#include "Mathematics/glm_inc.h"

#include <vector>



namespace bv
{

template< typename RowType >
class DataArrayRow : public DataArrayRowBase
{
public:
    template< typename Type >
    static ModelParamType       GetParamType()
    {
        static_assert( false, "Specialize DataArrayRow::GetParamType function for this parameter type." );
    }

    template<> static ModelParamType    GetParamType< float >() { return ModelParamType::MPT_FLOAT; }
    template<> static ModelParamType    GetParamType< glm::vec2 >() { return ModelParamType::MPT_VEC2; }
    template<> static ModelParamType    GetParamType< glm::vec3 >() { return ModelParamType::MPT_VEC3; }
    template<> static ModelParamType    GetParamType< glm::vec4 >() { return ModelParamType::MPT_VEC4; }

    template<> static ModelParamType    GetParamType< int >() { return ModelParamType::MPT_INT; }
    template<> static ModelParamType    GetParamType< bool >() { return ModelParamType::MPT_BOOL; }

    template<> static ModelParamType    GetParamType< std::string >() { return ModelParamType::MPT_STRING; }
    template<> static ModelParamType    GetParamType< std::wstring >() { return ModelParamType::MPT_WSTRING; }



private:

    std::vector< RowType >        m_array;

public:
    DataArrayRow( const std::string & name, std::vector< RowType > && row )
        :   DataArrayRowBase( name, GetParamType< RowType >() )
        ,   m_array( std::move( row ) )
    {}


    const std::vector< RowType > &  GetArray()  { return m_array; }
};



}	// bv


