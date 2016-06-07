#pragma once

#include "Engine/Models/EditorVariables/ParametersDescriptors/ParamAddress.h"

#include <string>
#include <vector>


class ISerializer;
class IDeserializer;

namespace bv
{


class EndUserParamDescriptor
{
private:

    ParameterAddress            m_paramPath;

    std::string                 m_name;
    std::string                 m_description;

    int                         m_order;
    std::vector< TimeType >     m_avaibleTimeValues;

    bool                        m_enabled;

public:
    EndUserParamDescriptor();
    ~EndUserParamDescriptor();

    void            SetName         ( const std::string & newName )     { m_name = newName; }
    void            SetDescription  ( const std::string & description ) { m_description = description; }
    void            SetOrder        ( int order )                       { m_order = order; }
    void            Enabel          ( bool enable )                     { m_enabled = enable; }

    const std::string &     GetName         ()  { return m_name; }
    const std::string &     GetDescription  ()  { return m_description; }
    int                     GetOrder        ()  { return m_order; }
    bool                    IsEnabled       ()  { return m_enabled; }


    void                    Serialize       ( ISerializer & ser ) const;
    void                    Deserialize     ( const IDeserializer & deser );

};



}	// bv