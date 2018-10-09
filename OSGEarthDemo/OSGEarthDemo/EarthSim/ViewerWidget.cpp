#include <osgEarthQt/ViewerWidget>
#include <osgGA/StateSetManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <QtGui>
#include <QtCore/QTimer>
#include <QWidget>
#include "ViewerWidget.h"
#include "EarthData.h"

ViewerWidget::ViewerWidget(QWidget *parent)
	:osgQt::GLWidget(parent)
{
	root = new osg::Group;
	
    // create a new viewer (a simple osgViewer::Viewer)
	//root = scene;
    createViewer();
	dynamic_cast<osgViewer::Viewer*>(_viewer.get())->setSceneData(root);
	EarthData::GetInstance()->SetEarthRoot(root);
    installFrameTimer();
}

ViewerWidget::~ViewerWidget()
{
    _timer.stop();
    if ( _viewer.valid() )
    {
        _viewer->stopThreading();
        _viewer = 0L;
    }

    OE_DEBUG << "ViewerWidget::DTOR" << std::endl;
}

void ViewerWidget::SetSceneData(osg::Node* node)
{
	if (node)
	{
		dynamic_cast<osgViewer::Viewer*>(_viewer.get())->setSceneData(node);
	}

}
void ViewerWidget::setTimerInterval(int milliseconds)
{
    if ( _timer.interval() != milliseconds )
    {
        _timer.start( milliseconds );
    }
}


void ViewerWidget::installFrameTimer()
{    
    // start the frame timer.
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(30);
}


void ViewerWidget::createViewer()
{
    // creates a simple basic viewer.
    osgViewer::Viewer* viewer = new osgViewer::Viewer();
	EarthData::GetInstance()->SetEarthViewer(viewer);
	viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	earthManipulator = new osgEarth::Util::EarthManipulator();
	EarthData::GetInstance()->SetEarthManipulator(earthManipulator);
	EarthData::GetInstance()->GetEarthManipulator()->setHomeViewpoint(osgEarth::Viewpoint("", 116.3, 39.9, 0, 0, -90, 6378137.0 * 4), 0);
	osgEarth::Util::EarthManipulator::Settings *setting = earthManipulator->getSettings();
	setting->setThrowingEnabled(true);
	viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
	//dynamic_cast<osgViewer::Viewer*>(_viewer.get())->getCamera()->setCullingMode(osg::CullSettings::ENABLE_ALL_CULLING);
	viewer->getCamera()->setNearFarRatio(0.00000002f);
	viewer->setCameraManipulator(earthManipulator);
    viewer->addEventHandler(new osgViewer::StatsHandler());
    viewer->addEventHandler(new osgGA::StateSetManipulator());
    reconfigure( viewer );
    _viewer = viewer;
	dynamic_cast<osgViewer::Viewer*>(_viewer.get())->realize();

}

void ViewerWidget::reconfigure( osgViewer::View* view )
{
    if ( !_gc.valid() )
    {
        // create the Qt graphics context if necessary; it will be shared across all views.
        osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits(ds);

        traits->readDISPLAY();
        if (traits->displayNum<0) traits->displayNum = 0;

        traits->windowName = "osgEarthViewerQt";
        traits->windowDecoration = false;
        traits->x = x();
        traits->y = y();
        traits->width = width();
        traits->height = height();
        traits->doubleBuffer = true;
        traits->inheritedWindowData = new osgQt::GraphicsWindowQt::WindowData(this);
		traits->samples = 4;
		traits->sharedContext = 0;
		//traits->setInheritedWindowPixelFormat = true;

        _gc = new osgQt::GraphicsWindowQt( traits.get() );
    }

    // reconfigure this view's camera to use the Qt GC if necessary.
    osg::Camera* camera = view->getCamera();
    if ( camera->getGraphicsContext() != _gc.get() )
    {
        camera->setGraphicsContext( _gc.get() );
        if ( !camera->getViewport() )
        {
            camera->setViewport(new osg::Viewport(0, 0, _gc->getTraits()->width, _gc->getTraits()->height));
        }
        camera->setProjectionMatrixAsPerspective(30.0f, camera->getViewport()->width()/camera->getViewport()->height(), 1.0f, 10000000.0f );
    }
}

      
void ViewerWidget::paintEvent(QPaintEvent* e)
{
    if (_viewer->getRunFrameScheme() == osgViewer::ViewerBase::CONTINUOUS ||  _viewer->checkNeedToDoFrame() )
    {
        _viewer->frame();
    }
}
