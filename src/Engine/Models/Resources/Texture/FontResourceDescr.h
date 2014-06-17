#pragma once

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {

class FontResourceDescr : public IFontResourceDescr
{
private:

    std::string m_fontFile;

public:

                                        FontResourceDescr       ( const std::string & fontFile );
                                        ~FontResourceDescr      ();

    virtual PluginResourceType          GetResourceType         () const override;

    virtual const std::string &         GetFontFile             () const override;

};

DEFINE_PTR_TYPE(FontResourceDescr)

} //model
} //bv
