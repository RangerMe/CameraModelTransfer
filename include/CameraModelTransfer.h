#ifndef __DEFINE_CAMERA_MODEL_TRANSFER__
#define __DEFINE_CAMERA_MODEL_TRANSFER__
#include "config.h"
#include "common.h"
typedef struct _CFG_CMT
{
    _CFG_CMT()
    {
        _help = "false";
        _path_to_ori_model = "NULL";
        _path_to_save_model = "NULL";
        _target_model_type = "NULL";
        _show_offset = "false";
        _log_level = 0;
        _curve_size = 1001;
        _curve_step = 0.1;
    }
    string _help;
    string _path_to_ori_model;
    string _path_to_save_model;
    string _target_model_type;
    string _show_offset;
    int _log_level;
    int _curve_size;
    float _curve_step;
}CFG_CMT;

/**
* @brief print help message
* @return void return
*/
static void printHelp();
/**
* @brief load camera int data from specific path
* @param path [in] path to config file
* @return config data structure
*/
static CamInt loadCamInts(const char* path);

/**
* @brief transfer camera model
* @param type [in] target camera model type, align with [TargetCameraModel]
* @param cam  [in] origin cam ints
* @return pointer to result
*/
static void* modelTransfer(int32_t type, CamInt* cam);

/**
* @brief save calculated model
* @param path  [in] save path
* @param model [in] calculated model
* @param type  [in] target camera model type, align with [TargetCameraModel]
* @return pointer to result
*/
static void modelSave(const char* path, void*model, int32_t type);
#endif