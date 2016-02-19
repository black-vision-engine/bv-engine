#include "stdafx.h"

#include "Interpolators.h"

namespace bv
{

// *******************************
//
FloatInterpolator                   InterpolatorsHelper::CreateConstValue                    ( float val )
{
	FloatInterpolator inter; inter.SetWrapPostMethod( bv::WrapMethod::pingPong );
	inter.AddKey( 0.f, val );

	return inter;
}

// *******************************
//
Vec4Interpolator                    InterpolatorsHelper::CreateConstValue                    ( const glm::vec4& val )
{
	Vec4Interpolator inter; inter.SetWrapPostMethod( bv::WrapMethod::pingPong );
	inter.AddKey( 0.f, val );

	return inter;
}

// *******************************
//
Vec3Interpolator                    InterpolatorsHelper::CreateConstValue                    ( const glm::vec3 & val )
{
    Vec3Interpolator inter; inter.SetWrapPostMethod( bv::WrapMethod::pingPong );
	inter.AddKey( 0.f, val );

	return inter;
}

} // bv