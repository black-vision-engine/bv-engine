#include "MockScenes.h"

#include "Engine/Models/Plugins/Geometry/PluginGeometryRing.h"
#include "Engine/Models/Plugins/Geometry/PluginGeometryRect.h"
#include "Engine\Models\Plugins\Extrude.h"
#include "Engine/Models/Plugins/Solid.h"
#include "Engine\Models\BasicNode.h"
#include "Mathematics\Transform\MatTransform.h"
#include "Engine\Models\ModelFactory.h"
#include "Engine\Models\Plugins\Transform\PluginTransformSimple.h"

namespace bv
{

bv::PluginTransformSimple* FuncTranslate(float x,float y,float z)
{
	bv::TransformF* trans = new bv::TransformF();
	bv::FloatInterpolator transX;
	transX.addKey(0.f,x);
	bv::FloatInterpolator transY;
	transY.addKey(0.f,y);
	bv::FloatInterpolator transZ;
	transZ.addKey(0.f,z);
	trans->addTranslation(transX,transY,transZ);
	bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);
	return transPlugin;
}



void AnimLines(bv::BasicNode* parent)
{
	bv::BasicNode* lineUp = new bv::BasicNode();
	bv::Vec4Interpolator color;
	
	color.addKey(0.01, glm::vec4(0,0,1,1));
	color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);
	bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
	lineUp->setPixelShaderPlugin(solidRect);
	lineUp->addGeometryPlugin(new bv::PluginGeometryRect(0.1f,0.1f));
	
	//bez sensu
	bv::FloatInterpolator si;
    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    float s = 5.0f;
    si.addKey(0.f, 0.1f);
	lineUp->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));


	bv::FloatInterpolator identity; 
	identity.addKey(0.f, 1.f);


	bv::FloatInterpolator scaleX; 
	scaleX.setWrapPostMethod(bv::WrapMethod::clamp);
    scaleX.addKey(0.f, 1.f);
    scaleX.addKey(2.f, 10.0f);
    
	bv::TransformF* trans = new bv::TransformF();
	trans->addScale(scaleX,identity,identity);
    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

	

	lineUp->addTransformPlugin(transPlugin);

	lineUp->addTransformPlugin(FuncTranslate(0,-1,0));




	if(parent != nullptr)
    {
        parent->addChild(lineUp);
    }

}


bv::BasicNode* CreateRect(bv::BasicNode* parent)
{
	bv::BasicNode* rectNode = new bv::BasicNode();
	bv::Vec4Interpolator color;
	color.addKey(0, glm::vec4(1,0,0,1));
	color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);
	bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
	rectNode->setPixelShaderPlugin(solidRect);

	//extrude
	bv::FloatInterpolator si;
    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    float s = 5.0f;
    si.addKey(0.f, 0.1f);
    si.addKey(1.f, -0.1f);
    //si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
	rectNode->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
	
	rectNode->addGeometryPlugin(new bv::PluginGeometryRect(0.2f,0.1f));

	// b edzie sie rozszerzal

	bv::FloatInterpolator identity; 
	identity.addKey(0.f, 1.f);

	bv::FloatInterpolator scaleX; scaleX.setWrapPostMethod(bv::WrapMethod::pingPong);
    scaleX.addKey(0.f, 1.f);
    scaleX.addKey(5.f, 1.2f);
    
	bv::TransformF* trans = new bv::TransformF();
	trans->addScale(scaleX,identity,identity);
    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

	rectNode->addTransformPlugin(transPlugin);
	if(parent != nullptr)
    {
        parent->addChild(rectNode);
    }
	return rectNode;
}

void createMyRing(bv::BasicNode* parent,float offsetZ,float offsetX,float offsetY,float startAngle,float endAngle)
{
	bv::BasicNode* mockTree = new bv::BasicNode();

	


    // SOLID
    bv::Vec4Interpolator color;

    float rs = 10.f;

    color.addKey(2, glm::vec4(0.1,0,0,1));
    color.addKey(5, glm::vec4(1,0,0,1));
 
    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);

    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

    mockTree->setPixelShaderPlugin(solid1);

    bv::FloatInterpolator si;

    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    float s = 5.0f;
    si.addKey(0.f, 0.1f);
    si.addKey(0.2f, 1.2f);
    //si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);

	mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));

	FloatInterpolator radIn;
	FloatInterpolator radOut;
	FloatInterpolator start;
	FloatInterpolator end;

	radIn.addKey(0.f, 1.7f);
	radOut.addKey(0.f, 2.f);
	start.addKey(0.f, startAngle);
	end.addKey(0.f, endAngle);
	   mockTree->addGeometryPlugin(new bv::PluginGeometryRing(radIn, radOut, start, end, 3));
	//mockTree->addGeometryPlugin(new bv::PluginGeometryRect());

    // TRANSFORMATION
    bv::FloatInterpolator angle; angle.setWrapPostMethod(bv::WrapMethod::pingPong);
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    x.addKey(0.f, 0.f);
    y.addKey(0.f, 0.f);
    z.addKey(0.f, -1.1f);
    angle.addKey(0.f, 0.f);
    //angle.addKey(5.f, 180.f);

    bv::TransformF* trans = new bv::TransformF();
    
	bv::FloatInterpolator transX;
	transX.addKey(0.f,offsetX);

	bv::FloatInterpolator transY;
	transY.addKey(0.f,offsetY);
	
	bv::FloatInterpolator transZ;
	transZ.addKey(0.f,offsetZ);

	trans->addTranslation(transX,transY,transZ);

	

	trans->addTransform(new bv::RotationF(angle, x,y,z));
    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

    mockTree->addTransformPlugin(transPlugin);


	

    if(parent != nullptr)
    {
        parent->addChild(mockTree);
    }
}
void NightRect(bv::BasicNode* parent, int number,float offsetX,float offsetY)
{
	float RectWidth=0.04;
	bv::BasicNode* rectNode = new bv::BasicNode();
	bv::Vec4Interpolator color;
	float atom=0.1;
	color.addKey(number*atom, glm::vec4(0,0,0,0));
	color.addKey(number*atom+2, glm::vec4(0,0,0,0));
	color.addKey(number*atom+3, glm::vec4(1,0,0,1));
	color.addKey(number*atom+4, glm::vec4(0,0,0,0));
	color.addKey(number*atom+6, glm::vec4(0,0,0,0));
	color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::repeat);
	bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
	rectNode->setPixelShaderPlugin(solidRect);

	//extrude
	bv::FloatInterpolator si;
    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    float s = 5.0f;
    si.addKey(0.f, 0.1f);
    //si.addKey(1.f, 0.3f);
    //si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
	rectNode->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
	
	rectNode->addGeometryPlugin(new bv::PluginGeometryRect(RectWidth,0.1f));

	// b edzie sie rozszerzal

	bv::FloatInterpolator identity; 
	identity.addKey(0.f, 1.f);

	bv::FloatInterpolator scaleX; scaleX.setWrapPostMethod(bv::WrapMethod::pingPong);
    scaleX.addKey(0.f, 1.f);
    scaleX.addKey(5.f, 1.2f);
    
	bv::TransformF* trans = new bv::TransformF();
	trans->addScale(scaleX,identity,identity);
    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);
	
	rectNode->addTransformPlugin(transPlugin);
	rectNode->addTransformPlugin(FuncTranslate(offsetX,offsetY,-4+number*RectWidth*6));
	if(parent != nullptr)
    {
        parent->addChild(rectNode);
    }
	
}
void NightRider(bv::BasicNode* parent)
{
	bv::BasicNode* NightRects= new bv::BasicNode();

	for(int i=0;i<50;i++)
	{
		NightRect(parent,i,-2,0);
		NightRect(parent,i,2,0);
		NightRect(parent,i,0,-2);
		NightRect(parent,i,0,2);
		

	}
	/*if(parent != nullptr)
    {
        parent->addChild(NightRects);
    }*/
}

void CreateRings(bv::BasicNode* parent,float z)
{
	float Angle1=3.1415f;
	float Angle2=3.1415f/2.0;
	float Angle3=0;
	float Angle4=-3.1415f/2;
	float offset=-3.1415f/2;
	createMyRing(parent,z,-1,0.5,Angle1,Angle1+offset);	
	createMyRing(parent,z,1,0.5,Angle2,Angle2+offset);
	
	createMyRing(parent,z,1,-0.5,Angle3, Angle3+offset);	
	createMyRing(parent,z,-1,-0.5,Angle4,Angle4+offset);
	
}

bv::BasicNode* SceneExamples::CreateSceneMock3(bv::BasicNode* parent)
{
	bv::BasicNode* rect=CreateRect(parent);

	

	CreateRings(rect,0);
	CreateRings(rect,-3);
	CreateRings(rect,-6);
	CreateRings(rect,-9);
	CreateRings(rect,-12);
	CreateRings(rect,-15);

	AnimLines(rect);
	NightRider(rect);

    return rect;


}

}