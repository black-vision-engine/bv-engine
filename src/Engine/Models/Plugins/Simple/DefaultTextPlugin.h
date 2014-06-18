#pragma once

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Events/BaseEvent.h"


namespace bv { namespace model {

class TextAtlas;

// ***************************** DESCRIPTOR **********************************
class DefaultTextPluginDesc : public BasePluginDescriptor
{
public:

    DefaultTextPluginDesc                                       ();

    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginConstPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
   
    virtual bool                            CanBeAttachedTo     ( IPluginConstPtr plugin )  const override;

    static  std::string                     UID                 ();

    static  std::string                     VertexShaderSource  ();
    static  std::string                     PixelShaderSource   ();

    static  std::string                     TextureName         ();
    static  std::string                     FontFileName        ();
};


// ***************************** PLUGIN ********************************** 
class DefaultTextPlugin : public BasePlugin< IPlugin >
{
private:

    DefaultPluginParamValModelPtr   m_paramValModel;

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    DefaultTexturesDataPtr          m_texturesData;

    unsigned int                    m_texCoordChannelIndex;

    std::wstring                    m_text;
    const TextAtlas*                m_textAtlas;
    bool                            m_textSet;

    ParamFloatPtr                   m_fontSizeParam;
    ParamFloatPtr                   m_blurSizeParam;

public:

    //delegates
    void                                        OnSetText                   ( IEventPtr evt );

    void                                        SetText                     ( const std::wstring & newText );

    explicit                                    DefaultTextPlugin           ( const std::string & name, const std::string & uid, IPluginConstPtr prev, DefaultPluginParamValModelPtr model );
                                                ~DefaultTextPlugin          ();

    virtual bool                                LoadResource                ( IPluginResourceDescrConstPtr resDescr ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelConstPtr         GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                Update                      ( TimeType t ) override;

private:

    void                                        InitAttributesChannel       ( IPluginConstPtr prev );
};

bool            SetTextPluginContent( IPluginPtr textPlugin, const std::wstring& text );

} // model
} // bv













//#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
//
//#include "Engine/Models/Plugins/Plugin.h"
//#include "Engine/Models/Resources/IResource.h"
//#include "Engine/Events/BaseEvent.h"
//
//
//namespace bv { namespace model {
//
//class Resource;
//class FontExtraData;
//class TextAtlas;
//class Text;
//
//// ***************************** UID **********************************
//class SimpleTextPluginUID
//{
//public:
//    static const char*       GetName()        { return "simple_text_plugin"; }
//};
//
//// ***************************** PLUGIN ********************************** 
//class SimpleTextPlugin : public BasePlugin< IPlugin >
//{
//private:
//
//    VertexAttributesChannelPtr  m_vertexAttributeChannel;
//
//    TextureInfoVec              m_textures;
//
//    const ResourceHandle*       m_fontResource;
//    const TextAtlas*            m_textAtlas;
//    std::wstring                m_text;
//
//    bool                        m_bolded;
//    bool                        m_italic;
//
//    bool                        m_textSet;
//
//private:
//
//    explicit                    SimpleTextPlugin    ( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bold, bool italic );
//
//public:
//
//    static SimpleTextPlugin*    Create( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bolded = false, bool italic = false );
//
//                                ~SimpleTextPlugin   ();
//
//    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
//    virtual TextureInfoVec                      GetTextures                 () const;
//    void                                        SetText                     ( const std::wstring & newText );
//
//    //delegates
//    void                                        OnSetText                   ( IEventPtr evt );
//
//    virtual void                        Update                      ( TimeType t ) override;
//    virtual void                        Print                       ( std::ostream & out, int tabs = 0 ) const;
//
//};
//
//} // model
//} // bv