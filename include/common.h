#ifndef __DEFINE_COMMON__
#define __DEFINE_COMMON__
#include "matrix.h"
#include <math.h>
#include <vector>
#include <memory.h>

#define PI				(3.14159265358979323846264)
#define DEG2RAD			(PI/180.0F)

#define MAX(x, y) ((x) >= (y)? (x):(y))
#define MIN(x, y) ((x) > (y)?(y):(x))

#define CLOG_E(format, ...)                                     \
{                                                               \
	printf("[Error %d] - " format "", __LINE__,##__VA_ARGS__);  \
}
#define CLOG_I(vb,format, ...)                                     				\
{                                                               				\
	if(vb <= LOG_LEVEL){														\
		printf("[Info %d] - " format "", __LINE__,##__VA_ARGS__);   \
	}																			\
}

#define DEFAULT_CURVE_SIZE (1001)
#define DEFAULT_CURVE_STEP (0.1)

enum CameraModel
{
	UNIVERSAL = 0,
	KANNALA_BRANDT			/* Kannala Brandt model */
};

enum CFlags
{
	CTRUE = 0,
	CFALSE
};

typedef signed char		   int8_t;
typedef unsigned char      uint8_t;
typedef signed short       int16_t;
typedef unsigned short     uint16_t;
typedef signed int         int32_t;
typedef unsigned int       uint32_t;
typedef float              float32_t;
typedef double             float64_t;

typedef struct _CamInt
{
	int32_t imgH;					/* Image height, in pixel */
	int32_t imgW;					/* Image width, in pixel */
	float32_t cu;					/* Optic center, u, in pixel */
	float32_t cv;					/* Optic center, v, in pixel */
	float32_t fu;					/* Fov, angle, in rad , u */
	float32_t fv;					/* Fov, angle, in rad , v */
	float32_t c;					/* Skew c */
	float32_t d;					/* Skew d */
	float32_t e;					/* Skew e */
	float32_t dStep;				/* Disortion curve type */
	int32_t dCurveSize;				/* Disortion curve size */
	float32_t *dCurve;				/* Disortion curve points */
}CamInt;

/**
* @brief find R(radius) from A (angle) in LUT
* @param theta [in]  target theta
* @param cam   [cam] camera model
* @return found radius
*/
float32_t findRfromA(float32_t theta, CamInt* cam);

/**
* @brief find A (angle) from R(radius) in LUT
* @param radius [in]  target radius
* @param cam    [cam] camera model
* @return found radius
*/
float32_t findAfromR(float32_t radius, CamInt* cam);
#endif