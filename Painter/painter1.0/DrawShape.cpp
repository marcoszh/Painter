#include "StdAfx.h"
#include "DrawShape.h"


CDrawShape::CDrawShape(void)
{
}


CDrawShape::~CDrawShape(void)
{
}

LONG CDrawShape::MIN(LONG num1, LONG num2)
{
	if(num1 > num2)
		return num2;
	return num1;
}

LONG CDrawShape::MAX(LONG num1, LONG num2)
{
	if(num1 > num2)
		return num1;
	return num2;
}