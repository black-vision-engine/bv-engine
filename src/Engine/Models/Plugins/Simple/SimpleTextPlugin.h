#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"
#include "Engine/Events/BaseEvent.h"

namespace bv { namespace model {

class Resource;
class FontExtraData;
class TextAtlas;
class Text;

// ***************************** UID **********************************
class SimpleTextPluginUID
{
public:
    static const char*       GetName()        { return "simple_text_plugin"; }
};

// ***************************** PLUGIN ********************************** 
class SimpleTextPlugin : public BasePlugin< IPlugin, SimpleTextPluginUID >
{
private:

    VertexAttributesChannel*            m_geomChannel;

    Textures                    m_textures;

    const FontExtraData*        m_fontExtraData;
    TextAtlas const*            m_atlasText;
    std::wstring                m_text;
    bool                        m_bolded;
    bool                        m_italic;

    bool                        m_textSet;

private:


    void                        LoadAtlas( const std::string& name );

    VertexAttributesChannel*    CreateVertexAttributesChannel();
    void                        EvalGeometryChannel( VertexAttributesChannel* geomChannel );

    const Text*                 GetFont() const;

    explicit                    SimpleTextPlugin    ( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bold, bool italic );

public:

    static SimpleTextPlugin*    Create( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bolded = false, bool italic = false );

                                ~SimpleTextPlugin   ();

    virtual const IVertexAttributesChannel *    GetGeometryChannel          () const override;
    virtual Textures                            GetTextures                 () const override;
    void                                        SetText                     ( const std::wstring& newText );

    //delegates
    void                                        OnSetText                   ( IEventPtr evt );

    virtual void                        Update                      ( TimeType t ) override;
    virtual void                        Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv