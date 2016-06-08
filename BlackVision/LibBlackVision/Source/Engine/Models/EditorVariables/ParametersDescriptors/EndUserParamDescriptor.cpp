#include "stdafx.h"
#include "EndUserParamDescriptor.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"


namespace bv
{




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
    ser.SetAttribute( "order", SerializationHelper::T2String( m_order ) );
    ser.SetAttribute( "enabled", SerializationHelper::T2String( m_enabled ) );

    ser.EnterArray( "avaibleTimeValues" );
    for( auto & time : m_avaibleTimeValues )
    {
        ser.EnterChild( "time" );
        ser.SetAttribute( "timeValue", SerializationHelper::T2String( time ) );
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
    descriptor.SetName( deser.GetAttribute( "description" ) );
    descriptor.SetOrder( SerializationHelper::String2T< int >( deser.GetAttribute( "order" ), 0 ) );
    descriptor.Enable( SerializationHelper::String2T( deser.GetAttribute( "enable" ), false ) );

    if( deser.EnterChild( "avaibleTimeValues" ) )
    {
        if( deser.EnterChild( "time" ) )
        {
            do
            {
                Expected< float > key = SerializationHelper::String2T< float >( "timeValue" );
                if( key.isValid )
                {
                    descriptor.AddKeyTimeValue( key.ham );
                }

            } while( deser.NextChild() );

            deser.ExitChild();  // time
        }
        
        deser.ExitChild();  //  avaibleTimeValues
    }

    return descriptor;
}

}	// bv