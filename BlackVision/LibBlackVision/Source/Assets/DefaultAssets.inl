namespace bv
{

// ***********************
//
template<>
inline std::shared_ptr< const TextureAssetDesc >    DefaultAssets::GetDefaultDesc       ()
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
inline std::shared_ptr< const AnimationAssetDesc >  DefaultAssets::GetDefaultDesc       ()
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
inline std::shared_ptr< const FontAssetDesc >       DefaultAssets::GetDefaultDesc       ()
{
    static FontAssetDescConstPtr fontDesc;
	if( !fontDesc )
    {
        fontDesc = FontAssetDesc::Create( m_config.fontName, m_config.fontSize, 0, 0, false );
    }
    return fontDesc;
}

// ***********************
//
template<>
inline std::shared_ptr< const AVAssetDesc >    DefaultAssets::GetDefaultDesc   ()
{
    static AVAssetDescConstPtr avDesc;
	if( !avDesc )
    {
        avDesc = AVAssetDesc::Create( m_config.videoStreamPath, m_config.videoStreamFormat );
    }
    return avDesc;
}

} // bv