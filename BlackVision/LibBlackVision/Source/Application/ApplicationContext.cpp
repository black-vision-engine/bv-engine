#include "ApplicationContext.h"

namespace bv {

const UInt32	ApplicationContext::HD_WIDTH = 1920;
const UInt32	ApplicationContext::HD_HEIGHT = 1080;

// ***********************
//
ApplicationContext::ApplicationContext						()
    : m_width( HD_WIDTH )
    , m_height( HD_HEIGHT )
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