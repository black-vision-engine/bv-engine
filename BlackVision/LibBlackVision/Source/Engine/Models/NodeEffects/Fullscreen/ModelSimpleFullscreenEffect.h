#pragma once

#include "Engine/Models/Interfaces/IModelFullscreenEffect.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model {

class ModelSimpleFullscreenEffect : public IModelFullscreenEffect
{
protected:

    std::string                 m_name;

    DefaultParamValModelPtr     m_paramValModel;

public:

                                            ModelSimpleFullscreenEffect ( const std::string & name );

    const std::string &			            GetName			() const override;

    virtual void                            Update          ( TimeType t ) override;

    virtual IParameterPtr                           GetParameter    ( const std::string & name ) const override;
    virtual const std::vector< IParameterPtr > &    GetParameters   () const override;

    virtual const std::vector< IValueConstPtr > &   GetValues       () const override;

};

} // model
} // bv
