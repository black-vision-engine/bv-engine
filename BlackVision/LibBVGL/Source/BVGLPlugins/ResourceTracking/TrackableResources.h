#pragma once

#include <hash_map>

// FIXME: remove later. Some temporary defines for the time of transition - to make sure that there is no explicit glew.h dependency
#include "BVGLDefs.h"


namespace bv {

template< typename ResourceDesc >
class TrackableResources
{
private:

    std::hash_map< GLuint, ResourceDesc >       m_allocatedResources;
    std::hash_map< GLenum, GLuint >             m_boundResources;

public:

    void            GenResources        ( GLsizei n, GLuint * resources );
    void            CreateResources     ( GLsizei n, GLuint * resources );

    void            BindResource        ( GLenum target, GLuint resource );

    void            DeleteResources     ( GLsizei n, const GLuint * resources );

    ResourceDesc &  GetBoundResource    ( GLenum target );

    void            PrintShortSummary   () const;
    void            PrintLongSummary    () const;
};

} //bv

