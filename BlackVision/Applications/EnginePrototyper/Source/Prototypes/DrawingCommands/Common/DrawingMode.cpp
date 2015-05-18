#include "DrawingMode.h"

#include "Prototypes/DrawingCommands/Common/Modes/DrawingModes.h"

namespace  bv {

// **************************
//
	DrawArraysIndirectCommand::DrawArraysIndirectCommand()
		: count ( 0 )
		, primCount ( 1 )
		, first ( 0 )
		, baseInstance ( 0 )
{
}

// **************************
//
	DrawElementsIndirectCommand::DrawElementsIndirectCommand()
		: count ( 0 )
		, primCount ( 1 )
		, first ( 0 )
		, baseVertex ( 0 )
		, baseInstance ( 0 )
{
}

// **************************
//
	DrawingModeData::DrawingModeData		()
		: mode( GL_TRIANGLES )
		, count( 0 )
		, primcount( 1 )
		, baseinstance( 0 )
		, type( GL_UNSIGNED_SHORT )
		, basevertex( 0 )
		, first( 0 )
		, stride( 0 )
		, scene ( nullptr )
{
}

// **************************
//
			DrawingMode::DrawingMode			()
{
}

// **************************
//
			DrawingMode::~DrawingMode			()
{
}

// *************************
//

std::map<unsigned char, DrawingMode*(*)()>	DrawingMode::m_modes;

// *************************
//
template<typename T> 
DrawingMode *	DrawingMode::CreateInstance()
{
	return new T;
}

// *************************
//
DrawingMode *			DrawingMode::GetMode				( unsigned char name )
{
	return m_modes[ name ]();
}

// *************************
//
void						DrawingMode::Initialize				()
{
	m_modes['0'] = &DrawingMode::CreateInstance< DrawingModeArrays >;
	m_modes['1'] = &DrawingMode::CreateInstance< DrawingModeElements >;

	m_modes['2'] = &DrawingMode::CreateInstance< DrawingModeElementsBaseVertex >;

	m_modes['3'] = &DrawingMode::CreateInstance< DrawingModeArraysInstanced >;
	m_modes['4'] = &DrawingMode::CreateInstance< DrawingModeElementsInstanced >;

	m_modes['5'] = &DrawingMode::CreateInstance< DrawingModeElementsInstancedBaseVertex >;

	m_modes['6'] = &DrawingMode::CreateInstance< DrawingModeArraysInstancedBaseInstance >;
	m_modes['7'] = &DrawingMode::CreateInstance< DrawingModeElementsInstancedBaseVertexBaseInstance >;

	m_modes['8'] = &DrawingMode::CreateInstance< DrawingModeArraysIndirect >;
	m_modes['9'] = &DrawingMode::CreateInstance< DrawingModeElementsIndirect >;
	m_modes['-'] = &DrawingMode::CreateInstance< DrawingModeMultiArraysIndirect >;
	m_modes['='] = &DrawingMode::CreateInstance< DrawingModeMultiElementsIndirect >;

	//add more modes with corresponding key...
}


} //bv
