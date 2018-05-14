#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

#include "Assets/Asset.h"
#include "Assets/DataArray/DataArrayAsset.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultLineChartPluginDesc : public DefaultGeometryPluginDescBase
{
public:

    DefaultLineChartPluginDesc                               				();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};




// ***************************** PLUGIN ********************************** 
/// @ingroup PluginsList
class DefaultLineChartPlugin : public DefaultGeometryPluginBase
{
public:

    struct PARAMS
    {
        static const std::string        WIDTH;
    };

    static const std::string            AssetName;

private:

    DataArrayAssetConstPtr          m_asset;
    bool                            m_assetUpdated;

public:

    explicit                                    DefaultLineChartPlugin   ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    ~DefaultLineChartPlugin  ();

private:
    virtual std::vector<IGeometryGeneratorPtr>	GetGenerators               () override;

    virtual bool                                NeedsTopologyUpdate         () override;
    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;
};


} // model
} // bv


