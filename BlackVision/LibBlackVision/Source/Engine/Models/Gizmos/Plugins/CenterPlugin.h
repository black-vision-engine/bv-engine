#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv {
namespace model {

// ***************************** DESCRIPTOR **********************************
class CenterPluginDesc : public DefaultGeometryPluginDescBase
{
public:

    CenterPluginDesc                               				();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};




// ***************************** PLUGIN ********************************** 
class CenterPlugin : public DefaultGeometryPluginBase
{
public:

    struct PARAMS
    {
        static const std::string        SIZE;
    };

protected:


public:

    explicit                                    CenterPlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    virtual                                     ~CenterPlugin  ();

private:
    virtual std::vector<IGeometryGeneratorPtr>	GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate() override;
};


} // model
} // bv



