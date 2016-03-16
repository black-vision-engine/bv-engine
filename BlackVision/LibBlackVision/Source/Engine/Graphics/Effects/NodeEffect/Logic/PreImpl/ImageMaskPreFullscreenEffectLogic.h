#pragma once

#include "Engine/Graphics/Effects/NodeEffect/Logic/PreFullScreenEffectLogic.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv { 

class ImageMaskPreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
private:
    ITextureDescriptorConstPtr      m_txDesc;

    ValueIntPtr                     m_fitVal;
    ValueIntPtr                     m_maskAspectVal;
    ValueMat4Ptr                    m_maskTxVal;

public:
                ImageMaskPreFullscreenEffectLogic   ();
    virtual     ~ImageMaskPreFullscreenEffectLogic  ();

protected:

    virtual void                        RenderImpl                          ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs ) override;

public:

    virtual std::vector< IValuePtr >    GetValues                           () const override;

    virtual unsigned int                GetPreferredNumOutputs              () const override;

    virtual bool                        IsFSERequired                       () const override;

    virtual void                        AddTexture                          ( const ITextureDescriptorConstPtr & ) override;

private:
    glm::mat4                           CalculateMaskTransformation         ( SizeType maskW, SizeType maskH, SizeType screenW, SizeType screenH , bool aspectMask, bool aspectScreen, SceneNode * node, RenderLogicContext * ctx ) const;

};

} // bv