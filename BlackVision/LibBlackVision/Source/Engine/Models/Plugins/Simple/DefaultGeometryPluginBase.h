#pragma once

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultGeometryPluginDescBase : public BasePluginDescriptor
{
public:

                                            DefaultGeometryPluginDescBase   ( const std::string & uid, const std::string & name );

    //virtual IPluginPtr            CreatePlugin                    ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
   
    static  std::string                     UID                             ();

};

// ***************************** PLUGIN ********************************** 
class DefaultGeometryPluginBase : public BasePlugin
{
protected:

    VertexAttributesChannelPtr                  m_vaChannel;
    
    DefaultPixelShaderChannelPtr                m_pixelShaderChannel; //FIXME: quick fix #112133381 - added only to disable cullCtx

    virtual std::vector<IGeometryGeneratorPtr>  GetGenerators() = 0;

protected:

    void                                        InitGeometry                ( PrimitiveType primitiveType = PrimitiveType::PT_TRIANGLE_STRIP );
    virtual bool                                NeedsTopologyUpdate         () = 0;

public:

    DefaultGeometryPluginBase( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
    //~DefaultGeometryPluginBase(void);

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;

    virtual void                                Update                      ( TimeType t );

    virtual bool                                SetPrevPlugin               ( IPluginPtr plugin ) override;

};

} // model
} // bv