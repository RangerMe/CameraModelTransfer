/**
* Copyright: Yihui Yu. All rights reserved.
*
* Author: Yihui Yu
* Date: 2020 - 02 - 06
* Description: fit camera model KannalaBrandt
*/
#ifndef __DEFINE_KANNALA_BRANDT__
#define __DEFINE_KANNALA_BRANDT__
#include "common.h"

/**
* r = k1 * theta + k2 * theta^3 + k3 * theta^5 + ... + ki * theta^(2*i-1)
* k1 is fixed to 1
* i is called "order", usually 5
*/
typedef struct _CamIntKannalaBrandt
{
	int32_t imgHeight;			/* img height */
	int32_t imgWidth;			/* img width */
	uint8_t order;				/* order, number of coef used, first one is fixed to 1 */
	std::vector<float32_t> k;	/* coef, first one is fixed to 1 */
	float32_t cu;				/* optic center, u */
	float32_t cv;				/* optic center, v */
	float32_t mu;				/* number of pixels per mm, u */
	float32_t mv;				/* number of pixels per mm, v */
}CamIntKannalaBrandt;

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
CFlags fitKannalaBrandt(CamIntKannalaBrandt* targetModel, CamInt* cam, int32_t order);

/**
* @brief extract KannalaBrandt model to universal model
* r = k1 * theta + k2 * theta^3 + k3 * theta^5 + ... + ki * theta^(2*i-1)
* k1 is fixed to 1
* i is called "order", usually 5
* @param cam         [out] universal camera model
* @param targetModel [in]  model parameters
* @return success flag
*/
CFlags extractKannalaBrandt(CamInt* cam, CamIntKannalaBrandt* targetModel);

/**
* @brief calculate theta's power sum.
*        this is for fitKannalaBrandt, during the lsq process
* @param cam   [in] cam model
* @param order [in] order of kannala brandt model
* @return vector contains power sum from order 6 to 4m+2, where m is kannala brandt order
*/
static std::vector<float32_t> thetaPowerSum(CamInt* cam, int32_t order);

/**
* @brief calculate radius*theta's power sum.
*        this is for fitKannalaBrandt, during the lsq process
* @param cam   [in] cam model
* @param order [in] order of kannala brandt model
* @return vector contains power sum from order 3 to 2m+1, where m is kannala brandt order
*/
static std::vector<float32_t> radiusThetaPowerSum(CamInt* cam, int32_t order);
/**
* @brief Construct matrix A to solve AK=B, for fitKannalaBrandt
* @param thetaPower [in] calculated theta power from 3 to 4m+2, where m is kannala brandt order
* @return calculated matrix A
*/
static Matrix constructMatrixA(std::vector<float32_t> thetaPower);

/**
* @brief Construct matrix B to solve AK=B, for fitKannalaBrandt
* @param radiusThetaPower [in] calculated radius*theta power from 3 to 2m+1, where m is kannala brandt order
* @param thetaPower       [in] calculated theta power from 3 to 4m+2, where m is kannala brandt order
* @return calculated matrix B
*/
static Matrix constructMatrixB(std::vector<float32_t> radiusThetaPower, std::vector<float32_t> thetaPower);
#endif