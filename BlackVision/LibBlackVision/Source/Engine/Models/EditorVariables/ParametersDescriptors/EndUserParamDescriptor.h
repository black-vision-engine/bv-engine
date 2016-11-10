#pragma once

#include "CoreDEF.h"


#include <string>
#include <vector>



namespace bv
{


class ISerializer;
class IDeserializer;


class EndUserParamDescriptor
{
private:

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
    void            Enable          ( bool enable )                     { m_enabled = enable; }

    const std::string &     GetName         ()  { return m_name; }
    const std::string &     GetDescription  ()  { return m_description; }
    int                     GetOrder        ()  { return m_order; }
    bool                    IsEnabled       ()  { return m_enabled; }

    void                    AddKeyTimeValue ( TimeType time );
    bool                    RemoveKeyTime   ( TimeType time );

    std::vector< TimeType > &       GetAvaibleTimeValues    ()                                          { return m_avaibleTimeValues; }
    void                            SetAvaibleTimeValues    ( std::vector< TimeType > && values );

    void                            Serialize       ( ISerializer & ser ) const;
    static EndUserParamDescriptor   Create          ( const IDeserializer & deser );

    std::vector< TimeType >         DeserializeTimeValues   ( const IDeserializer & deser );
};



}	// bv