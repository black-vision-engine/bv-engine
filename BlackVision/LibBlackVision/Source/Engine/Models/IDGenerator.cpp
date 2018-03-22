#include "stdafx.h"

#include "IDGenerator.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// ***********************
//
IDGenerator::IDGenerator						()
    : m_id( 0 )
{
}

// ***********************
//
IDGenerator &			IDGenerator::Instance	()
{
	static IDGenerator instance;
	return instance;
}

// ***********************
//
UniqueID                IDGenerator::GetID		()
{
	return ++m_id;
}

}