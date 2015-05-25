#pragma once

#include "Prototypes/DrawingCommands/Common/DrawingMode.h"
#include "Prototypes/DrawingCommands/Common/DrawingTestScene.h"

namespace bv {

class DrawingModeArrays : public DrawingMode
{
public:
    virtual void				Render		( const DrawingModeData& data );
	virtual const char *		GetName		()  { return "glDrawArrays"; }
};

inline	void					DrawingModeArrays::Render		( const DrawingModeData& data )
{
	BVGL::bvglDrawArrays( data.mode, data.first, data.count );
}

// *****************************
//
class DrawingModeElements : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glDrawElements"; }
};

inline	void		DrawingModeElements::Render							( const DrawingModeData& data ){
	BVGL::bvglDrawElements( data.mode, data.count, data.type, 0 );
}

// *****************************
//
class DrawingModeArraysInstanced : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glDrawArraysInstanced"; }
};

inline	void		DrawingModeArraysInstanced::Render					( const DrawingModeData& data ){
	BVGL::bvglDrawArraysInstanced( data.mode, data.first, data.count, data.primcount );
}


// *****************************
//
class DrawingModeElementsInstanced : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glDrawElementsInstanced"; }
};

inline	void		DrawingModeElementsInstanced::Render				( const DrawingModeData& data ){
	BVGL::bvglDrawElementsInstanced( data.mode, data.count, data.type, 0, data.primcount );
}

// *****************************
//
class DrawingModeElementsBaseVertex : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glDrawElementsBaseVertex"; }
};

inline	void		DrawingModeElementsBaseVertex::Render					( const DrawingModeData& data ){
	BVGL::bvglDrawElementsBaseVertex( data.mode, data.count, data.type, 0, data.basevertex );
}

// *****************************
//
class DrawingModeArraysInstancedBaseInstance : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glDrawArraysInstancedBaseInstance"; }
};

inline	void		DrawingModeArraysInstancedBaseInstance::Render		( const DrawingModeData& data ){
	BVGL::bvglDrawArraysInstancedBaseInstance( data.mode, data.first, data.count, data.primcount, data.baseinstance );
}

// *****************************
//
class DrawingModeElementsInstancedBaseVertexBaseInstance : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glDrawElementsInstancedBaseVertexBaseInstance"; }
};

inline	void		DrawingModeElementsInstancedBaseVertexBaseInstance::Render	( const DrawingModeData& data ){
	BVGL::bvglDrawElementsInstancedBaseVertexBaseInstance( data.mode, data.count, data.type, 0, data.primcount, data.basevertex, data.baseinstance );
}

// *****************************
//
class DrawingModeElementsInstancedBaseVertex : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glDrawElementsInstancedBaseVertex"; }
};

inline	void		DrawingModeElementsInstancedBaseVertex::Render		( const DrawingModeData& data ){
	BVGL::bvglDrawElementsInstancedBaseVertex( data.mode, data.count, data.type, 0, data.primcount, data.basevertex );
}

// *****************************
//
class DrawingModeArraysIndirect : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glDrawArraysIndirect"; }
};

inline	void		DrawingModeArraysIndirect::Render					( const DrawingModeData& data ){
	data.scene->BindIndirectArrays();
	BVGL::bvglDrawArraysIndirect( data.mode, 0 );
}

// *****************************
//
class DrawingModeElementsIndirect : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glDrawElementsIndirect"; }
};

inline	void		DrawingModeElementsIndirect::Render					( const DrawingModeData& data ){
	data.scene->BindIndirectElements();
	BVGL::bvglDrawElementsIndirect( data.mode, data.type, 0 );
}

// *****************************
//
class DrawingModeMultiArraysIndirect : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glMultiDrawArraysIndirect"; }
};

inline	void		DrawingModeMultiArraysIndirect::Render				( const DrawingModeData& data ){
	data.scene->BindIndirectArrays();
	BVGL::bvglMultiDrawArraysIndirect( data.mode, 0, data.primcount, data.stride );
}

// *****************************
//
class DrawingModeMultiElementsIndirect : public DrawingMode
{
public:
    virtual void		Render		( const DrawingModeData& data );
	const char *		GetName		()  { return "glMultiDrawElementsIndirect"; }
};

inline	void		DrawingModeMultiElementsIndirect::Render			( const DrawingModeData& data ){
	data.scene->BindIndirectElements();
	BVGL::bvglMultiDrawElementsIndirect( data.mode, data.type, 0, data.primcount, data.stride );
}

} //bv
