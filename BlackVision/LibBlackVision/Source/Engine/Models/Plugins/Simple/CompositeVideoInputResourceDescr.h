#include "Engine/Models/Resources/IPluginResourceDescr.h"

#include <vector>

namespace bv { namespace model {

    class CompositeVideoInputResourceDescr : public IVideoInputResourceDescr
    {
        std::vector< IVideoInputResourceDescrConstPtr > inputs;
        size_t currentInput;

    public:
        CompositeVideoInputResourceDescr() { currentInput = size_t( -1 ); }
        virtual PluginResourceType      GetResourceType () const { return PluginResourceType::PRT_VIDEOINPUT; }
        virtual ITextureDescriptor*     GetITextureDescriptor  () const override { return inputs[ currentInput ]->GetITextureDescriptor(); }
        virtual void Update() const override { inputs[ currentInput ]->Update(); }
        
        void AddAndSetInputAsCurrent( IVideoInputResourceDescrConstPtr d ) { inputs.push_back( d ); currentInput = inputs.size()-1; }
        void SetCurrentInput( int i ) { assert( i < inputs.size() ); currentInput = i; }
    };

} }