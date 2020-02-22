/**
* Copyright: Yihui Yu. All rights reserved.
*
* Author: Yihui Yu
* Date: 2020 - 02 - 06
* Description: fit camera model KannalaBrandt
*/
#include "KannalaBrandt.h"
/**
* @brief fit KannalaBrandt model
* r = k1 * theta + k2 * theta^3 + k3 * theta^5 + ... + ki * theta^(2*i-1)
* k1 is fixed to 1
* i is called "order", usually 5
* @param targetModel [out] model parameters
* @param cam         [in]  universal camera model
* @param order       [in]  target number order, usually 5
* @return success flag
*/
CFlags fitKannalaBrandt(CamIntKannalaBrandt* targetModel, CamInt* cam, int32_t order)
{
	CFlags ret = CTRUE;
	memset(targetModel, 0, sizeof(CamIntKannalaBrandt));

	targetModel->order = order;

	targetModel->imgHeight = cam->imgH;
	targetModel->imgWidth = cam->imgW;

	targetModel->cu = cam->cu;
	targetModel->cv = cam->cv;

	float32_t ru = findRfromA(cam->fu, cam);
	float32_t rv = findRfromA(cam->fv, cam);
	targetModel->mu = targetModel->cu / ru;
	targetModel->mv = targetModel->cv / rv;

	std::vector<float32_t> thetaPow = thetaPowerSum(cam, order);
	std::vector<float32_t> radiusThetaPow = radiusThetaPowerSum(cam, order);
	Matrix A = constructMatrixA(thetaPow);
	Matrix B = constructMatrixB(radiusThetaPow, thetaPow);
	Matrix A_inv = inverse(A);
	Matrix K = A_inv*B;
	targetModel->k.push_back(1);
	for (int32_t kIdx = 0; kIdx < order - 1; kIdx++)
	{
		targetModel->k.push_back(float32_t(K[kIdx][0]));
	}
	return ret;
}

/**
* @brief extract KannalaBrandt model to universal model
* r = k1 * theta + k2 * theta^3 + k3 * theta^5 + ... + ki * theta^(2*i-1)
* k1 is fixed to 1
* i is called "order", usually 5
* @param cam         [out] universal camera model
* @param targetModel [in]  model parameters
* @return success flag
*/
CFlags extractKannalaBrandt(CamInt* cam, CamIntKannalaBrandt* targetModel)
{
	CFlags ret = CTRUE;
	//memset(cam, 0, sizeof(CamInt));
	cam->imgH = targetModel->imgHeight;
	cam->imgW = targetModel->imgWidth;
	cam->c = 1;
	cam->d = 0;
	cam->e = 0;
	cam->cu = targetModel->cu;
	cam->cv = targetModel->cv;
	cam->dCurveSize = DEFAULT_CURVE_SIZE;
	cam->dStep = DEFAULT_CURVE_STEP;
	cam->dCurve = new float[2 * DEFAULT_CURVE_SIZE];
	for (int32_t idx = 0; idx < cam->dCurveSize;idx++)
	{
		*(cam->dCurve + 2 * idx) = 0;
		*(cam->dCurve + 2 * idx) = idx*cam->dStep;
		float32_t theta = idx*cam->dStep*DEG2RAD;
		*(cam->dCurve + 2 * idx + 1) = 0.0F;
		for (int32_t kIdx = 0; kIdx < targetModel->k.size();kIdx++)
		{
			*(cam->dCurve + 2 * idx + 1) += targetModel->k[kIdx] * pow(theta, 2 * kIdx + 1);
		}
	}
	float32_t ru = targetModel->cu / targetModel->mu;
	float32_t rv = targetModel->cv / targetModel->mv;
	cam->fu = findAfromR(ru, cam);
	cam->fv = findAfromR(rv, cam);
	return CTRUE;
}

/**
* @brief calculate theta's power sum.
*        this is for fitKannalaBrandt, during the lsq process
* @param cam   [in] cam model
* @param order [in] order of kannala brandt model
* @return vector contains power sum from order 6 to 4m+2, where m is kannala brandt order
*/
static std::vector<float32_t> thetaPowerSum(CamInt* cam, int32_t order)
{
	std::vector<float32_t> ret;
	float32_t* theta = new float32_t[cam->dCurveSize];
	for (int32_t thetaIdx = 0; thetaIdx < cam->dCurveSize; thetaIdx++)
	{
		*(theta + thetaIdx) = float32_t((thetaIdx)*cam->dStep*DEG2RAD);
	}
	float32_t* thetaBuffer = new float32_t[cam->dCurveSize];
	memcpy(thetaBuffer, theta, sizeof(float32_t)*cam->dCurveSize);
	for (int32_t thetaIdx = 0; thetaIdx < cam->dCurveSize; thetaIdx++)
	{
		*(thetaBuffer + thetaIdx) *= *(theta + thetaIdx);/* theta power of 2 */
	}

	int32_t st = 3;
	int32_t ed = 4 * (order)+2;
	for (int32_t idx = st; idx < ed + 1; idx++)
	{
		float32_t summ = 0.0F;
		for (int32_t thetaIdx = 0; thetaIdx < cam->dCurveSize; thetaIdx++)
		{
			*(thetaBuffer + thetaIdx) *= *(theta + thetaIdx);/* power increase by 1 */
			summ += *(thetaBuffer + thetaIdx);
		}
		ret.push_back(summ);
	}
	delete thetaBuffer, theta;
	return ret;
}

/**
* @brief calculate radius*theta's power sum.
*        this is for fitKannalaBrandt, during the lsq process
* @param cam   [in] cam model
* @param order [in] order of kannala brandt model
* @return vector contains power sum from order 3 to 2m+1, where m is kannala brandt order
*/
static std::vector<float32_t> radiusThetaPowerSum(CamInt* cam, int32_t order)
{
	std::vector<float32_t> ret;
	float32_t* radius = new float32_t[cam->dCurveSize];
	for (int32_t thetaIdx = 0; thetaIdx < cam->dCurveSize;thetaIdx++)
	{
		*(radius + thetaIdx) = *(cam->dCurve + thetaIdx*2 +1);
	}
	float32_t* theta = new float32_t[cam->dCurveSize];
	for (int32_t thetaIdx = 0; thetaIdx < cam->dCurveSize; thetaIdx++)
	{
		*(theta + thetaIdx) = float32_t((thetaIdx)*cam->dStep *DEG2RAD);/* radius*theta power of 2 */
	}
	float32_t* thetaBuffer = new float32_t[cam->dCurveSize];
	memcpy(thetaBuffer, theta, sizeof(float32_t)*cam->dCurveSize);
	for (int32_t thetaIdx = 0; thetaIdx < cam->dCurveSize; thetaIdx++)
	{
		*(thetaBuffer + thetaIdx) *= (*(radius + thetaIdx))*(*(theta + thetaIdx));/* radius*theta power of 2 */
	}

	int32_t st = 3;
	int32_t ed = 2 * (order)+1;
	for (int32_t idx = st; idx < ed + 1; idx++)
	{
		float32_t summ = 0.0F;
		for (int32_t thetaIdx = 0; thetaIdx < cam->dCurveSize; thetaIdx++)
		{
			*(thetaBuffer + thetaIdx) *= *(theta + thetaIdx);/* power increase by 1 */
			summ += *(thetaBuffer + thetaIdx);
		}
		ret.push_back(summ);
	}
	delete thetaBuffer, theta, radius;
	return ret;
}

/**
* @brief Construct matrix A to solve AK=B, for fitKannalaBrandt
* @param thetaPower [in] calculated theta power from 3 to 4m+2, where m is kannala brandt order
* @return calculated matrix A
*/
static Matrix constructMatrixA(std::vector<float32_t> thetaPower)
{
	int32_t nRowCol = int32_t(thetaPower.size() / 4) - 1;
	Matrix A = zeros(nRowCol, nRowCol);
	for (int32_t rowIdx = 0; rowIdx < nRowCol; rowIdx++)
	{
		for (int32_t colIdx = 0; colIdx < nRowCol; colIdx++)
		{
			int32_t pw = 2 * (rowIdx + 1) + 1 + 2 * (colIdx + 1) + 1;
			A[rowIdx][colIdx] = thetaPower[pw - 3];
		}
	}
	return A;
}

/**
* @brief Construct matrix B to solve AK=B, for fitKannalaBrandt
* @param radiusThetaPower [in] calculated radius*theta power from 3 to 2m+1, where m is kannala brandt order
* @param thetaPower       [in] calculated theta power from 3 to 4m+2, where m is kannala brandt order
* @return calculated matrix B
*/
static Matrix constructMatrixB(std::vector<float32_t> radiusThetaPower, std::vector<float32_t> thetaPower)
{
	int32_t nRow = int32_t(thetaPower.size() / 4) - 1;
	Matrix B = zeros(nRow, 1);
	for (int32_t rowIdx = 0; rowIdx < nRow; rowIdx++)
	{
		int32_t pw = 2 * (rowIdx + 1) + 1;
		B[rowIdx][0] = radiusThetaPower[pw - 3] - thetaPower[pw - 2];
	}
	return B;
}