#include "common.h"

/**
* @brief find R(radius) from A (angle) in LUT
* @param theta [in]  target theta, in radius
* @param cam   [cam] camera model
* @return found radius
*/
float32_t findRfromA(float32_t theta, CamInt* cam)
{
	float32_t radius = 0.0F;
	float32_t* pLut = cam->dCurve;
	if (theta >= 0 && theta < (cam->dCurveSize-1)*cam->dStep*DEG2RAD)
	{
		/* Transfer theta to index */
		int32_t idxB = int(theta / (cam->dStep*DEG2RAD)) - 1;

		if (idxB < cam->dCurveSize - 1)
		{
			int32_t idxU = idxB + 1;

			float32_t rB = *(pLut + idxB*2 + 1);
			float32_t rU = *(pLut + idxU*2 + 1);
			float32_t thetaB = (idxB + 1)*(cam->dStep*DEG2RAD);
			float32_t thetaU = (idxU + 1)*(cam->dStep*DEG2RAD);
			radius = ((theta - thetaB)*rU + (thetaU - theta)*rB) / (cam->dStep*DEG2RAD);
		}
		else
		{
			float32_t rB = *(pLut + idxB*2 + 1);
			radius = rB;
		}
	}
	else if (theta<0)
	{
		radius = 0.0F;
	}
	else
	{
		radius = *(pLut + (cam->dCurveSize - 1)*2 + 1);
	}
	return radius;
}

/**
* @brief find A (angle) from R(radius) in LUT
* @param radius [in]  target radius
* @param cam    [cam] camera model
* @return found radius
*/
float32_t findAfromR(float32_t radius, CamInt* cam)
{
	float32_t theta = 0.0F;
	float32_t* pLut = cam->dCurve;
	if (radius < 0)
	{
		theta = 0.0F;
	}
	else if (radius > *(pLut + (cam->dCurveSize - 1))*2 + 1)
	{
		theta = (float32_t)((cam->dCurveSize - 1)*cam->dStep*DEG2RAD);
	}
	else
	{
		//the following is the tranditional half size searching.
		CFlags flagFind = CFALSE;
		int32_t idxTop = 0;
		int32_t idxBottom = cam->dCurveSize;
		int32_t idxMid = 0;
		int32_t idxFind = 0;
		while ((CFALSE == flagFind) && (idxTop <= idxBottom))
		{

			idxMid = (idxTop + idxBottom) / 2;
			if (radius >= *(pLut + idxMid*2 + 1) && radius < *(pLut + (idxMid + 1)*2 + 1))
			{
				idxFind = idxMid;
				flagFind = CTRUE;
			}
			else
			{
				if (radius < *(pLut + (idxMid + 1) * 2 + 1))
				{
					idxBottom = idxMid - 1;
				}
				else
				{
					idxTop = idxMid + 1;
				}
			}
		}
		//find, interpolate
		float32_t thetaB = (float32_t)((idxFind + 1)*cam->dStep*DEG2RAD);
		float32_t thetaU = (float32_t)((idxFind + 2)*cam->dStep*DEG2RAD);
		theta = ((radius - *(pLut + idxFind * 2 + 1))*thetaU + (*(pLut + (idxFind + 1) * 2 + 1) - radius)*thetaB) / 
			(*(pLut + (idxFind + 1) * 2 + 1) - *(pLut + idxFind * 2 + 1));
	}
	return theta;
}