#include "EarthSimWnd.h"

EarthSimWnd::EarthSimWnd(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle(QString::fromLocal8Bit("���ֵ���չʾ"));
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
	_pActionGraticule = new QAction(QString::fromLocal8Bit("��γ��"));
	_pActionXianLine = new QAction(QString::fromLocal8Bit("�ؽ���"));
	_pActionShiLine = new QAction(QString::fromLocal8Bit("�н���"));
	_pActionProvinceLine = new QAction(QString::fromLocal8Bit("ʡ����"));
	_pActionNationLine = new QAction(QString::fromLocal8Bit("������"));

	_pActionImportModel = new QAction(QString::fromLocal8Bit("����ģ��"));

	_pActionRain = new QAction(QString::fromLocal8Bit("��"));
	_pActionFog = new QAction(QString::fromLocal8Bit("��"));
	_pActionWind = new QAction(QString::fromLocal8Bit("��"));
}
void EarthSimWnd::initMenu()
{
	_pMenuOption = new QMenu(QString::fromLocal8Bit("����ѡ��"));
	_pMenuOption->addAction(_pActionGraticule);
	_pMenuOption->addAction(_pActionXianLine);
	_pMenuOption->addAction(_pActionShiLine);
	_pMenuOption->addAction(_pActionProvinceLine);
	_pMenuOption->addAction(_pActionNationLine);
	this->menuBar()->addMenu(_pMenuOption);

	_pMenuModel = new QMenu(QString::fromLocal8Bit("ģ��ѡ��"));
	_pMenuModel->addAction(_pActionImportModel);
	this->menuBar()->addMenu(_pMenuModel);

	_pMenuEnvironment = new QMenu(QString::fromLocal8Bit("����ѡ��"));
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