#pragma once

#include "CoreDEF.h"

namespace bv
{

class ApplicationContext
{
private:
	static const UInt32		HD_WIDTH;
	static const UInt32		HD_HEIGHT;

private:
	UInt32					m_width; //framebuffer
	UInt32					m_height; //framebuffer

	UInt64					m_timestamp;

public:
    static ApplicationContext &	Instance			();

	UInt32					GetWidth				() const;
	UInt32					GetHeight				() const;
	UInt64					GetTimestamp			() const;

	//FIXME: shouldn't be public
	void					SetResolution			( UInt32 width, UInt32 height );
	void					SetTimestamp			( UInt64 timestamp );

protected:
								ApplicationContext		();
								~ApplicationContext		();
};

}
