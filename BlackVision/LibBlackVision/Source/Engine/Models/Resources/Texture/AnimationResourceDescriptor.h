#pragma once

#include "Engine/Models/Resources/ResourceDescriptor.h"

namespace bv { namespace model {

class AnimationResourceDesc;
DEFINE_PTR_TYPE(AnimationResourceDesc)
DEFINE_CONST_PTR_TYPE(AnimationResourceDesc)

class AnimationResourceDesc : public ResourceDesc, public std::enable_shared_from_this< AnimationResourceDesc >
{
private:

	static const std::string		uid;

protected:

	virtual VoidConstPtr            QueryThis	() const override;

public:

	virtual const std::string &		GetUID		() const override;

	virtual bool					IsCacheable	() const override;

	static const std::string &		UID			();

public:

                                                AnimationResourceDesc	( const std::vector< std::string > & frames );
                                                ~AnimationResourceDesc	();

    virtual const std::vector< std::string > &  GetFrames               () const;

    static AnimationResourceDescConstPtr		CreateFromDirFrames     ( const std::string & path, const std::string & filter );

private:

    std::vector< std::string >  m_frames;

};



} //model
} //bv
