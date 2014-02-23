#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"


namespace bv { namespace model {

class DefaultFinalizePlugin : public IPlugin
{
private:

    const IPlugin *     m_prevPlugin;
    
    std::string         m_name;
    static std::string  m_uid;

public:

                                                DefaultFinalizePlugin       ();
                                                ~DefaultFinalizePlugin      ();

    virtual const std::string &                 GetName                     () const override;
    virtual const std::string &                 GetTypeUid                  () const override;
                                                                                     
    virtual IPluginParamValModel *              GetPluginParamValModel      () override;
                                                                                     
    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;
    virtual const ITransformChannel *           GetTransformChannel         () const override;
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const override;
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const override;
    virtual const IGeometryShaderChannel *      GetGeometryShaderChannel    () const override;
                                                                                     
    virtual Textures                            GetTextures                 () const override; // FIXME: use resources
    virtual bool                                HasAnimatingTexture         () const override;
    virtual ISequenceAnimationSource *          QuerySequenceAnimationSource() override; // FIXME: use generic interface for querying resources that can be 

    virtual void                                Update                      ( TimeType t ) override;

    void                                        RegisterPrevPlugin          ( const IPlugin * plugin );
    void                                        SetName                     ( const std::string & name );

};

typedef std::shared_ptr< DefaultFinalizePlugin > DefaultFinalizePluginPtr;

} //model
}  //bv
