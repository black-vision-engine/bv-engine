#pragma once

#include "Mathematics/Transform/MatTransform.h"



namespace bv
{

// ***********************
//
class TEST_ACCESSOR( CompositeTransform )
{
public:

    static FloatInterpolator &      TranslationX    ( CompositeTransform & transform )  { return transform.m_translationX; }
    static FloatInterpolator &      TranslationY    ( CompositeTransform & transform )  { return transform.m_translationX; }
    static FloatInterpolator &      TranslationZ    ( CompositeTransform & transform )  { return transform.m_translationX; }

    static FloatInterpolator &      Pitch           ( CompositeTransform & transform )  { return transform.m_eulerPitch; }
    static FloatInterpolator &      Yaw             ( CompositeTransform & transform )  { return transform.m_eulerYaw; }
    static FloatInterpolator &      Roll            ( CompositeTransform & transform )  { return transform.m_eulerRoll; }

    static FloatInterpolator &      ScaleX          ( CompositeTransform & transform )  { return transform.m_scaleX; }
    static FloatInterpolator &      ScaleY          ( CompositeTransform & transform )  { return transform.m_scaleY; }
    static FloatInterpolator &      ScaleZ          ( CompositeTransform & transform )  { return transform.m_scaleZ; }

    static FloatInterpolator &      CenterX         ( CompositeTransform & transform )  { return transform.m_centerX; }
    static FloatInterpolator &      CenterY         ( CompositeTransform & transform )  { return transform.m_centerY; }
    static FloatInterpolator &      CenterZ         ( CompositeTransform & transform )  { return transform.m_centerZ; }

    static const FloatInterpolator &      TranslationX    ( const CompositeTransform & transform )  { return transform.m_translationX; }
    static const FloatInterpolator &      TranslationY    ( const CompositeTransform & transform )  { return transform.m_translationX; }
    static const FloatInterpolator &      TranslationZ    ( const CompositeTransform & transform )  { return transform.m_translationX; }

    static const FloatInterpolator &      Pitch           ( const CompositeTransform & transform )  { return transform.m_eulerPitch; }
    static const FloatInterpolator &      Yaw             ( const CompositeTransform & transform )  { return transform.m_eulerYaw; }
    static const FloatInterpolator &      Roll            ( const CompositeTransform & transform )  { return transform.m_eulerRoll; }

    static const FloatInterpolator &      ScaleX          ( const CompositeTransform & transform )  { return transform.m_scaleX; }
    static const FloatInterpolator &      ScaleY          ( const CompositeTransform & transform )  { return transform.m_scaleY; }
    static const FloatInterpolator &      ScaleZ          ( const CompositeTransform & transform )  { return transform.m_scaleZ; }

    static const FloatInterpolator &      CenterX         ( const CompositeTransform & transform )  { return transform.m_centerX; }
    static const FloatInterpolator &      CenterY         ( const CompositeTransform & transform )  { return transform.m_centerY; }
    static const FloatInterpolator &      CenterZ         ( const CompositeTransform & transform )  { return transform.m_centerZ; }
};


}	// bv


