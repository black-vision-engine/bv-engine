#include "Renderer.h"

namespace bv {

// *********************************
//
void Renderer::SetAlphaState ( const AlphaState * as )
{
    if ( as->blendEnabled )
    {
        //FIXME: RENDER DATA -> RENDER STATE to verify if necessary
        if ( true /* renderState->state to change */)
        {
            mData->mCurrentRS.mAlphaBlendEnabled = true;
            glEnable(GL_BLEND);
        }

        GLenum srcBlend = gOGLAlphaSrcBlend[mAlphaState->SrcBlend];
        GLenum dstBlend = gOGLAlphaDstBlend[mAlphaState->DstBlend];
        if (srcBlend != mData->mCurrentRS.mAlphaSrcBlend
        ||  dstBlend != mData->mCurrentRS.mAlphaDstBlend)
        {
            mData->mCurrentRS.mAlphaSrcBlend = srcBlend;
            mData->mCurrentRS.mAlphaDstBlend = dstBlend;
            glBlendFunc(srcBlend, dstBlend);
        }

        if (mAlphaState->ConstantColor != mData->mCurrentRS.mBlendColor)
        {
            mData->mCurrentRS.mBlendColor = mAlphaState->ConstantColor;
            glBlendColor(
                mData->mCurrentRS.mBlendColor[0],
                mData->mCurrentRS.mBlendColor[1],
                mData->mCurrentRS.mBlendColor[2],
                mData->mCurrentRS.mBlendColor[3]);
        }
    }
    else
    {
        if (mData->mCurrentRS.mAlphaBlendEnabled)
        {
            mData->mCurrentRS.mAlphaBlendEnabled = false;
            glDisable(GL_BLEND);
        }
    }

    if (mAlphaState->CompareEnabled)
    {
        if (!mData->mCurrentRS.mAlphaCompareEnabled)
        {
            mData->mCurrentRS.mAlphaCompareEnabled = true;
            glEnable(GL_ALPHA_TEST);
        }

        GLenum compare = gOGLAlphaCompare[mAlphaState->Compare];
        float reference = mAlphaState->Reference;
        if (compare != mData->mCurrentRS.mCompareFunction
        ||  reference != mData->mCurrentRS.mAlphaReference)
        {
            mData->mCurrentRS.mCompareFunction = compare;
            mData->mCurrentRS.mAlphaReference = reference;
            glAlphaFunc(compare, reference);
        }
    }
    else
    {
        if (mData->mCurrentRS.mAlphaCompareEnabled)
        {
            mData->mCurrentRS.mAlphaCompareEnabled = false;
            glDisable(GL_ALPHA_TEST);
        }
    }
}


} //bv
