#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"


namespace bv { namespace model {

class BasePluginDescriptor : public IPluginDescriptor
{
private:

    std::string     m_uid;
    std::string     m_defaultName;

protected:

    BasePluginDescriptor    ( const std::string & uid, const std::string & defaultName );

public:

    virtual const std::string &             GetPluginTypeUID    () const override;
    virtual const std::string &             DefaultPluginName   () const override;

    virtual bool                            CanBeAttachedTo     ( const IPlugin * plugin )  const override;
    virtual IPluginParamValModelPtr         CreateModel         ( ITimeEvaluatorPtr timeEvaluator ) const override;

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const = 0;

    template< typename PluginType >
    PluginType *                           CreatePluginTyped    ( const std::string & name, const IPlugin * prev, ITimeEvaluatorPtr timeEvaluator  ) const
    {
        return new PluginType( name, GetPluginTypeUID(), prev, DefaultPluginParamValModelPtr( CreateDefaultModel( timeEvaluator ) ) );
    }

    virtual ~BasePluginDescriptor () {}

};

} //model
} //bv
