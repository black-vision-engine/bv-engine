#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"


namespace bv { namespace model {

class Resource;
class FontExtraData;
class TextAtlas;
class Text;

// ***************************** DESCRIPTOR **********************************
class SimpleTextPluginPD : public BaseParametersDescriptor
{
public:

    static const std::string pluginName;

    explicit SimpleTextPluginPD()
        : BaseParametersDescriptor( "SimpleTextPlugin" )
    {}
};


// ***************************** PLUGIN ********************************** 
class SimpleTextPlugin : public BasePlugin< IPlugin, SimpleTextPluginPD >
{
private:

    const FontExtraData*        m_fontExtraData;
    TextAtlas const*            m_atlasText;
    std::wstring                m_text;
    bool                        m_bolded;
    bool                        m_italic;


    TextureInfo*                LoadAtlas( const std::string& name );

    void                        EvalGeometryChannel();

    const Text*                 GetFont() const;

    explicit                    SimpleTextPlugin    ( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bold = false, bool italic = false );

public:

    static SimpleTextPlugin*    Create( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize );

                                ~SimpleTextPlugin   ();


    void                        Update                      ( float t ) override;
    void                        Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv