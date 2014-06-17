#pragma once

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {

class TextureResourceDescr : public ITextureResourceDescr
{
private:

    std::string m_textureFile;

public:

                                        TextureResourceDescr    ( const std::string & textureFile );
                                        ~TextureResourceDescr   ();

    virtual PluginResourceType          GetResourceType         () const override;

    virtual const std::string &         GetTextureFile          () const override;

};

DEFINE_PTR_TYPE(TextureResourceDescr)

} //model
} //bv
