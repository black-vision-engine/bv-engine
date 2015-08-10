#include "DefaultTransformChannel.h"

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Types/Values/ValuesFactory.h"

namespace bv { namespace model
{

// *********************************
//
DefaultTransformChannel::DefaultTransformChannel( IPluginPtr prev, const ValueMat4PtrVec & values, bool isReadOnly )
    : m_values( values )
    , m_isReadOnly( isReadOnly )
    , m_prevValues( nullptr )
{
    if( prev != nullptr && prev->GetTransformChannel() != nullptr )
	{
        m_prevValues = static_cast< const ValueMat4PtrVec * >( &prev->GetTransformChannel()->GetTransformValues() );

		assert( m_prevValues->size() == values.size() );
    }
}

// *********************************
//
DefaultTransformChannel *   DefaultTransformChannel::Create              ()
{
	ValueMat4PtrVec values;
    values.push_back( ValuesFactory::CreateValueMat4( "" ) );
    return new DefaultTransformChannel( nullptr, values, true );
}

// *********************************
//
DefaultTransformChannel *   DefaultTransformChannel::Create              ( IPluginPtr prev, const ValueMat4PtrVec & values, bool isReadOnly )
{
    return new DefaultTransformChannel( prev, values, isReadOnly );
}

// *********************************
//
const ValueMat4PtrVec &     DefaultTransformChannel::GetTransformValues  ()  const
{
    return m_values;
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
    if( m_prevValues )
    {
        for( unsigned int i = 0; i < m_values.size(); ++i )
        {
            
            ValueMat4 & m   = *m_values[ i ];
            ValueMat4 & mp  = *(*m_prevValues)[ i ];

            m.SetValue( mp.GetValue() * m.GetValue() ); //FIXME: be careful with multiplication order
        }
    }
}

} // model
} // bv
