#pragma once

#include "CoreDEF.h"

namespace bv
{

class ApplicationContext
{
private:
	UInt32					m_width;
	UInt32					m_height;
	UInt64					m_timestamp;

public:
    static ApplicationContext &	Instance			();

	UInt32					GetWidth				() const;
	UInt32					GetHeight				() const;
	UInt64					GetTimestamp			() const;
	void					SetResolution			( UInt32 width, UInt32 height );
	void					SetTimestamp			( UInt64 timestamp );

protected:
								ApplicationContext		();
								~ApplicationContext		();
};

}
