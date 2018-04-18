#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



namespace bv {
namespace model {

// ***************************** DESCRIPTOR **********************************
class LinesPluginDesc : public DefaultGeometryPluginDescBase
{
public:

    LinesPluginDesc                               				();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};




// ***************************** PLUGIN ********************************** 
///@ingroup PluginsList
class LinesPlugin : public DefaultGeometryPluginBase
{
public:

    static const std::string            AssetName;

protected:

    DataArrayAssetConstPtr          m_asset;
    bool                            m_assetUpdated;

public:

    explicit                                    LinesPlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    virtual                                     ~LinesPlugin  ();

private:
    virtual std::vector<IGeometryGeneratorPtr>	GetGenerators           () override;

    virtual bool                                NeedsTopologyUpdate     () override;
    virtual bool                                LoadResource            ( AssetDescConstPtr assetDescr ) override;
};


} // model
} // bv





