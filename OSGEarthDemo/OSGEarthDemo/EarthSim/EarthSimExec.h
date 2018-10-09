#ifndef EARTHSIMEXEC_H
#define EARTHSIMEXEC_H

#include <QObject>
#include <osgDB/ReadFile>
#include <osgEarth/MapNode>
#include <osgEarth/Map>
#include <osgEarth/SpatialReference>
#include <osgEarthUtil/Sky>
#include <osgEarthUtil/Shadowing>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthSymbology/LineSymbol>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarth/ModelLayer>
#include <osgEarth/Layer>
#include <osgEarthUtil/GeodeticGraticule>
#include <osg/MatrixTransform>
#include <osgEarth/Viewpoint>
#include <osgParticle/PrecipitationEffect>
#include <osg/Fog>
#include <osgEarthUtil/Fog>
#include "ViewerWidget.h"

class EarthSimWnd;
class EarthSimExec : public QObject
{
	Q_OBJECT

public:
	EarthSimExec(EarthSimWnd* pWnd, QObject *parent = 0);
	~EarthSimExec();
	QWidget* GetViewerWgt();
private:
	void initSkyNode();
	void initWorldBoundaries();
	void initProvinceBoundaries();
	void initProvinceCapitalCity();
	void initShiBoundaries();
	void initCountyBoundaries();
	void initGraticule();

	void loadModelOnEarth(QString &filePath);
	void flyToEarthPosition(double lon, double lat, double hei);
private slots:
	void onActionGraticuleClicked();
	void onActionXianLineClicked();
	void onActionShiLineClicked();
	void onActionProvinceLineClicked();
	void onActionNationLineClicked();
	void onActionImportModelClicked();
	void onActionRainClicked();
	void onActionFogClicked();
	void onActionWindClicked();
private:
	EarthSimWnd* _pMainUi;
	ViewerWidget* _pViewer;
	osgEarth::MapNode *_pMapNode;
	osgEarth::Map *_pMap;
	const osgEarth::SpatialReference *_pMapSRS;
	osgEarth::Util::SkyNode* _pSkyNode;
	osg::ref_ptr<osgEarth::Util::GeodeticGraticule> _pGraticule;
	osg::ref_ptr<osgEarth::ModelLayer> _pXianLayer;
	osg::ref_ptr<osgEarth::ModelLayer> _pProvinceLayer;
	osg::ref_ptr<osgEarth::ModelLayer> _pShiLayer;
	osg::ref_ptr<osgEarth::ModelLayer> _pNationLayer;

	osg::ref_ptr<osgParticle::PrecipitationEffect> _pEffect;
};

#endif // EARTHSIMEXEC_H
