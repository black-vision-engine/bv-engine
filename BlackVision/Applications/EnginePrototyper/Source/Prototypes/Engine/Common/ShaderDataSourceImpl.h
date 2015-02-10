#pragma once

#include "Engine/Interfaces/IShaderDataSource.h"


namespace bv {

class ShaderDataSourceImpl : public IShaderDataSource
{
private:

    std::string     m_source;

public:

                                    ShaderDataSourceImpl    ( const std::string & source );
                                    ~ShaderDataSourceImpl   ();

    virtual const std::string &     GetShaderSource         ()  const override;
    virtual ITexturesDataConstPtr   GetTexturesData         ()  const override;

    virtual const std::vector< IValueConstPtr > &   GetValues   () const override;
    virtual IValueConstPtr                          GetValue    ( const std::string & name ) const override;

};

} // bv
