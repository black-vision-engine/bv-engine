#pragma once

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"


namespace bv { namespace model {

class SimpleIParamSet : public IParamSet
{
private:

    std::vector< IParameterPtr >     m_parameters;

public:

    virtual std::vector< IParameterPtr > &      GetParameters       () override;
    virtual IParameterPtr                       GetParameter        ( const std::string & name ) override;

    bool                                        AddParameter        ( IParameterPtr param );
    bool                                        RemoveParameter     ( IParameterPtr param );
    unsigned int                                RemoveParameters    ( const std::string & name );

};

} //model
} //bv
