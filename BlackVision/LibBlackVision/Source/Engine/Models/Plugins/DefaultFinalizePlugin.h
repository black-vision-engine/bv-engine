#pragma once

#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Channels/DefaultFinalizePixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultFinalizeVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultFinalizeGeometryShaderChannel.h"

namespace bv { namespace model {

class DefaultFinalizePlugin : public IFinalizePlugin
{
private:

    IPluginPtr                      m_prevPlugin;

    std::string                     m_shadersDir;

    std::string                     m_name;
    static std::string              m_uid;

    DefaultVertexShaderChannelPtr   m_defaultVSChannel;

    mutable DefaultFinalizePixelShaderChannelPtr    m_finalizePSC;
    mutable DefaultFinalizeVertexShaderChannelPtr   m_finalizeVSC;
    mutable DefaultFinalizeGeometryShaderChannelPtr m_finalizeGSC;

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
                 
    virtual RendererContextConstPtr             GetRendererContext          () const override;

    virtual IPluginConstPtr                     GetPrevPlugin               () const override;
	virtual mathematics::RectConstPtr			GetAABB						( const glm::mat4 & currentTransformation ) const override;

    virtual bool                                LoadResource                ( AssetDescConstPtr resDescr ) override;

    virtual void                                Update                      ( TimeType t ) override;

    void                                        SetPrevPlugin               ( IPluginPtr plugin );
    void                                        SetName                     ( const std::string & name );

    static std::string                          ShadersDir                  ();

	virtual ParamTransformVecPtr				GetParamTransform			() const override;

private:

    std::vector< std::string >                  PrevUIDS                    ( unsigned int skipFirstEntries ) const;

};

DEFINE_PTR_TYPE(DefaultFinalizePlugin)

} //model
}  //bv
