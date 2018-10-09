#ifndef EARTHDATA_H
#define EARTHDATA_H
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgEarthUtil/EarthManipulator>
class EarthData
{
public:
	
	~EarthData();
	static EarthData* GetInstance();
	osg::Group* GetEarthRoot(){ return _pRoot; }
	void SetEarthRoot(osg::Group *node);

	osg::Group* GetEarthOtherRoot(){ return _pOtherRoot; }
	void SetEarthOtherRoot(osg::Group *node){ _pOtherRoot = node; }

	osgViewer::Viewer* GetEarthViewer(){ return _pViewer; }
	void SetEarthViewer(osgViewer::Viewer *viewer){ _pViewer = viewer; }

	osgEarth::Util::EarthManipulator* GetEarthManipulator(){ return _pManipulator; }
	void SetEarthManipulator(osgEarth::Util::EarthManipulator* manipulator){ _pManipulator = manipulator; }
private:
	EarthData();
private:
	static EarthData* _pInstance;
	osg::ref_ptr<osg::Group> _pRoot;
	osg::ref_ptr<osg::Group> _pOtherRoot;
	osg::ref_ptr<osgViewer::Viewer> _pViewer;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> _pManipulator;
};

#endif // EARTHDATA_H
