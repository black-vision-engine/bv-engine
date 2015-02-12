#pragma once

#include "Engine/Interfaces/IShaderDataSource.h"


namespace bv {

enum class ShaderDataSourceType : int
{
    SDST_SOLID_COLOR = 0,
    SDST_ONE_TEXTURE,
    SDST_TOTAL
};

class ShaderDataSourceCreator
{
public:

    static IShaderDataSourceConstPtr    VertexShader        ( ShaderDataSourceType sdst );
    static IShaderDataSourceConstPtr    FragmentShader      ( ShaderDataSourceType sdst );
    static IShaderDataSourceConstPtr    GeometryShader      ( ShaderDataSourceType sdst );

private:

    static std::string                  VertexShaderSource  ( ShaderDataSourceType sdst );
    static std::string                  FragmentShaderSource( ShaderDataSourceType sdst );
    static std::string                  GeometryShaderSource( ShaderDataSourceType sdst );

    static std::string                  VertexShaderFile    ( ShaderDataSourceType sdst );
    static std::string                  FragmentShaderFile  ( ShaderDataSourceType sdst );
    static std::string                  GeometryShaderFile  ( ShaderDataSourceType sdst );

    static std::string                  ShadersRootDir      ();

    static std::string                  ReadSourceFromFile  ( const std::string & filename );

};

} // bv
