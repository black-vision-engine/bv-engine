#pragma once

#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"
#include "Engine/Types/Values/TypedValues.h"

namespace bv { namespace model {

class SamplerStateModel;
DEFINE_PTR_TYPE(SamplerStateModel)
DEFINE_CONST_PTR_TYPE(SamplerStateModel)

class SamplerStateModel : public ResourceStateModel
{
public:
	struct Params
	{
		static const std::string WRAPPING_MODE_X;
		static const std::string WRAPPING_MODE_Y;
		static const std::string WRAPPING_MODE_Z;
		static const std::string FILTERING_MODE;
		static const std::string BORDER_COLOR;
	};

private:

	ValueIntPtr					m_wrappingModeX;
	ValueIntPtr					m_wrappingModeY;
	ValueIntPtr					m_wrappingModeZ;
	ValueIntPtr					m_filteringMode;
	ValueVec4Ptr				m_borderColor;

public:
								SamplerStateModel	( ITimeEvaluatorPtr timeEvaluator );
								SamplerStateModel	( ITimeEvaluatorPtr timeEvaluator, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc );
	
	static SamplerStateModelPtr	Create				( ITimeEvaluatorPtr timeEvaluator );
	static SamplerStateModelPtr	Create				( ITimeEvaluatorPtr timeEvaluator, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc );

    TextureWrappingMode			GetWrappingModeX	() const;
    TextureWrappingMode			GetWrappingModeY	() const;
	TextureWrappingMode			GetWrappingModeZ	() const;
    TextureFilteringMode		GetFilteringMode	() const;
    const glm::vec4 &			GetBorderColor		() const;

    void                        SetWrappingModeX	( TextureWrappingMode wm );
    void                        SetWrappingModeY	( TextureWrappingMode wm );
	void                        SetWrappingModeZ	( TextureWrappingMode wm );
    void                        SetFilteringMode	( TextureFilteringMode fm );
    void                        SetBorderColor		( const glm::vec4 & bc );

private:
	void						Initialize			( ITimeEvaluatorPtr timeEvaluator, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc );

};

} //model
} //bv
