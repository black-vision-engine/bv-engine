#pragma once

#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"


namespace bv { namespace model {

class DefaultFinalizePlugin : public IFinalizePlugin
{
private:

    IPluginConstPtr                 m_prevPlugin;
    
    std::string                     m_name;
    static std::string              m_uid;

    DefaultVertexShaderChannelPtr   m_defaultVSChannel;

public:

                                                DefaultFinalizePlugin       ();
                                                ~DefaultFinalizePlugin      ();

    virtual const std::string &                 GetName                     () const override;
    virtual const std::string &                 GetTypeUid                  () const override;
                                                                                     
    virtual IPluginParamValModelPtr             GetPluginParamValModel      () const override;
    virtual IParameterPtr                       GetParameter                ( const std::string & name ) const override;
    virtual bv::IValueConstPtr                  GetValue                    ( const std::string & name ) const override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual ITransformChannelConstPtr           GetTransformChannel         () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;
    virtual IGeometryShaderChannelConstPtr      GetGeometryShaderChannel    () const override;
                 
    virtual const RendererContext *             GetRendererContext          () const override;

    virtual bool                                LoadResource                ( IPluginResourceDescrConstPtr resDescr ) override;

    virtual void                                Update                      ( TimeType t ) override;

    void                                        SetPrevPlugin               ( IPluginConstPtr plugin );
    void                                        SetName                     ( const std::string & name );

};

DEFINE_PTR_TYPE(DefaultFinalizePlugin)

} //model
}  //bv
