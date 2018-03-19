#pragma once

#include "CoreDEF.h"

#include <atomic>


namespace bv
{


typedef UInt64 UniqueID;


// ***********************
//
class IDGenerator
{
private:

	std::atomic< UInt64 >       m_id;

public:

	static IDGenerator &	Instance			();
    UniqueID                GetID				();

private:

							IDGenerator ();

};

}
