#pragma once

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
//#include "Engine/Models/Plugins/Plugin.h"
#include "DefaultGeometryProcessorBase.h"

namespace bv {

namespace model {

// ***********************
//
class TriangulatePluginDesc : public BasePluginDescriptor
{
public:
                                            TriangulatePluginDesc       ();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel          ( ITimeEvaluatorPtr timeEvaluator ) const;
    virtual IPluginPtr                      CreatePlugin                ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const;

};

// ***********************
//
class TriangulatePlugin : public DefaultGeometryProcessorBase
{

protected:
    virtual void                            ProcessConnectedComponent   (   model::ConnectedComponentPtr & currComponent,
                                                                                    std::vector< IConnectedComponentPtr > & allComponents,
                                                                                    PrimitiveType topology );


public:
    explicit							    TriangulatePlugin	        ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );


};

} // model

} // bv
