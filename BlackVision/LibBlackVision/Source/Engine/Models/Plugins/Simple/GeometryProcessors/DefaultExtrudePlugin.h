#pragma once

#include "Engine/Models/Plugins/Simple/GeometryProcessors/DefaultGeometryProcessorBase.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultExtrudePluginDesc : public DefaultGeometryProcessorDescBase
{
public:

    DefaultExtrudePluginDesc                ();


    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();

};

// ***************************** PLUGIN ********************************** 
class DefaultExtrudePlugin : public DefaultGeometryProcessorBase
{
public:

    struct PARAMS
    {
        static const std::string        ALPHA;
    };

public:

    explicit            DefaultExtrudePlugin   (    const std::string & name,
                                                                            const std::string & uid,
                                                                            IPluginPtr prev,
                                                                            DefaultPluginParamValModelPtr model );
                        ~DefaultExtrudePlugin  ();


    virtual void        ProcessConnectedComponent       ( model::ConnectedComponentPtr & currComponent, std::vector< IConnectedComponentPtr > & allComponents ) override;
};


} // model
} // bv


