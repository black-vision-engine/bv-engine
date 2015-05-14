#	pragma once

#include "Prototypes/DrawingCommands/Common/DrawingMode.h"

namespace bv {

class DrawingModeArrays : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeArrays::Render							( const DrawingModeData& data ){
	BVGLExt::bvglDrawArrays( data.mode, data.first, data.count );
}

class DrawingModeElements : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeElements::Render							( const DrawingModeData& data ){
	BVGLExt::bvglDrawElements( data.mode, data.count, data.type, data.indices );
}

// *****************************
//
class DrawingModeElementsBaseVertex : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeElementsBaseVertex::Render					( const DrawingModeData& data ){
	BVGLExt::bvglDrawElementsBaseVertex( data.mode, data.count, data.type, const_cast<void *>( data.indices ), data.basevertex );
}

// *****************************
//
class DrawingModeArraysInstanced : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeArraysInstanced::Render					( const DrawingModeData& data ){
	BVGLExt::bvglDrawArraysInstanced( data.mode, data.first, data.count, data.primcount );
}

// *****************************
//
class DrawingModeElementsInstanced : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeElementsInstanced::Render				( const DrawingModeData& data ){
	BVGLExt::bvglDrawElementsInstanced( data.mode, data.count, data.type, data.indices, data.primcount );
}

// *****************************
//
class DrawingModeArraysInstancedBaseInstance : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeArraysInstancedBaseInstance::Render		( const DrawingModeData& data ){
	BVGLExt::bvglDrawArraysInstancedBaseInstance( data.mode, data.first, data.count, data.primcount, data.baseinstance );
}

// *****************************
//
class DrawingModeElementsInstancedBaseVertexBaseInstance : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeElementsInstancedBaseVertexBaseInstance::Render	( const DrawingModeData& data ){
	//const_cast?
	BVGLExt::bvglDrawElementsInstancedBaseVertexBaseInstance( data.mode, data.count, data.type, const_cast<void *>( data.indices ), data.primcount, data.basevertex, data.baseinstance );
}

// *****************************
//
class DrawingModeElementsInstancedBaseVertex : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeElementsInstancedBaseVertex::Render		( const DrawingModeData& data ){
	//const_cast?
	BVGLExt::bvglDrawElementsInstancedBaseVertex( data.mode, data.count, data.type, const_cast<void *>( data.indices ), data.primcount, data.basevertex );
}

// *****************************
//
class DrawingModeArraysIndirect : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeArraysIndirect::Render					( const DrawingModeData& data ){
	BVGLExt::bvglDrawArraysIndirect( data.mode, &data.indirectArrays );
}

// *****************************
//
class DrawingModeElementsIndirect : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeElementsIndirect::Render					( const DrawingModeData& data ){
	BVGLExt::bvglDrawElementsIndirect( data.mode, data.type, &data.indirectElements );
}

// *****************************
//
class DrawingModeMultiArraysIndirect : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeMultiArraysIndirect::Render				( const DrawingModeData& data ){
	BVGLExt::bvglMultiDrawArraysIndirect( data.mode, &data.indirectArrays, data.drawcount, data.stride );
}

// *****************************
//
class DrawingModeMultiElementsIndirect : public DrawingMode
{
public:
    virtual void                Render			( const DrawingModeData& data );
};

inline	void		DrawingModeMultiElementsIndirect::Render			( const DrawingModeData& data ){
	BVGLExt::bvglMultiDrawElementsIndirect( data.mode, data.type, &data.indirectElements, data.drawcount, data.stride );
}

} //bv
