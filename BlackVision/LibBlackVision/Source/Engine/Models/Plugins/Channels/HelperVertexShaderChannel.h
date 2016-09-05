#pragma once

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model
{


class HelperVertexShaderChannel
{
public:
    static void         InverseTextureMatrix        ( IPluginParamValModelPtr pluginParamValModel, const char * paramName );
};

}   // model
}   // bv
