#pragma once

#include "IUpdater.h"
#include "Transform.h"
#include "Engine/Models/IPlugin.h"

namespace bv
{
class TransformableEntity;

class TransformUpdater : public IUpdater
{
private:
    TransformableEntity*    m_out;
    TTransformPluginVec*    m_in;
public:
            TransformUpdater    ( TransformableEntity* out, TTransformPluginVec* in ); 
            ~TransformUpdater   ();

    void    Update              ( float t );

};

}