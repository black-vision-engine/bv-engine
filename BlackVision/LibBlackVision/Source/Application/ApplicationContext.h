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

    UInt32                  m_clientWidth;
    UInt32                  m_clientHeight;

	UInt64					m_updateCounter;

public:
    static ApplicationContext &	Instance			();

	UInt32					GetWidth				() const;
	UInt32					GetHeight				() const;
	UInt64					GetUpdateCounter        () const;

    UInt32                  GetClientHeight         () const;
    UInt32                  GetClientWidth          () const;

	//FIXME: shouldn't be public
	void					SetResolution			( UInt32 width, UInt32 height );
	void					SetUpdateCounter        ( UInt64 counter );
    void                    IncrementUpdateCounter  ();

protected:
								ApplicationContext		();
								~ApplicationContext		();
};

}
