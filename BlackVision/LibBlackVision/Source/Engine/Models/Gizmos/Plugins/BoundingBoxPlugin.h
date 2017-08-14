#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



namespace bv {
namespace model {

// ***************************** DESCRIPTOR **********************************
class BoundingBoxPluginDesc : public DefaultGeometryPluginDescBase
{
public:

    BoundingBoxPluginDesc                               				();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};




// ***************************** PLUGIN ********************************** 
class BoundingBoxPlugin : public DefaultGeometryPluginBase
{
public:

    struct PARAMS
    {
        static const std::string        WIDTH;
    };

protected:


public:

    explicit                                    BoundingBoxPlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    ~BoundingBoxPlugin  ();

private:
    virtual std::vector<IGeometryGeneratorPtr>	GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate() override;
};


} // model
} // bv




