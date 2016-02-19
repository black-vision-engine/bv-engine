#include "ModelCompositeFullscreenEffect.h"


namespace bv { namespace model {


// ********************************
//
ModelCompositeFullscreenEffect::ModelCompositeFullscreenEffect  ( const std::string & name )
    : m_name( name )
{
}

// ********************************
//
const std::string &                         ModelCompositeFullscreenEffect::GetName         () const
{
    return m_name;
}

// ********************************
//
void                                        ModelCompositeFullscreenEffect::Update          ( TimeType t )
{
    for( auto & effect : m_effects )
    {
        effect->Update( t );
    }
}

// ********************************
//
model::IParameterPtr                        ModelCompositeFullscreenEffect::GetParameter    ( const std::string & name ) const
{
    for( auto & param : m_parameters )
    {
        if( param->GetName() == name )
        {
            return param;
        }
    }

    return nullptr;
}

// ********************************
//
const std::vector< IParameterPtr > &        ModelCompositeFullscreenEffect::GetParameters   () const
{
    return m_parameters;
}

// ********************************
//
const std::vector< bv::IValueConstPtr > &   ModelCompositeFullscreenEffect::GetValues       () const
{
    return m_values;
}

// ********************************
//
void            ModelCompositeFullscreenEffect::AddEffect      ( IModelFullscreenEffectPtr effect )
{
    m_effects.push_back( effect );

    for( auto & param : effect->GetParameters() )
    {
        m_parameters.push_back( param );
    }

    for( auto & value : effect->GetValues() )
    {
        m_values.push_back( value );
    }
}

} // model
} // bv
