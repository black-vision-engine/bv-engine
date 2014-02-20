#include "DefaultTransformChannel.h"

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"


namespace bv { namespace model
{

// *********************************
//
DefaultTransformChannel::DefaultTransformChannel( const IPlugin * prev, const ValueMat4PtrVec & values, bool isReadOnly )
    : m_values( values )
    , m_isReadOnly( isReadOnly )
    , m_prevValues( nullptr )
{
    if( prev != nullptr && prev->GetTransformChannel() != nullptr )
    {
        assert( m_prevValues->size() == values.size() );

        m_prevValues = static_cast< const ValueMat4PtrVec * >( &prev->GetTransformChannel()->GetTransformValues() );
    }
}

// *********************************
//
DefaultTransformChannel *   DefaultTransformChannel::Create              ( const IPlugin * prev, IValueSet * values, bool isReadOnly )
{
    assert( values );

    ValueMat4PtrVec typedVals;

    for( auto val : values->GetValues() )
    {
        typedVals.push_back( QueryTypedValue< ValueMat4 >( val ) );
    }

    return new DefaultTransformChannel( prev, typedVals, isReadOnly );
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

            m.SetValue( m.GetValue()  * mp.GetValue() );
        }
    }
}

} // model
} // bv

