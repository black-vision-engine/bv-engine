#include "ModelSimpleFullscreenEffect.h"


namespace bv { namespace model {


// ********************************
//
ModelSimpleFullscreenEffect::ModelSimpleFullscreenEffect  ( const std::string & name )
    : m_name( name )
    , m_paramValModel( std::make_shared< DefaultParamValModel >() )
{
}

// ********************************
//
const std::string &                         ModelSimpleFullscreenEffect::GetName    () const
{
    return m_name;
}

// ********************************
//
void                                        ModelSimpleFullscreenEffect::Update     ( TimeType t )
{
    { t; }
    m_paramValModel->Update();
}

// ********************************
//
model::IParameterPtr                        ModelSimpleFullscreenEffect::GetParameter   ( const std::string & name ) const
{
    return m_paramValModel->GetParameter( name );
}

// ********************************
//
const std::vector< IParameterPtr > &        ModelSimpleFullscreenEffect::GetParameters   () const
{
    return m_paramValModel->GetParameters();
}

// ********************************
//
const std::vector< bv::IValueConstPtr > &   ModelSimpleFullscreenEffect::GetValues      () const
{
    return m_paramValModel->GetValues();
}

} // model
} // bv
