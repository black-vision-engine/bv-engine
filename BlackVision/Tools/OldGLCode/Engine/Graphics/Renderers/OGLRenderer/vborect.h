#pragma once

namespace bv {

class VBORect
{
private:

    unsigned int m_VaoHandle;

public:

	VBORect		( float w, float h );

    void Render	() const;

};

} //bv
