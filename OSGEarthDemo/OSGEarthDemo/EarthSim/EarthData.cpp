#include "EarthData.h"

EarthData* EarthData::_pInstance = NULL;
EarthData::EarthData()
	: _pOtherRoot(NULL)
	, _pRoot(NULL)
{

}

EarthData::~EarthData()
{

}
EarthData* EarthData::GetInstance()
{
	if (!_pInstance)
	{
		_pInstance = new EarthData;
	}
	return _pInstance;
}
void EarthData::SetEarthRoot(osg::Group *node)
{
	_pRoot = node; 
	_pOtherRoot = new osg::Group;
	if (_pOtherRoot)
	{
		_pRoot->addChild(_pOtherRoot);
	}
	
}