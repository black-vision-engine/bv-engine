#include "DefaultMultipassEffect.h"


namespace bv
{

DefaultMultipassEffect::DefaultMultipassEffect( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds )
	:	DefaultEffect( psds, vsds, gsds )
{
}


DefaultMultipassEffect::~DefaultMultipassEffect(void)
{
}


RenderablePass* DefaultMultipassEffect::addPass( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds )
{
	RenderablePass* pass = CreateRenderablePass( psds, vsds, gsds );

    AddPass( pass );
	return pass;
}

} //bv
