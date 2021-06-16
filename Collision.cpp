//=============================================================================
//
// 衝突チェック  [Collision.cpp]
// Author : KAZUKI KURA
//
//=============================================================================

#include "Collision.h"

int CollisionCircleToCircle(XMFLOAT3 cir1, XMFLOAT3 cir2 ,float Rad1, float Rad2)
{
	if ((cir2.x - cir1.x)*(cir2.x - cir1.x) + (cir2.y - cir1.y)*(cir2.y - cir1.y) + (cir2.z - cir1.z)*(cir2.z - cir1.z) <= (Rad1 + Rad2)*(Rad1 + Rad2))
	{
		return 1;
	}
	
	return 0;
}

int CollisionRectToPoint(XMFLOAT3 f3Rect, XMFLOAT3 f3Scal, XMFLOAT3 f3Point)
{
	if (f3Rect.x - f3Scal.x / 2.0f<f3Point.x&&f3Rect.x + f3Scal.x / 2.0f>f3Point.x&&
		f3Rect.y - f3Scal.y / 2.0f<f3Point.y&&f3Rect.y + f3Scal.y / 2.0f>f3Point.y&&
		f3Rect.z - f3Scal.z / 2.0f<f3Point.z&&f3Rect.z + f3Scal.z / 2.0f>f3Point.z)
	{
		return 1;
	}

	return 0;
}

//=============================================================================
// 矩形同士の衝突判定
//=============================================================================
int CollisionRect(float fAx, float fAy, float fAz,
	float fAw, float fAh, float fAd, float fBx, float fBy, float fBz,
	float fBw, float fBh, float fBd)
{
	return fAx - fAw <= fBx + fBw &&
		fBx - fBw <= fAx + fAw &&
		fAy - fAh <= fBy + fBh &&
		fBy - fBh <= fAy + fAh &&
		fAz - fAd <= fBz + fBd &&
		fBz - fBd <= fAz + fAd;


}