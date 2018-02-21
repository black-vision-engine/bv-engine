#include "ParametersSets.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/SamplerStateModel.h"


namespace bv
{




// ***********************
//
void            ParametersSets::SetSamplerStateParamsTestSet1        ( model::IParamValModelPtr resStateModel )
{
    auto samplerModel = std::static_pointer_cast< model::SamplerStateModel >( resStateModel );

    samplerModel->SetWrappingModeX( TextureWrappingMode::TWM_CLAMP_BORDER );
    samplerModel->SetWrappingModeY( TextureWrappingMode::TWM_REPEAT );
    samplerModel->SetWrappingModeZ( TextureWrappingMode::TWM_CLAMP_EDGE );

    samplerModel->SetFilteringMode( TextureFilteringMode::TFM_POINT_MIPMAP_LINEAR );
    samplerModel->SetBorderColor( glm::vec4( 0.24f, 0.34f, 0.44f, 0.54f ) );
}

// ***********************
/// @Note This function compares resStateModel parameters with values set by SetSamplerStateParamsTestSet1.
bool            ParametersSets::CompareToSamplerStateParamsTestSet1  ( model::IParamValModelPtr resStateModel )
{
    auto samplerModel = std::static_pointer_cast< model::SamplerStateModel >( resStateModel );

    model::SamplerStateModelPtr referenceState = std::make_shared< model::SamplerStateModel >( samplerModel->GetParameter( model::SamplerStateModel::Params::WRAPPING_MODE_X )->GetTimeEvaluator() );
    SetSamplerStateParamsTestSet1( referenceState );


    if( samplerModel->GetBorderColor() != referenceState->GetBorderColor() )
        return false;

    if( samplerModel->GetWrappingModeX() != referenceState->GetWrappingModeX() )
        return false;

    if( samplerModel->GetWrappingModeY() != referenceState->GetWrappingModeY() )
        return false;

    if( samplerModel->GetWrappingModeZ() != referenceState->GetWrappingModeZ() )
        return false;

    if( samplerModel->GetFilteringMode() != referenceState->GetFilteringMode() )
        return false;
    
    return true;
}

}	// bv
