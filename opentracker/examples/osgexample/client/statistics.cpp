#include <OpenSG/OSGRenderAction.h>
#include <OpenSG/OSGViewport.h>
#include <OpenSG/OSGImageBackground.h>
#include <OpenSG/OSGImage.h>
#include <OpenSG/OSGImageForeground.h>
#include <OpenSG/OSGFileGrabForeground.h>
#include <OpenSG/OSGPerspectiveCamera.h>
#include <OpenSG/OSGStereoBufferViewport.h>
#include <OpenSG/OSGColorBufferViewport.h>
#include <OpenSG/OSGShearedStereoCameraDecorator.h>
#include <OpenSG/OSGSimpleStatisticsForeground.h>
#include <OpenSG/OSGDrawable.h>
#include <OpenSG/OSGRenderAction.h>
#include <OpenSG/OSGDrawActionBase.h>

OSG_USING_NAMESPACE

void showStats(ViewportPtr _viewport, RenderAction *_action);


void showStats(ViewportPtr _viewport, RenderAction *_action)
{
   SimpleStatisticsForegroundPtr sf = SimpleStatisticsForeground::create();
   beginEditCP(sf);

    sf->setSize(25);
    sf->setColor(Color4f(0,1,0,0.7));
	sf->addElement(RenderAction::statTravTime, "FPS: %r.3f");
	sf->addElement(DrawActionBase::statCulledNodes, "%d Nodes culled");
	sf->addElement(RenderAction::statNMaterials, "%d material changes");
	sf->addElement(RenderAction::statNMatrices,  "%d matrix changes");
	sf->addElement(Drawable::statNTriangles,            "%d triangles drawn");
	sf->addElement(Drawable::statNLines,                "%d lines drawn");
	sf->addElement(Drawable::statNPoints,                "%d points drawn");
	sf->addElement(Drawable::statNVertices,            "%d vertices transformed");
	sf->addElement(RenderAction::statNTextures,        "%d textures used");
	sf->addElement(RenderAction::statNTexBytes,        "%d bytes of texture used");
   endEditCP(sf);

     _viewport->getForegrounds().push_back(sf);
	// _viewport->
	 _action->setStatistics(&sf->getCollector());



}