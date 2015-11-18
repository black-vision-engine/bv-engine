#include "ApplicationContext.h"

namespace bv {

// ***********************
//
ApplicationContext::ApplicationContext						()
    : m_width( 0 )
    , m_height( 0 )
    , m_timestamp( 0 )
{
}

// ***********************
//
ApplicationContext::~ApplicationContext						()
{
}

// ***********************
//
ApplicationContext &	ApplicationContext::Instance		()
{
	static auto instance = ApplicationContext();
	return instance;
}

// ***********************
//
UInt32					ApplicationContext::GetWidth		() const
{
	return m_width;
}

// ***********************
//
UInt32					ApplicationContext::GetHeight		() const
{
	return m_height;
}

// ***********************
//
UInt64					ApplicationContext::GetTimestamp	() const
{
	return m_timestamp;
}

// ***********************
//
void					ApplicationContext::SetResolution	( UInt32 width, UInt32 height )
{
	m_width = width;
	m_height = height;
}

// ***********************
//
void					ApplicationContext::SetTimestamp	( UInt64 timestamp )
{
	m_timestamp = timestamp;
}

}