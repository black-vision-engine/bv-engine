#include "IDGenerator.h"

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
	static auto instance = IDGenerator();
	return instance;
}

// ***********************
//
UInt32					IDGenerator::GetID		()
{
	return m_id++;
}

}