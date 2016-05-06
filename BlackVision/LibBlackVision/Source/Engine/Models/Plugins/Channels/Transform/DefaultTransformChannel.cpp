#include "stdafx.h"

#include "DefaultTransformChannel.h"

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Types/Values/ValuesFactory.h"

namespace bv { namespace model
{

// *********************************
//
DefaultTransformChannel::DefaultTransformChannel( IPluginPtr prev, const ValueMat4Ptr & value, bool isReadOnly )
    : m_value( value )
    , m_isReadOnly( isReadOnly )
    , m_prevValue( nullptr )
{
    if( prev != nullptr && prev->GetTransformChannel() != nullptr )
    {
        m_prevValue = static_cast< const ValueMat4Ptr * >( &prev->GetTransformChannel()->GetTransformValue() );
    }
}

// *********************************
//
DefaultTransformChannel *   DefaultTransformChannel::Create              ()
{
    return new DefaultTransformChannel( nullptr, ValuesFactory::CreateValueMat4( "" ), true );
}

// *********************************
//
DefaultTransformChannel *   DefaultTransformChannel::Create              ( IPluginPtr prev, const ValueMat4Ptr & value, bool isReadOnly )
{
    return new DefaultTransformChannel( prev, value, isReadOnly );
}

// *********************************
//
const ValueMat4Ptr &        DefaultTransformChannel::GetTransformValue   ()  const
{
    return m_value;
}

// *********************************
//
bool                        DefaultTransformChannel::IsReadOnly          ()  const
{
    return m_isReadOnly;
}

// *********************************
//
void                        DefaultTransformChannel::PostUpdate          () 
{
    if( m_prevValue )
    {
        ValueMat4 & m   = *m_value;
        ValueMat4 & mp  = *(*m_prevValue);

        m.SetValue( mp.GetValue() * m.GetValue() ); //FIXME: be careful with multiplication order
    }
}

} // model
} // bv
