#include "stdafx.h"

#include "CompositeInterpolator.inl"

namespace bv
{

// *******************************
//
template<>
CompositeInterpolator< bv::TimeType, std::string >::CompositeInterpolator( float tolerance )
	: m_type( CurveType::CT_POINT )
	, m_tolerance( tolerance )
	, m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{}

// *******************************
//
template<>
CompositeInterpolator< bv::TimeType, std::wstring >::CompositeInterpolator( float tolerance )
	: m_type( CurveType::CT_POINT )
	, m_tolerance( tolerance )
	, m_preMethod( WrapMethod::clamp ), m_postMethod( WrapMethod::clamp )
{}

template class CompositeInterpolator<TimeType, TimeType>;
template class CompositeInterpolator<TimeType, bool>;
template class CompositeInterpolator<TimeType, int>;
template class CompositeInterpolator<TimeType, float>;

template class CompositeInterpolator<TimeType, glm::vec2>;
template class CompositeInterpolator<TimeType, glm::vec3>;
template class CompositeInterpolator<TimeType, glm::vec4>;

template class CompositeInterpolator< TimeType, std::string >;
template class CompositeInterpolator< TimeType, std::wstring >;


} // bv