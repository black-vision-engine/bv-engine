#pragma once

#include "Engine/Interfaces/IShaderDataSource.h"

#include "Prototypes/Engine/Common/TexturesDataImpl.h"

#include "Common/PrototyperCore.h"


namespace bv {

class ShaderDataSourceImpl : public IShaderDataSource
{
private:

    std::string     m_source;

    std::vector< IValueConstPtr >   m_values;

    TexturesDataImplPtr             m_texturesData;

public:

                                    ShaderDataSourceImpl    ( const std::string & source );
                                    ~ShaderDataSourceImpl   ();

    void                            AddValue                ( const std::string & name, const glm::vec4 & val );
    void                            AddValue                ( const std::string & name, float val );

    bool                            AddTextureFromFile      ( const std::string & textureFile, const std::string & textureName );

    virtual const std::string &     GetShaderSource         ()  const override;
    virtual ITexturesDataConstPtr   GetTexturesData         ()  const override;

    virtual const std::vector< IValueConstPtr > &   GetValues   () const override;
    virtual IValueConstPtr                          GetValue    ( const std::string & name ) const override;

};

DEFINE_PTR_TYPE(ShaderDataSourceImpl)
DEFINE_CONST_PTR_TYPE(ShaderDataSourceImpl)

} // bv
