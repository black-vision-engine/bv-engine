#include "MainDisplayTarget.h"

#include <cassert>
#include <vector>

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"


namespace bv {

// **************************
//
RenderTarget *  MainDisplayTarget::CreateDisplayRenderTarget    ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, false, false );
}

// **************************
//
RenderTarget *  MainDisplayTarget::CreateAuxRenderTarget        ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, true, false );
}

// **************************
//
Camera *        MainDisplayTarget::CreateDisplayCamera          ()
{
    auto camera = new Camera( false );

    //FIXME: upewnic sie, ze tu sa prawdziwe macierze jednostkowe bez zadnych piardow z zaokraglen (i jesli nie sa, to recznie to zapewnic ustawiajac macierze z palca)
    //Ponizszy kod tworzy poprawne macierze identycznosciowe, ale konstruktor domyslnie tez to robi - kod zostaje dla celow szkoleniowych, ale zostawiamy wyniki z konstruktora macierzy w kamerze
    
    //camera->SetFrame( glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 0.f, -1.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //camera->SetFrustum( -1.f, 1.f, -1.f, 1.f, 1.f, -1.f );

    //FIXME: pewnie mozna na spokojnie te macierze potem poprawic na identycznosciowe (skoro kod do ich generacji jest poporawny i wiadomo, na jakiej zasadzie dziala)
    return camera;
}

// **************************
//
TriangleStrip * MainDisplayTarget::CreateDisplayRect            ()
{
    //FIXME: implement
    return nullptr;
}

} //bv
