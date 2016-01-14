#include "NodeEffectLogic.h"


namespace bv {

// *********************************
//
NodeEffectLogic::NodeEffectLogic                    ()
    : m_preFSELogic( nullptr )
    , m_FSE( nullptr )
    , m_postFSELogic( nullptr )
{
}

// *********************************
//
NodeEffectLogic::~NodeEffectLogic                   ()
{
    delete m_preFSELogic;
    delete m_FSE;
    delete m_postFSELogic;
}

// *********************************
//
void    NodeEffectLogic::Render                      ( SceneNode * node, RenderLogicContext * ctx )
{
    { node; ctx; }
}

// *********************************
//
void    NodeEffectLogic::SetPreFullscreenEffectLogic ( PreFullscreenEffectLogic * logic )
{
    delete m_preFSELogic;

    m_preFSELogic = logic;
}

// *********************************
//
void    NodeEffectLogic::SetPostFullscreenEffectLogic( PostFullscreenEffectLogic * logic )
{
    delete m_postFSELogic;

    m_postFSELogic = logic;
}

// *********************************
//
void    NodeEffectLogic::SetFullscreenEffect         ( FullscreenEffectTr * fse )
{
    delete m_FSE;

    m_FSE = fse;
}

} //bv
