#include "EarthSimExec.h"
#include "EarthData.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "EarthSimWnd.h"
EarthSimExec::EarthSimExec(EarthSimWnd* pWnd, QObject *parent)
	: QObject(parent)
	, _pMainUi(pWnd)
	, _pEffect(NULL)
{
	
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../resource/EarthSim/EarthData/3d.earth");
	if (node)
	{
		_pViewer = new ViewerWidget(_pMainUi);
		_pMapNode = osgEarth::MapNode::findMapNode(node);
		_pMapNode->setName("MapNode");
		EarthData::GetInstance()->GetEarthRoot()->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
		_pMap = _pMapNode->getMap();
		_pMapSRS = _pMapNode->getMapSRS();
		
	}
	else
	{
		QMessageBox::critical(_pMainUi, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("MapNode加载失败。。。"));
		return;
	}
	initSkyNode();
	/*initGraticule();
	initWorldBoundaries();
	initProvinceBoundaries();
	initShiBoundaries();
	initCountyBoundaries();*/
}

EarthSimExec::~EarthSimExec()
{

}

QWidget* EarthSimExec::GetViewerWgt()
{
	return _pViewer;
}

void EarthSimExec::initSkyNode()
{
	//osg::DisplaySettings::instance()->setNumMultiSamples(4);
	//EarthData::GetInstance()->GetEarthViewer()->getCamera()->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(_pMapNode));
	//double equatorRadius = _pMap->getSRS()->getEllipsoid()->getRadiusEquator();//6378137.0
	//EarthData::GetInstance()->GetEarthManipulator()->getSettings()->setArcViewpointTransitions(true);
	_pSkyNode = osgEarth::Util::SkyNode::create(_pMapNode);
	_pSkyNode->setName("SkyNode");
	_pSkyNode->setMoonVisible(true);
	_pSkyNode->setStarsVisible(true);
	_pSkyNode->setAtmosphereVisible(true);
	osgEarth::Util::Ephemeris* ephemeris = new osgEarth::Util::Ephemeris;
	_pSkyNode->setEphemeris(ephemeris);
	_pSkyNode->attach(EarthData::GetInstance()->GetEarthViewer(), 0);
	_pSkyNode->setLighting(true);
	_pSkyNode->addChild(_pMapNode);
	//_pSkyNode->getOrCreateStateSet()->setMode(GL_DEPTH, osg::StateAttribute::ON);
	EarthData::GetInstance()->GetEarthRoot()->addChild(_pSkyNode);
	
	/*osg::ref_ptr<osg::Light> light = new osg::Light;
	osg::Vec4 vec1(255 / 255.0f, 248 / 255.0f, 220 / 255.0f, 1.0f);
	osg::Vec4 vec2(156 / 255.0f, 156 / 255.0f, 156 / 255.0f, 1.0f);
	light->setDiffuse(vec2);	light->setAmbient(vec1);
	light->setPosition(osg::Vec4(118.7487498, 31.969264, 2000, 1.0f));
	EarthData::GetInstance()->GetEarthViewer()->setLight(light);
	EarthData::GetInstance()->GetEarthViewer()->setLightingMode(osg::View::LightingMode::HEADLIGHT);*/
}

void EarthSimExec::initGraticule()
{
	osgEarth::Util::GeodeticGraticuleOptions op;
	op.color() = osgEarth::Color("#A0E74E");
	//op.lineWidth() = 0.1;
	op.labelColor() = osgEarth::Color("#A0E74E");

	_pGraticule = new osgEarth::Util::GeodeticGraticule(op);
	_pGraticule->setOpacity(0.2);
	_pGraticule->setVisible(false);
	_pMapNode->getMap()->addLayer(_pGraticule);
}

void EarthSimExec::initWorldBoundaries()
{
	osgEarth::Symbology::Style labelstyle;
	osgEarth::Symbology::TextSymbol* text = labelstyle.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	text->font() = "simhei.ttf";
	text->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
	text->content() = osgEarth::Symbology::StringExpression("[name]");
	text->size() = 16.0f;
	text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	text->fill()->color() = osgEarth::Color("#01E5FF");
	//text->halo()->color() = osgEarth::Color("#FFF901");
	osgEarth::Symbology::LineSymbol* line = labelstyle.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	line->stroke()->color() = osgEarth::Color("#01E5FF");
	line->stroke()->width() = 1.0;
	//osgEarth::Symbology::AltitudeSymbol* altitude = labelstyle.getOrCreateSymbol<osgEarth::Symbology::AltitudeSymbol>();
	//altitude->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	//altitude->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_GPU;
	//altitude->verticalOffset() = 0.1;
	osgEarth::Drivers::OGRFeatureOptions featureOptions;
	featureOptions.url() = "../resource/EarthSim/EarthData/shpFile/world/line_world.shp";
	featureOptions.buildSpatialIndex() = true;

	osgEarth::Drivers::FeatureGeomModelOptions geomOptions;
	geomOptions.featureOptions() = featureOptions;
	geomOptions.styles() = new osgEarth::StyleSheet();
	geomOptions.styles()->addStyle(labelstyle);
	geomOptions.minRange() = 6378137.0*2;
	_pNationLayer = new osgEarth::ModelLayer("world", geomOptions);
	_pMapNode->getMap()->addModelLayer(_pNationLayer);

}
void EarthSimExec::initProvinceBoundaries()
{
	osgEarth::Symbology::Style labelstyle;
	osgEarth::Symbology::TextSymbol* text = labelstyle.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	text->font() = "simhei.ttf";
	text->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
	text->content() = osgEarth::Symbology::StringExpression("[name]");
	text->size() = 16.0f;
	text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	text->fill()->color() = osgEarth::Color("#01E5FF");
	//text->halo()->color() = osgEarth::Color("#FFF901");
	osgEarth::Symbology::LineSymbol* line = labelstyle.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	line->stroke()->color() = osgEarth::Color("#FFC601");
	line->stroke()->width() = 1.0;
	//osgEarth::Symbology::AltitudeSymbol* altitude = labelstyle.getOrCreateSymbol<osgEarth::Symbology::AltitudeSymbol>();
	//altitude->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	//altitude->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_GPU;
	//altitude->verticalOffset() = 0.1;
	osgEarth::Drivers::OGRFeatureOptions featureOptions;
	featureOptions.url() = "../resource/EarthSim/EarthData/shpFile/china/china.shp";
	featureOptions.buildSpatialIndex() = true;

	osgEarth::Drivers::FeatureGeomModelOptions geomOptions;
	geomOptions.featureOptions() = featureOptions;
	geomOptions.styles() = new osgEarth::StyleSheet();
	geomOptions.styles()->addStyle(labelstyle);
	geomOptions.minRange() = 100000 ;
	geomOptions.maxRange() = 6378137.0 * 2;
	_pProvinceLayer = new osgEarth::ModelLayer("china", geomOptions);
	_pMapNode->getMap()->addModelLayer(_pProvinceLayer);
	initProvinceCapitalCity();
}
void EarthSimExec::initProvinceCapitalCity()
{
	osgEarth::Symbology::Style labelstyle;
	osgEarth::Symbology::TextSymbol* text = labelstyle.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	text->font() = "simhei.ttf";
	text->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
	text->content() = osgEarth::Symbology::StringExpression("[name]");
	text->size() = 16.0f;
	text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	text->fill()->color() = osgEarth::Color("#01E5FF");
	//text->halo()->color() = osgEarth::Color("#FFF901");
	//osgEarth::Symbology::LineSymbol* line = labelstyle.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	//line->stroke()->color() = osgEarth::Color("#01E5FF");
	//line->stroke()->width() = 1.0;
	//osgEarth::Symbology::AltitudeSymbol* altitude = labelstyle.getOrCreateSymbol<osgEarth::Symbology::AltitudeSymbol>();
	//altitude->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	//altitude->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_GPU;
	//altitude->verticalOffset() = 0.1;
	osgEarth::Drivers::OGRFeatureOptions featureOptions;
	featureOptions.url() = "../resource/EarthSim/EarthData/shpFile/china/point_sheng.shp";
	featureOptions.buildSpatialIndex() = true;

	osgEarth::Drivers::FeatureGeomModelOptions geomOptions;
	geomOptions.featureOptions() = featureOptions;
	geomOptions.styles() = new osgEarth::StyleSheet();
	geomOptions.styles()->addStyle(labelstyle);
	geomOptions.minRange() = 100000;
	geomOptions.maxRange() = 6378137.0 * 2;
	_pMapNode->getMap()->addModelLayer(new osgEarth::ModelLayer("provinceCapital", geomOptions));
}
void EarthSimExec::initShiBoundaries()
{
	osgEarth::Symbology::Style labelstyle;
	osgEarth::Symbology::TextSymbol* text = labelstyle.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	text->font() = "simhei.ttf";
	text->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
	text->content() = osgEarth::Symbology::StringExpression("[name]");
	text->size() = 16.0f;
	text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	text->fill()->color() = osgEarth::Color("#01E5FF");
	//text->halo()->color() = osgEarth::Color("#FFF901");
	osgEarth::Symbology::LineSymbol* line = labelstyle.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	line->stroke()->color() = osgEarth::Color("#FFC601");
	line->stroke()->width() = 1.0;
	//osgEarth::Symbology::AltitudeSymbol* altitude = labelstyle.getOrCreateSymbol<osgEarth::Symbology::AltitudeSymbol>();
	//altitude->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	//altitude->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_GPU;
	//altitude->verticalOffset() = 0.1;
	osgEarth::Drivers::OGRFeatureOptions featureOptions;
	featureOptions.url() = "../resource/EarthSim/EarthData/shpFile/china/line_shi.shp";
	featureOptions.buildSpatialIndex() = true;

	osgEarth::Drivers::FeatureGeomModelOptions geomOptions;
	geomOptions.featureOptions() = featureOptions;
	geomOptions.styles() = new osgEarth::StyleSheet();
	geomOptions.styles()->addStyle(labelstyle);
	geomOptions.minRange() = 100000;
	geomOptions.maxRange() = 6378137.0;
	_pShiLayer = new osgEarth::ModelLayer("shi", geomOptions);
	_pMapNode->getMap()->addModelLayer(_pShiLayer);
}
void EarthSimExec::initCountyBoundaries()
{
	osgEarth::Symbology::Style labelstyle;
	osgEarth::Symbology::TextSymbol* text = labelstyle.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	text->font() = "simhei.ttf";
	text->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
	text->content() = osgEarth::Symbology::StringExpression("[name]");
	text->size() = 16.0f;
	text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	text->fill()->color() = osgEarth::Color("#01E5FF");
	//text->halo()->color() = osgEarth::Color("#FFF901");
	osgEarth::Symbology::LineSymbol* line = labelstyle.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
	line->stroke()->color() = osgEarth::Color("#FFC601");
	line->stroke()->width() = 1.0;
	//osgEarth::Symbology::AltitudeSymbol* altitude = labelstyle.getOrCreateSymbol<osgEarth::Symbology::AltitudeSymbol>();
	//altitude->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	//altitude->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_GPU;
	//altitude->verticalOffset() = 0.1;
	osgEarth::Drivers::OGRFeatureOptions featureOptions;
	featureOptions.url() = "../resource/EarthSim/EarthData/shpFile/china/line_xian.shp";
	featureOptions.buildSpatialIndex() = true;

	osgEarth::Drivers::FeatureGeomModelOptions geomOptions;
	geomOptions.featureOptions() = featureOptions;
	geomOptions.styles() = new osgEarth::StyleSheet();
	geomOptions.styles()->addStyle(labelstyle);
	geomOptions.minRange() = 1000;
	geomOptions.maxRange() = 100000;
	_pXianLayer = new osgEarth::ModelLayer("xian", geomOptions);
	_pMapNode->getMap()->addModelLayer(_pXianLayer);
}
void EarthSimExec::onActionGraticuleClicked()
{
	_pGraticule->setVisible(!_pGraticule->getVisible());
}
void EarthSimExec::onActionXianLineClicked()
{
	_pXianLayer->setVisible(!_pXianLayer->getVisible());
}
void EarthSimExec::onActionShiLineClicked()
{
	_pShiLayer->setVisible(!_pShiLayer->getVisible());
}
void EarthSimExec::onActionProvinceLineClicked()
{
	_pProvinceLayer->setVisible(!_pProvinceLayer->getVisible());
}
void EarthSimExec::onActionNationLineClicked()
{
	_pNationLayer->setVisible(!_pNationLayer->getVisible());
}
void EarthSimExec::onActionImportModelClicked()
{
	QString filePath = QFileDialog::getOpenFileName(_pMainUi, QString::fromLocal8Bit("选择模型文件"), QDir::currentPath(), "*.ive;;*.osg");
	if (!filePath.isEmpty())
	{
		loadModelOnEarth(filePath);
	}
}
void EarthSimExec::loadModelOnEarth(QString &filePath)
{
	osg::ref_ptr<osg::Node> airport = osgDB::readNodeFile(filePath.toStdString());
	if (airport)
	{
		airport->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
		osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;
		csn->setEllipsoidModel(new osg::EllipsoidModel());

		osg::ref_ptr<osg::MatrixTransform> mtAirport = new osg::MatrixTransform;
		mtAirport->addChild(airport);
		EarthData::GetInstance()->GetEarthOtherRoot()->addChild(mtAirport);

		osg::Matrixd mtTemp;
		csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.3762), osg::DegreesToRadians(109.1263), 360, mtTemp);
		mtAirport->setMatrix(mtTemp);
		flyToEarthPosition(109.1263, 34.3762, 0.0);
		/*flyAirport = osgDB::readNodeFile("../../../builder/data/Module/fly.ive");
		mtFlySelf = new osg::MatrixTransform;
		mtFlySelf->setMatrix(osg::Matrixd::scale(10, 10, 10)* osg::Matrixd::rotate(-1.57 / 2, osg::Vec3(0, 0, 1)));
		mtFlySelf->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
		mtFlySelf->addChild(flyAirport);

		mtfly = new osg::MatrixTransform;
		mtfly->addChild(mtFlySelf);
		mRoot->addChild(mtfly);

		csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.3834), osg::DegreesToRadians(109.1347), 437, mtTemp);
		mtfly->setMatrix(mtTemp);*/
		osg::ref_ptr<osgParticle::PrecipitationEffect> pe = new osgParticle::PrecipitationEffect;
		pe->snow(0.2);
		pe->rain(0.9);
		pe->setWind(osg::Vec3d(0, 0, 0));
		pe->setPosition(osg::Vec3(34.3762, 109.1263, 0));
		EarthData::GetInstance()->GetEarthOtherRoot()->addChild(pe);
	}
}
void EarthSimExec::flyToEarthPosition(double lon, double lat, double hei)
{
	EarthData::GetInstance()->GetEarthManipulator()->setViewpoint(osgEarth::Viewpoint("",lon, lat, 0, 0, -45, hei), 2);
}
void EarthSimExec::onActionRainClicked()
{
	osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;
	csn->setEllipsoidModel(new osg::EllipsoidModel());
	osg::Matrixd mtTemp;
	csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.3762), osg::DegreesToRadians(109.1263), 360, mtTemp);
	osg::Matrixd matrixEffect;
	matrixEffect.makeTranslate(osg::Vec3d(osg::DegreesToRadians(34.3762), osg::DegreesToRadians(109.1263), 0.0));

	// 设置粒子位置
	osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
	// 对粒子范围进行了放大
	trans->setMatrix(matrixEffect * osg::Matrixd::scale(100, 100, 100));

	// 创建雨粒子
	osgParticle::PrecipitationEffect* pe = new osgParticle::PrecipitationEffect;
	pe->rain(1.0);
	pe->setUseFarLineSegments(true);
	// iLevel参数是一个int值，表示雨的级别，一般1-10就够用了
	pe->setParticleSize(0.02);
	// 设置颜色
	pe->setParticleColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	trans->addChild(pe);
	EarthData::GetInstance()->GetEarthRoot()->addChild(trans);
	flyToEarthPosition(109.1263, 34.3762, 0.0);
}
void EarthSimExec::onActionFogClicked()
{
	// Setup a Fog state attribute    
	osg::Fog* fog = new osg::Fog;
	fog->setColor(EarthData::GetInstance()->GetEarthViewer()->getCamera()->getClearColor());
	fog->setDensity(0.02);
	fog->setMode(osg::Fog::LINEAR);
	fog->setStart(5.0);
	fog->setEnd(50.0);
	EarthData::GetInstance()->GetEarthRoot()->getOrCreateStateSet()->setAttributeAndModes(fog, osg::StateAttribute::ON);

	// Attach the FogCallback to keep the Fog uniforms up to date.
	//fog->setUpdateCallback(new osgEarth::Util::FogCallback());
}
void EarthSimExec::onActionWindClicked()
{

}