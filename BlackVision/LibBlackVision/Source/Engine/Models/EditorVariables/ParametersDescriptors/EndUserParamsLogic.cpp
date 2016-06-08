#include "stdafx.h"
#include "EndUserParamsLogic.h"



namespace bv
{

// ***********************
//
EndUserParamsLogic::EndUserParamsLogic()
{}

// ***********************
//
EndUserParamsLogic::~EndUserParamsLogic()
{}

// ***********************
//
bool            EndUserParamsLogic::AddDescriptor   ( ParameterAddress && param, EndUserParamDescriptor && descriptor )
{
    m_paramsDescsMap[ param ] = descriptor;
    return true;
}

// ***********************
//
bool            EndUserParamsLogic::RemoveDescriptor( const ParameterAddress & param )
{
    auto iter = m_paramsDescsMap.find( param );
    if( iter != m_paramsDescsMap.end() )
    {
        m_paramsDescsMap.erase( iter );
        return true;
    }
    return false;
}

// ***********************
//
const EndUserParamDescriptor *      EndUserParamsLogic::GetDescriptor   ( const ParameterAddress & param )
{
    auto iter = m_paramsDescsMap.find( param );
    if( iter != m_paramsDescsMap.end() )
    {
        m_paramsDescsMap.erase( iter );
        return &iter->second;
    }
    return nullptr;
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void            EndUserParamsLogic::Serialize       ( ISerializer & ser ) const
{
    ser.EnterArray( "endUserParams" );

        for( auto & paramMapping : m_paramsDescsMap )
        {
            ser.EnterChild( "mapping" );
                ser.EnterChild( "param" );
                paramMapping.first.Serialize( ser );
                ser.ExitChild();    // param

                paramMapping.second.Serialize( ser );
            ser.ExitChild();    // mapping
        }

    ser.ExitChild();    // endUserParams
}

// ***********************
//
void            EndUserParamsLogic::Deserialize     ( const IDeserializer & deser )
{
    if( deser.EnterChild( "endUserParams" ) )
    {
        if( deser.EnterChild( "mapping" ) )
        {
            do
            {
                bool paramValid = false;
                bool descriptorValid = false;

                ParameterAddress param;
                if( deser.EnterChild( "param" ) )
                {
                    param = ParameterAddress::Create( deser );

                    deser.ExitChild();  // param
                    paramValid = true;
                }

                EndUserParamDescriptor descriptor;
                if( deser.EnterChild( "paramDescriptor" ) )
                {
                    descriptor = EndUserParamDescriptor::Create( deser );

                    deser.ExitChild();  // paramDescriptor
                    descriptorValid = true;
                }

                if( paramValid && descriptorValid )
                    AddDescriptor( std::move( param  ), std::move( descriptor ) );

            } while( deser.NextChild() );

            deser.ExitChild();  // mapping
        }
        deser.ExitChild();  // endUserParams
    }
}

}	// bv