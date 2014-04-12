#pragma once

#include <vector>

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {

class AnimationResourceDescr : public IPluginResourceDescr
{
private:

    std::string                 m_name;

    std::vector< std::wstring > m_frames;

public:

                                        AnimationResourceDescr  ( const std::vector< std::wstring > & frames, const std::string & name );
                                        ~AnimationResourceDescr ();

    virtual const std::string &         GetName                 () const override;
    virtual PluginResourceType          GetResourceType         () const override;

    const std::vector< std::wstring > & GetFrames               () const;

    static AnimationResourceDescr *     CreateFromDirFrames     ( const std::wstring & path, const std::wstring & filter, const std::string & name );

};

typedef std::shared_ptr< AnimationResourceDescr > AnimationResourceDescrPtr;

} //model
} //bv
