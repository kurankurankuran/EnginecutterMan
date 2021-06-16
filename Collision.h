#include "main.h"
#pragma once

int CollisionCircleToCircle(XMFLOAT3, XMFLOAT3,float,float);
int CollisionRectToPoint(XMFLOAT3 f3Rect, XMFLOAT3 f3Scal, XMFLOAT3 f3Point);
int CollisionRect(float fAx, float fAy, float fAz,
	float fAw, float fAh, float fAd, float fBx, float fBy, float fBz,
	float fBw, float fBh, float fBd);
