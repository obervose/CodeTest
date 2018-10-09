#include "EarthSimWnd.h"

EarthSimWnd::EarthSimWnd(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle(QString::fromLocal8Bit("数字地球展示"));
	_pExec = new EarthSimExec(this);
	this->setCentralWidget(_pExec->GetViewerWgt());
	initAction();
	initMenu();
	initConnection();
}

EarthSimWnd::~EarthSimWnd()
{

}
void EarthSimWnd::initAction()
{
	_pActionGraticule = new QAction(QString::fromLocal8Bit("经纬线"));
	_pActionXianLine = new QAction(QString::fromLocal8Bit("县界限"));
	_pActionShiLine = new QAction(QString::fromLocal8Bit("市界限"));
	_pActionProvinceLine = new QAction(QString::fromLocal8Bit("省界限"));
	_pActionNationLine = new QAction(QString::fromLocal8Bit("国界线"));

	_pActionImportModel = new QAction(QString::fromLocal8Bit("导入模型"));

	_pActionRain = new QAction(QString::fromLocal8Bit("雨"));
	_pActionFog = new QAction(QString::fromLocal8Bit("雾"));
	_pActionWind = new QAction(QString::fromLocal8Bit("风"));
}
void EarthSimWnd::initMenu()
{
	_pMenuOption = new QMenu(QString::fromLocal8Bit("地球选项"));
	_pMenuOption->addAction(_pActionGraticule);
	_pMenuOption->addAction(_pActionXianLine);
	_pMenuOption->addAction(_pActionShiLine);
	_pMenuOption->addAction(_pActionProvinceLine);
	_pMenuOption->addAction(_pActionNationLine);
	this->menuBar()->addMenu(_pMenuOption);

	_pMenuModel = new QMenu(QString::fromLocal8Bit("模型选项"));
	_pMenuModel->addAction(_pActionImportModel);
	this->menuBar()->addMenu(_pMenuModel);

	_pMenuEnvironment = new QMenu(QString::fromLocal8Bit("环境选项"));
	_pMenuEnvironment->addAction(_pActionRain);
	_pMenuEnvironment->addAction(_pActionFog);
	_pMenuEnvironment->addAction(_pActionWind);
	this->menuBar()->addMenu(_pMenuEnvironment);
}
void EarthSimWnd::initConnection()
{
	connect(_pActionGraticule, SIGNAL(triggered()), _pExec, SLOT(onActionGraticuleClicked()));
	connect(_pActionXianLine, SIGNAL(triggered()), _pExec, SLOT(onActionXianLineClicked()));
	connect(_pActionShiLine, SIGNAL(triggered()), _pExec, SLOT(onActionShiLineClicked())); 
	connect(_pActionProvinceLine, SIGNAL(triggered()), _pExec, SLOT(onActionProvinceLineClicked()));
	connect(_pActionNationLine, SIGNAL(triggered()), _pExec, SLOT(onActionNationLineClicked()));

	connect(_pActionImportModel, SIGNAL(triggered()), _pExec, SLOT(onActionImportModelClicked()));

	connect(_pActionRain, SIGNAL(triggered()), _pExec, SLOT(onActionRainClicked()));
	connect(_pActionFog, SIGNAL(triggered()), _pExec, SLOT(onActionFogClicked()));
	connect(_pActionWind, SIGNAL(triggered()), _pExec, SLOT(onActionWindClicked()));
}