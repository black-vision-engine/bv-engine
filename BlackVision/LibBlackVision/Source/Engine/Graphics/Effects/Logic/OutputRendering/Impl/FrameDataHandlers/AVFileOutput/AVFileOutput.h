#pragma once

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/OutputInstance.h"

namespace bv
{
class AVFileOutputHandler;

class AVFileOutput : public OutputInstance
{
    AVFileOutputHandler * m_handler;

public:

    AVFileOutput          ( unsigned int width, unsigned int height, AVFileOutputHandler * handler );
    ~AVFileOutput         ();

    void            StartToAVFileRendering  ( const std::string & outputFilePath );
    void            StopToAVFileRendering   ();
};

};
