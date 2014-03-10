#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"


namespace bv { namespace model {

class DefaultFinalizePlugin : public IPlugin
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
                                                                                     
    virtual IPluginParamValModel *              GetPluginParamValModel      () const override;
    virtual IParameter *                        GetParameter                ( const std::string & name ) const override;
    virtual const bv::IValue *                  GetValue                    ( const std::string & name ) const override;

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;
    virtual const ITransformChannel *           GetTransformChannel         () const override;
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const override;
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const override;
    virtual const IGeometryShaderChannel *      GetGeometryShaderChannel    () const override;
                                                                                     
    virtual TextureInfoVec                      GetTextures                 () const override; // FIXME: use resources
    virtual bool                                HasAnimatingTexture         () const override;
    virtual const ISequenceAnimationSource *    QuerySequenceAnimationSource() const override; // FIXME: use generic interface for querying resources that can be 

    virtual void                                Update                      ( TimeType t ) override;

    void                                        SetPrevPlugin               ( const IPlugin * plugin );
    void                                        SetName                     ( const std::string & name );

};

typedef std::shared_ptr< DefaultFinalizePlugin > DefaultFinalizePluginPtr;

} //model
}  //bv
