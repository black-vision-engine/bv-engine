#include "MixChannelsNodeEffect.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// *********************************
//
MixChannelsNodeEffect::MixChannelsNodeEffect   ()
    : NodeEffect( NodeEffect::Type::T_MIX_CHANNELS )
{
    auto & values = AccessValues();

    // FIXME: string "alpha" used explicitely - suxx
    auto value = ValuesFactory::CreateValueInt( "channelMask" );
    value->SetValue( 228 ); //FIXME: suxx
    values.push_back( value );
}

// *********************************
//
MixChannelsNodeEffect::~MixChannelsNodeEffect  ()
{
}

} // bv
