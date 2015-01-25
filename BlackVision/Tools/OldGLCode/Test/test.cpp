#define GLM_SWIZZLE
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GL/glfw.h>

#include <glm/glm.hpp>
using namespace glm;


//#include "scene.h"
//#include "Rectangle.h"
#include "Mathematics/Transform/Transform.h"
#include "Mathematics/Interpolators/Interpolators.h"

#include <assert.h>

namespace bv{

}

int main_glfw( void )
{
	return 0;
	//// Initialise GLFW
	//if( !glfwInit() )
	//{
	//	fprintf( stderr, "Failed to initialize GLFW\n" );
	//	return -1;
	//}

	//glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	//glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	//glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//// Open a window and create its OpenGL context
	//if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	//{
	//	fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	//	glfwTerminate();
	//	return -1;
	//}

	//// Initialize GLEW
	//if (glewInit() != GLEW_OK) {
	//	fprintf(stderr, "Failed to initialize GLEW\n");
	//	return -1;
	//}

	//glfwSetWindowTitle( "Playground" );

	//// Ensure we can capture the escape key being pressed below
	//glfwEnable( GLFW_STICKY_KEYS );

	//// Dark blue background
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//do{
	//	// Draw nothing, see you in tutorial 2 !

	//	// Swap buffers
	//	glfwSwapBuffers();

	//} // Check if the ESC key was pressed or the window was closed
	//while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
	//	   glfwGetWindowParam( GLFW_OPENED ) );

	//// Close OpenGL window and terminate GLFW
	//glfwTerminate();

	//return 0;
}


void TestScene()
{

	glm::vec4 a = glm::vec4(2, 0, 0, 0);
	glm::vec4 b = glm::vec4(0, 2, 0, 0);
	glm::vec4 c = glm::vec4((a.zyx() + b.xyz()).xyz * 0.5f, 1.0f);
	// Before being pasted to a function, a swizzle operator needs to be cast into
	// a vector either explicitly or with the operator ()
	glm::vec2 d = glm::normalize(glm::vec2(c.yz));
	// glm::vec2 d = glm::normalize(c.yz()); // Equivalent code
	a.xyzw = d.xyxy;

	float t0 = 0.0;
	float t1 = 10.0;

	bv::FloatInterpolator attrR;
	bv::FloatInterpolator attrG;
	bv::FloatInterpolator attrB;
	bv::FloatInterpolator attrA;

	attrR.addKey( t0, 1.0f ); attrR.addKey( t1, 0.0f );
	attrG.addKey( t0, 0.2f ); attrG.addKey( (t1 + t0)  * 0.5f, 0.9f ); attrG.addKey( t1, 0.2f );
	attrB.addKey( t0, 0.0 ); attrB.addKey( t1, 1.0 );
	attrA.addKey( t0, 1.0 ); attrA.addKey( ( t1 + t0 ) * 0.5f, 0.2f ); attrA.addKey( t1, 0.0 );

//	bv::MaterialSolidColorF mat0( attrR, attrG, attrB, attrA );
//	bv::BaseObject * pRect0 = new bv::Rectangle( 200, 50, mat0 );
	//bv::SceneTree * tree = new bv::SceneTree( pRect0 );

	//bv::Camera cam( 0.0f, 2.5f, 3.0f, 0.0f, 0.0f, -2.0f, 0.0f, 1.0f, 0.0f );

	//bv::SceneBasic s( cam, tree );

	//s.Render();
}

int main(int argc, char** argv)
{
    bv::DoubleInterpolator di = bv::DoubleInterpolator(0.00005);
    
    di.addKey( 0.0, 1.0 );
    di.evaluate( -1.0 );
    di.evaluate( 0.0 );
    di.evaluate( 1.0 );
    di.addKey( 0.0, -2.0 );
    di.evaluate( -1.0 );
    di.evaluate( 0.0 );
    di.evaluate( 1.0 );
    di.addKey( 0.0, 1.0 );
    di.addKey( -1.0, -2.0 );

    bv::FloatInterpolator fi = bv::FloatInterpolator(0.0000001f);
    
    fi.addKey( 0.0, 0.0 );
    fi.addKey( 0.0, -2.0 );
    fi.addKey( 0.0, 0.0 );
    fi.addKey( -1.0, -2.0 );

    bv::DoubleTransformation trans;
    assert(trans.size() == 0);

    trans.addRotation(di, di, di, di);
    trans.addTranslation(di, di, di);
    trans.addScale(di, di, di);
    assert(trans.size() == 3);

    glm::mat4 m = trans.evaluate(1.);

	TestScene();

	main_glfw();

    return 0;
}

