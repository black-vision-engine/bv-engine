#pragma once

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {

    class AnimationResourceDescr : public IAnimationResourceDescr
{
private:

    std::vector< std::string >  m_frames;

public:

                                                AnimationResourceDescr  ( const std::vector< std::string > & frames );
                                                ~AnimationResourceDescr ();

    virtual PluginResourceType                  GetResourceType         () const override;

    virtual const std::vector< std::string > &  GetFrames               () const override;

    static AnimationResourceDescr *             CreateFromDirFrames     ( const std::string & path, const std::string & filter );

};

typedef std::shared_ptr< AnimationResourceDescr > AnimationResourceDescrPtr;

} //model
} //bv
