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

    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;
    virtual IPluginParamValModelPtr         CreateModel         ( ITimeEvaluatorPtr timeEvaluator ) const override;

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const = 0;

    template< typename PluginType >
    std::shared_ptr< PluginType >           CreatePluginTyped    ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator  ) const
    {
        return std::make_shared< PluginType >( name, GetPluginTypeUID(), prev, DefaultPluginParamValModelPtr( CreateDefaultModel( timeEvaluator ) ) );
    }

    virtual ~BasePluginDescriptor () {}

protected:
    DefaultParamValModelPtr                  CreateVacModel      ( DefaultPluginParamValModelPtr&, ITimeEvaluatorPtr ) const;
    template< typename T >
    void                                     AddParam            ( DefaultParamValModelPtr&, ITimeEvaluatorPtr timeEvaluator, std::string name, const T& defaultValue ) const;
};

} //model
} //bv
