#include "DefaultAssets.h"




namespace bv
{


// ========================================================================= //
// Default assets descriptors
// ========================================================================= //


// ***********************
//
template< typename AssetDescType >
inline std::shared_ptr< const AssetDescType >               DefaultAssets::GetDefaultDesc   ()
{
    static_assert( false, "Implement DefaultAssets::GetDefaultDesc specialization for this type." );
}

// ***********************
//
template<>
inline std::shared_ptr< const TextureAssetDesc >            DefaultAssets::GetDefaultDesc       ()
{
    static TextureAssetDescConstPtr texture2DDesc;
	if( !texture2DDesc )
    {
		auto props = image::GetImageProps( ProjectManager::GetInstance()->ToAbsPath( m_config.texture2DPath ).Str() );

        if( !props.error.empty() )
        {
            assert( false ); //missing default asset
            return nullptr;
        }
        texture2DDesc = TextureAssetDesc::Create( SingleTextureAssetDesc::Create( m_config.texture2DPath, props.width, props.height, EnumsUtils::Convert( props.format ), true ) );
    }
	return texture2DDesc;
}

// ***********************
//
template<>
inline std::shared_ptr< const AnimationAssetDesc >          DefaultAssets::GetDefaultDesc       ()
{
    static AnimationAssetDescConstPtr animation2DDesc;
	if( !animation2DDesc )
    {
        animation2DDesc = AnimationAssetDesc::Create( m_config.animation2DDir, 1, m_config.animationFilter );
    }
    return animation2DDesc;
}

// ***********************
//
template<>
inline std::shared_ptr< const FontAssetDesc >               DefaultAssets::GetDefaultDesc       ()
{
    static FontAssetDescConstPtr fontDesc;
	if( !fontDesc )
    {
        fontDesc = FontAssetDesc::Create( m_config.fontName, m_config.fontSize, 0, 0, 0, true );
    }
    return fontDesc;
}

// ***********************
//
template<>
inline std::shared_ptr< const AVAssetDesc >                 DefaultAssets::GetDefaultDesc   ()
{
    static AVAssetDescConstPtr avDesc;
	if( !avDesc )
    {
        avDesc = AVAssetDesc::Create( m_config.videoStreamPath, m_config.videoStreamFormat );
    }
    return avDesc;
}

// ***********************
//
template<>
inline std::shared_ptr< const DataArrayAssetDesc >          DefaultAssets::GetDefaultDesc   ()
{
    static DataArrayAssetDescConstPtr daDesc;
	if( !daDesc )
    {
        daDesc = DataArrayAssetDesc::Create( std::vector< std::string >(), std::vector< std::string >(), std::vector< ModelParamType >() );
    }
    return daDesc;
}

// ***********************
//
template<>
inline std::shared_ptr< const DataArrayRowAssetDesc >       DefaultAssets::GetDefaultDesc   ()
{
    static DataArrayRowAssetDescConstPtr daDesc;
    if( !daDesc )
    {
        daDesc = DataArrayRowAssetDesc::Create( std::vector< DataArrayRowBase * >() );
    }
    return daDesc;
}

// ***********************
//
template<>
inline std::shared_ptr< const VideoInputAssetDesc >         DefaultAssets::GetDefaultDesc   ()
{
    static VideoInputAssetDescConstPtr daDesc;
    if( !daDesc )
    {
        daDesc = VideoInputAssetDesc::Create( 0 );
    }
    return daDesc;
}

// ========================================================================= //
// Fallback assets descriptors
// ========================================================================= //

// ***********************
//
template< typename AssetDescType >
inline std::shared_ptr< const AssetDescType >               DefaultAssets::GetFallbackDesc  ()
{
    static_assert( false, "Implement DefaultAssets::GetFallbackDesc specialization for this type." );
}

// ***********************
//
template<>
inline std::shared_ptr< const TextureAssetDesc >            DefaultAssets::GetFallbackDesc  ()
{
    auto props = image::GetImageProps( ProjectManager::GetInstance()->ToAbsPath( m_config.texture2DFallbackPath ).Str() );

    if( !props.error.empty() )
        return nullptr;

    return TextureAssetDesc::Create( SingleTextureAssetDesc::Create( m_config.texture2DFallbackPath, props.width, props.height, EnumsUtils::Convert( props.format ), true ) );
}


} // bv


