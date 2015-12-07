#pragma once

#include "CoreDEF.h"

namespace bv
{

class IDGenerator
{
private:

	UInt32					m_id;

public:

	static IDGenerator &	Instance			();
    UInt32					GetID				();

private:

							IDGenerator ();

};

}
