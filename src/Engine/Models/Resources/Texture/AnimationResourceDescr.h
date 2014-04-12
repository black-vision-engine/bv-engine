#pragma once

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {

    class AnimationResourceDescr : public IAnimationResourceDescr
{
private:

    std::string                 m_name;

    std::vector< std::wstring > m_frames;

public:

                                                AnimationResourceDescr  ( const std::vector< std::wstring > & frames, const std::string & name );
                                                ~AnimationResourceDescr ();

    virtual const std::string &                 GetName                 () const override;
    virtual PluginResourceType                  GetResourceType         () const override;

    virtual const std::vector< std::wstring > & GetFrames               () const override;

    static AnimationResourceDescr *             CreateFromDirFrames     ( const std::wstring & path, const std::wstring & filter, const std::string & name );

};

typedef std::shared_ptr< AnimationResourceDescr > AnimationResourceDescrPtr;

} //model
} //bv
