#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"



namespace bv { namespace model {

class DefaultGeometryShaderChannel;
DEFINE_PTR_TYPE(DefaultGeometryShaderChannel)



class DefaultGeometryShaderChannel : public ShaderChannel< IGeometryShaderChannel >
{
private:
public:
    explicit DefaultGeometryShaderChannel       ( const std::string & shaderSource, IValueSetConstPtr values );
            ~DefaultGeometryShaderChannel       ();


    static  DefaultGeometryShaderChannelPtr    Create              ();
    static  DefaultGeometryShaderChannelPtr    Create              ( const std::string & shaderFile, IValueSetConstPtr values );
    
    static  DefaultGeometryShaderChannelPtr    Create              ( IValueSetConstPtr values );

};



} //model
} //bv

