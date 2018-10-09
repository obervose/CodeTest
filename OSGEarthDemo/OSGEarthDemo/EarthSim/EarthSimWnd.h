#ifndef EARTHSIMWND_H
#define EARTHSIMWND_H

#include <QtWidgets/QMainWindow>
#include "ui_EarthSimWnd.h"
#include "EarthSimExec.h"
class EarthSimWnd : public QMainWindow
{
	Q_OBJECT

public:
	EarthSimWnd(QWidget *parent = 0);
	~EarthSimWnd();
private:
	void initAction();
	void initMenu();
	void initConnection();
private:
	Ui::EarthSimWndClass ui;
	EarthSimExec* _pExec;
	QAction* _pActionGraticule, *_pActionXianLine, *_pActionShiLine, *_pActionProvinceLine, *_pActionNationLine;
	QAction* _pActionImportModel;
	QAction* _pActionRain,*_pActionFog,*_pActionWind;
	QMenu *_pMenuOption;
	QMenu *_pMenuModel;
	QMenu *_pMenuEnvironment;
};

#endif // EARTHSIMWND_H
