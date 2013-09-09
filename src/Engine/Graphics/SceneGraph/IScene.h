#pragma once


namespace bv
{

class Renderer;
class Camera;

class IScene
{
public:

    /**
      Load textures, initialize shaders, etc.
      */
    virtual void	initScene	()					= 0;

    /**
      This is called prior to every frame. Float
      was chosen arbitrarily now.
      */
    virtual void	update		( float t )			= 0;

    /**
      Scene drawing. Will be updated during
      implementation of additional features.
      */
    virtual void     render( Renderer* renderer ) = 0;


    virtual void     setCamera(Camera* cam) = 0;

    /**
      Usually something has to be done when 
      window/scene is resized.
      */
    virtual void	resize		( int w, int h )	= 0;

    /**
      Virtual destructor must be implemented 
      if IScene is ever to be deleted by ptr
    */
    virtual			~IScene		() {}
};

} // bv