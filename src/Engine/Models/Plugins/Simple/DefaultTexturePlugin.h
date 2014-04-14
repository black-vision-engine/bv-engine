#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"



namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class DefaultTexturePluginDesc : public BasePluginDescriptor
{
public:

    DefaultTexturePluginDesc                                    ();

    virtual IPlugin *                       CreatePlugin        ( const std::string & name, const IPlugin * prev ) const override;
    virtual DefaultPluginParamValModel *    CreateDefaultModel  () const override;
   
    static  std::string                     UID                 ();

    static  std::string                     VertexShaderSource  ();
    static  std::string                     PixelShaderSource   ();

};

class Resource;
class TexturePixelShaderChannel;
class TextureVertexShaderChannel;


// ***************************** PLUGIN ********************************** 
class DefaultTexturePlugin : public BasePlugin< IPlugin >
{
private:

    DefaultPluginParamValModelPtr   m_paramValModel;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    unsigned int                    m_texCoordChannelIndex;

    ParamFloat *                    m_paramWrapModeX;
    ParamFloat *                    m_paramWrapModeY;
    ParamFloat *                    m_paramFilteringMode;
    ParamFloat *                    m_paramAttachMode;

    TextureWrappingMode             m_lastTextureWrapModeX;
    TextureWrappingMode             m_lastTextureWrapModeY;
    TextureFilteringMode            m_lastTextureFilteringMode;
    TextureAttachmentMode           m_lastTextureAttachMode;

public:

    explicit                                    DefaultTexturePlugin        ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultTexturePlugin       ();

    virtual bool                                LoadResource                ( const IPluginResourceDescr * resDescr ) override;

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;
    virtual const IPixelShaderChannel *         GetPixelShaderChannel       () const override;
    virtual const IVertexShaderChannel *        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

private:

    void                                        InitAttributesChannel       ( const IPlugin * prev );

    TextureWrappingMode                         GetWrapModeX                ( TimeType t ) const;
    TextureWrappingMode                         GetWrapModeY                ( TimeType t ) const;
    TextureFilteringMode                        GetFilteringMode            ( TimeType t ) const;
    TextureAttachmentMode                       GetAttachementMode          ( TimeType t ) const;

    bool                                        StateChanged                ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am ) const;
    bool                                        UpdateState                 ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am );

};

} // model
} // bv
