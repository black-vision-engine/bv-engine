#pragma once

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {

class AnimationResourceDescr;
DEFINE_PTR_TYPE(AnimationResourceDescr)
DEFINE_CONST_PTR_TYPE(AnimationResourceDescr)

class AnimationResourceDescr : public IAnimationResourceDescr
{
private:

    std::vector< std::string >  m_frames;

public:

                                                AnimationResourceDescr  ( const std::vector< std::string > & frames );
                                                ~AnimationResourceDescr ();

    virtual PluginResourceType                  GetResourceType         () const override;

    virtual const std::vector< std::string > &  GetFrames               () const override;

    static AnimationResourceDescrPtr            CreateFromDirFrames     ( const std::string & path, const std::string & filter );

};



} //model
} //bv
