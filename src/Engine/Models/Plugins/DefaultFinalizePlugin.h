#pragma once

#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"


namespace bv { namespace model {

class DefaultFinalizePlugin : public IFinalizePlugin
{
private:

    const IPlugin *                 m_prevPlugin;
    
    std::string                     m_name;
    static std::string              m_uid;

    DefaultVertexShaderChannel *    m_defaultVSChannel;

public:

                                                DefaultFinalizePlugin       ();
                                                ~DefaultFinalizePlugin      ();

    virtual const std::string &                 GetName                     () const override;
    virtual const std::string &                 GetTypeUid                  () const override;
                                                                                     
    virtual IPluginParamValModelPtr             GetPluginParamValModel      () const override;
    virtual IParameterPtr                       GetParameter                ( const std::string & name ) const override;
    virtual bv::IValueConstPtr                  GetValue                    ( const std::string & name ) const override;

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;
    virtual const ITransformChannel *           GetTransformChannel         () const override;
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const override;
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const override;
    virtual const IGeometryShaderChannel *      GetGeometryShaderChannel    () const override;
                 
    virtual const RendererContext *             GetRendererContext          () const override;

    virtual bool                                LoadResource                ( const IPluginResourceDescr * resDescr ) override;

    virtual void                                Update                      ( TimeType t ) override;

    void                                        SetPrevPlugin               ( const IPlugin * plugin );
    void                                        SetName                     ( const std::string & name );

};

DEFINE_PTR_TYPE(DefaultFinalizePlugin)

} //model
}  //bv
