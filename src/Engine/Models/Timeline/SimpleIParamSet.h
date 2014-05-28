#pragma once

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"


namespace bv { namespace model {

class SimpleIParamSet : public IParamSet
{
private:

    std::vector< IParameter * >     m_parameters;

public:

    virtual std::vector< IParameter * > &       GetParameters       () override;
    virtual IParameter *                        GetParameter        ( const std::string & name ) override;

    bool                                        AddParameter        ( IParameter * param );
    bool                                        RemoveParameter     ( IParameter * param );
    unsigned int                                RemoveParameters    ( const std::string & name );

};

} //model
} //bv
