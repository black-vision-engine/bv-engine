#include "stdafx.h"
#include "EndUserParamDescriptor.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"




#include "Memory/MemoryLeaks.h"



namespace bv
{

const float FloatTolarance = 0.0001f;


// ***********************
//
EndUserParamDescriptor::EndUserParamDescriptor()
{}

// ***********************
//
EndUserParamDescriptor::~EndUserParamDescriptor()
{}

// ***********************
//
void            EndUserParamDescriptor::AddKeyTimeValue ( TimeType time )
{
    bool exists = false;
    for( TimeType timeValue : m_avaibleTimeValues )
    {
        if( abs( timeValue - time ) < FloatTolarance )
            exists = true;
    }

    if( !exists )
        m_avaibleTimeValues.push_back( time );
}
// ***********************
//
bool            EndUserParamDescriptor::RemoveKeyTime   ( TimeType time )
{
    for( auto iter = m_avaibleTimeValues.begin(); iter != m_avaibleTimeValues.end(); ++iter )
    {
        if( *iter == time )
        {
            iter = m_avaibleTimeValues.erase( iter );
            return true;
        }
    }
    return false;
}

// ***********************
//
void            EndUserParamDescriptor::SetAvaibleTimeValues    ( std::vector< TimeType > && values )
{
    m_avaibleTimeValues = values;
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void            EndUserParamDescriptor::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "paramDescriptor" );
    
    ser.SetAttribute( "name", m_name );
    ser.SetAttribute( "description", m_description );
    ser.SetAttribute( "order", Convert::T2String( m_order ) );
    ser.SetAttribute( "enabled", Convert::T2String( m_enabled ) );

    ser.EnterArray( "avaibleTimeValues" );
    for( auto & time : m_avaibleTimeValues )
    {
        ser.EnterChild( "time" );
        ser.SetAttribute( "timeValue", Convert::T2String( time ) );
        ser.ExitChild();    // time
    }
    ser.ExitChild();    // keyTime

    ser.ExitChild();    // descriptor
}

// ***********************
//
EndUserParamDescriptor      EndUserParamDescriptor::Create          ( const IDeserializer & deser )
{
    EndUserParamDescriptor descriptor;

    descriptor.SetName( deser.GetAttribute( "name" ) );
    descriptor.SetDescription( deser.GetAttribute( "description" ) );
    descriptor.SetOrder( Convert::String2T< int >( deser.GetAttribute( "order" ), 0 ) );
    descriptor.Enable( Convert::String2T( deser.GetAttribute( "enable" ), false ) );

    if( deser.EnterChild( "avaibleTimeValues" ) )
    {
        if( deser.EnterChild( "time" ) )
        {
            do
            {
                Expected< float > key = Convert::String2T< float >( deser.GetAttribute( "timeValue" ) );
                if( key.IsValid() )
                {
                    descriptor.AddKeyTimeValue( key );
                }

            } while( deser.NextChild() );

            deser.ExitChild();  // time
        }
        
        deser.ExitChild();  //  avaibleTimeValues
    }

    return descriptor;
}


}	// bv