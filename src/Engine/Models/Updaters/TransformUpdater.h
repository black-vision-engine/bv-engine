#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Interfaces/IUpdater.h"

#include "Engine/Graphics/Types/Transform.h"


namespace bv
{

class TransformableEntity;

namespace model
{
    class ITransformChannel;
}

class TransformUpdater : public IUpdater
{
private:

    TransformableEntity *               m_out;
    const model::ITransformChannel *    m_in;

public:
            //FIXME: transform updater should use new implementation based on channels
            TransformUpdater    ( TransformableEntity * out, const model::ITransformChannel * in ); 
            ~TransformUpdater   ();

   virtual  void    Update      ( TimeType t );

};

}
