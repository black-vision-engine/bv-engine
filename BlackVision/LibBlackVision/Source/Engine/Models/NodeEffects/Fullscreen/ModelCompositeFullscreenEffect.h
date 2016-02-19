#pragma once

#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model {

class ModelCompositeFullscreenEffect : public IModelFullscreenEffect
{
private:
    
    std::vector< IModelFullscreenEffectPtr >    m_effects;
    std::vector< IParameterPtr >                m_parameters;
    std::vector< IValueConstPtr >               m_values;

protected:

    std::string                                 m_name;

public:

                                            ModelCompositeFullscreenEffect  ( const std::string & name );

    const std::string &		                GetName			() const override;

    virtual void                            Update          ( TimeType t ) override;

    IParameterPtr                           GetParameter    ( const std::string & name ) const override;
    const std::vector< IParameterPtr > &    GetParameters   () const override;

    const std::vector< IValueConstPtr > &   GetValues       () const override;

protected:

    void                                    AddEffect       ( IModelFullscreenEffectPtr effect );

};

} // model
} // bv
