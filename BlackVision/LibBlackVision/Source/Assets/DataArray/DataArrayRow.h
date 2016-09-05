#pragma once

#include "Engine/Types/Enums.h"


#include <string>


namespace bv
{

class DataArrayRowBase
{
private:
    ModelParamType  m_type;
    std::string     m_rowName;

public:
    DataArrayRowBase( const std::string & name, ModelParamType type )
        :   m_type( type )
        ,   m_rowName( name )
    {}

    ModelParamType          GetType()       { return m_type; }
    const std::string       GetName()       { return m_rowName; }
};


}	// bv
