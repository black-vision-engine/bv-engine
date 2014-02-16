#include "DefaultTransformChannel.h"

#include "Engine/Models/Plugins/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Interfaces/IValueSet.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"

namespace bv { namespace model
{

// *********************************
//
DefaultTransformChannel::DefaultTransformChannel( IPlugin * prev, const ValueMat4PtrVec & values, bool isReadOnly )
    : m_values( values )
    , m_isReadOnly( isReadOnly )
    , m_prevChannel( nullptr )
{
    if( prev != nullptr )
    {

        //TODO: implement
    }

}

// *********************************
//
DefaultTransformChannel *   DefaultTransformChannel::Create              ( IPlugin * prev, IValueSet * values, bool isReadOnly )
{
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
    //TODO: concatenate with prev plugin results
}

} // model
} // bv
