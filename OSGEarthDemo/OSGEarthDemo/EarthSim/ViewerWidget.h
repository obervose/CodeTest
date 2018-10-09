#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H 

#include <osgEarthQt/Common>

#include <osgEarth/Map>
#include <osgEarthUtil/EarthManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/ViewerBase>

#include <QtCore/QTimer>


/**
    * Qt widget that encapsulates an osgViewer::Viewer.
    */
class  ViewerWidget : public osgQt::GLWidget
{
    Q_OBJECT;

public:
    /**
        * Constructs a new ViewerWidget, creating an underlying viewer.
        * @param[in ] scene Scene graph to attach to the viewer (optional)
        */
    ViewerWidget(QWidget *parent=NULL);

    /**
        * Access the underlying viewer.
        */
    osgViewer::ViewerBase* getViewer() { return _viewer.get(); }

    /**
        * Populates the incoming collection with the views comprising this viewer.
        */
    template<typename T>
    void getViews( T& views ) const {
        osgViewer::ViewerBase::Views temp;
        _viewer->getViews(temp);
        views.insert(views.end(), temp.begin(), temp.end());
    }

    virtual ~ViewerWidget();
	osgEarth::Util::EarthManipulator* GetMaipulator(){ return earthManipulator.get(); }

	void SetSceneData(osg::Node* root);
public slots:
        
    /**
        * Change the underlying timer's interval
        */
    void setTimerInterval( int milliseconds );


protected:

    QTimer _timer;

    void installFrameTimer();

    void createViewer();
    void reconfigure( osgViewer::View* );
    void paintEvent( QPaintEvent* );

    osg::observer_ptr<osgViewer::ViewerBase> _viewer;

    osg::ref_ptr<osg::GraphicsContext>       _gc;
	osg::ref_ptr<osg::Group> root;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> earthManipulator;
};


#endif // OSGEARTHQT_VIEWERWIDGET_H
