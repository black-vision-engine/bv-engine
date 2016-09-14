#include "SVGAssetAccessor.h"

#include "Assets/SVG/SVGAssetDescriptor.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibBlackVision

namespace bv {

SVGAssetAccessorConstPtr SVGAssetAccessor::Create          ( const Path & rootPath, const StringVector & fileExts )
{
    auto obj = new SVGAssetAccessor( rootPath, fileExts );
    return SVGAssetAccessorConstPtr( obj );
}
    
AssetDescConstPtr	SVGAssetAccessor::GetAssetDesc		( const Path & path ) const
{
    auto p = m_rootPath / path;

    if( Path::Exists( p ) )
    {
        return SVGAssetDescriptor::Create( p.Str() );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Asset '" << p.Str() << "' doesn't exist.";
        return nullptr;
    }
}

/*explicit*/                    SVGAssetAccessor::SVGAssetAccessor    ( const Path & rootPath, const StringVector & fileExts )
    : MeshAssetAccessor( rootPath, fileExts )
{
}


}
