#pragma once

#include "Common/PrototyperCore.h"
#include "Interfaces/IRenderable.h"

namespace bv {

class VBOCube : public IRenderable
{
private:

    GLuint  m_vaoHandle;
	
	static const unsigned int INDICIES_NUM;

public:

    VBOCube                 ( float x );
    ~VBOCube                ();

    virtual void    Render  () override;

};

} //bv