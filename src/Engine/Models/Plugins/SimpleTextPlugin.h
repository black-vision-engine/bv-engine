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

    GeometryChannel*            m_geomChannel;

    const FontExtraData*        m_fontExtraData;
    TextAtlas const*            m_atlasText;
    std::wstring                m_text;
    bool                        m_bolded;
    bool                        m_italic;


    void                        LoadAtlas( const std::string& name );

    void                        EvalGeometryChannel();

    const Text*                 GetFont() const;

    explicit                    SimpleTextPlugin    ( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bold, bool italic );

public:

    static SimpleTextPlugin*    Create( const std::wstring& text, const std::string & fontFileName, unsigned int fontSize, bool bolded = false, bool italic = false );

                                ~SimpleTextPlugin   ();

    virtual const IGeometryChannel *    GetGeometryChannel          () const override;

    virtual void                        Update                      ( TimeType t ) override;
    virtual void                        Print                       ( std::ostream & out, int tabs = 0 ) const override;

};

} // model
} // bv