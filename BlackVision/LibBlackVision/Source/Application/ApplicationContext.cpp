#include "stdafx.h"

#include "ApplicationContext.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

const UInt32	ApplicationContext::HD_WIDTH = 1920;
const UInt32	ApplicationContext::HD_HEIGHT = 1080;

// ***********************
//
ApplicationContext::ApplicationContext						()
    : m_width( HD_WIDTH )
    , m_height( HD_HEIGHT )
    , m_updateCounter( 0 )
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
UInt64					ApplicationContext::GetUpdateCounter	() const
{
	return m_updateCounter;
}

// ***********************
//
UInt32                  ApplicationContext::GetClientHeight() const
{
    return m_clientHeight;
}

// ***********************
//
UInt32                  ApplicationContext::GetClientWidth() const
{
    return m_clientWidth;
}

// ***********************
//
void					ApplicationContext::SetResolution	( UInt32 width, UInt32 height )
{
	m_clientWidth = width;
	m_clientHeight = height;
}

// ***********************
//
void					ApplicationContext::SetUpdateCounter	( UInt64 counter )
{
	m_updateCounter = counter;
}

// ***********************
//
void                    ApplicationContext::IncrementUpdateCounter  ()
{
    m_updateCounter++;
}

}