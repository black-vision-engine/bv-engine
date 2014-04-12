#pragma once

#include <vector>

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {

class AnimationResourceDescr : public IPluginResourceDescr
{
private:

    std::string                 m_name;

    std::vector< std::string >  m_frames;

public:

                                        AnimationResourceDescr  ( const std::vector< std::string > & frames, const std::string & name );
                                        ~AnimationResourceDescr ();

    virtual const std::string &         GetName                 () const;
    virtual PluginResourceType          GetResourceType         () const;

    const std::vector< std::string > &  GetFrames               () const;

    static AnimationResourceDescr *     CreateFromDirFrames     ( const std::string & path, const std::string & filter );

};

typedef std::shared_ptr< AnimationResourceDescr > AnimationResourceDescrPtr;

} //model
} //bv
