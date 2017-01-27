#include "stdafx.h"

#define GLM_FORCE_SSE2

#include "Transform.h"

#include "glm/gtx/simd_mat4.hpp"


#include "Memory/MemoryLeaks.h"



namespace bv {

const Transform Transform::IDENTITY;

// *********************************
//
Transform::Transform       ()
    : m_fwdTransform( 1.0f )
{
}

// *********************************
//
Transform::Transform       ( const glm::mat4 & fwd )
    : m_fwdTransform( fwd )
{
}

// *********************************
//
Transform::Transform       ( glm::mat4 && fwd )
	: m_fwdTransform( std::move( fwd ) )
{}
// *********************************
//
Transform::~Transform      ()
{
}

// *********************************
//
void                Transform::Reset           ()
{
    m_fwdTransform = glm::mat4( 1.0f );
}

// *********************************
//
void                Transform::SetMatrix       ( const glm::mat4 & m )
{
    m_fwdTransform = m;
}

// *********************************
//
const glm::mat4 &   Transform::Matrix          () const
{
    return m_fwdTransform;
}

// *********************************
//
glm::vec4           Transform::operator *      ( const glm::vec4 & v ) const
{
    return m_fwdTransform * v;
}

// *********************************
//
Transform           Transform::operator *      ( const Transform & t ) const
{
    return Transform( m_fwdTransform * t.m_fwdTransform );   
}

} //bv
