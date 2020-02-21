#include "CameraModelTransfer.h"
#include "KannalaBrandt.h"
#include <string>
CamInt* pCamIntUni = new CamInt;
int helpFlag = 0;
int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		CLOG_E("Please specify config file\n");
		return 0;
	}
	else if (argc == 2)
	{
		string wd = argv[1];
		if ((wd == "--help")||(wd == "-h"))
		{
			printHelp();
			return 0;
		}
		else
		{
			FILE* fileTest = fopen(argv[1],"r");
			if(NULL == fileTest)
			{
				CLOG_E("Could not find config file %s \n",argv[1]);
				printHelp();
				return 0;
			}
			else
			{
				fclose(fileTest);
			}
		}
	}
	else
	{
		CLOG_E("Unsupport number of inputs\n");
		printHelp();
		return 0;
	}
	CameraModel mode = UNIVERSAL;
	CONFIG cfgFile;
	cfgFile.setConfigGroup(NULL, "NULL");
	cfgFile.loadConfig(argv[1]);
	CFG_CMT cfg;
	if(CONFIG_RET_SUCCESS != cfgFile.checkCfgValue("_help","NULL","NULL"))
	{
		cfgFile.extractCfgValue(&cfg._help,"_help","NULL");
	}
	if(CONFIG_RET_SUCCESS != cfgFile.checkCfgValue("_path_to_ori_model","NULL","NULL"))
	{
		cfgFile.extractCfgValue(&cfg._path_to_ori_model,"_path_to_ori_model","NULL");
	}
	if(CONFIG_RET_SUCCESS != cfgFile.checkCfgValue("_path_to_save_model","NULL","NULL"))
	{
		cfgFile.extractCfgValue(&cfg._path_to_save_model,"_path_to_save_model","NULL");
	}
	if(CONFIG_RET_SUCCESS != cfgFile.checkCfgValue("_target_model_type","NULL","NULL"))
	{
		cfgFile.extractCfgValue(&cfg._target_model_type,"_target_model_type","NULL");
	}
	if(CONFIG_RET_SUCCESS != cfgFile.checkCfgValue("_show_offset","NULL","NULL"))
	{
		cfgFile.extractCfgValue(&cfg._show_offset,"_show_offset","NULL");
	}
	if(CONFIG_RET_SUCCESS != cfgFile.checkCfgValue("_curve_size","NULL","NULL"))
	{
		cfgFile.extractCfgValue(&cfg._curve_size,"_curve_size","NULL");
	}
	if(CONFIG_RET_SUCCESS != cfgFile.checkCfgValue("_curve_step","NULL","NULL"))
	{
		cfgFile.extractCfgValue(&cfg._curve_step,"_curve_step","NULL");
	}	
	if (cfg._help == "true")
	{
		printHelp();
	}
	if(cfg._path_to_ori_model == "NULL")
	{
		CLOG_E("Please privide original camera model file\n");
		printHelp();
		return 0;
	}
	if(cfg._target_model_type == "NULL")
	{
		CLOG_E("Please specify target model type\n");
		printHelp();
		return 0;
	}
	else if(cfg._target_model_type == "UNIVERSAL")
	{
		mode = UNIVERSAL;
	}
	else if(cfg._target_model_type == "KANNALA_BRANDT")
	{
		mode = KANNALA_BRANDT;
	}
	else
	{
		CLOG_E("Unsupport target camera model type: %s\n",cfg._target_model_type.c_str());
		printHelp();
		return 0;
	}
	
	if(cfg._path_to_save_model == "NULL")
	{
		cfg._path_to_save_model = cfg._path_to_ori_model+"_"+cfg._target_model_type;
		CLOG_I(2,"Output camera model will be saved at: \n%s\n",cfg._path_to_save_model.c_str());
	}

	*pCamIntUni = loadCamInts(cfg._path_to_ori_model.c_str());
	void* model = NULL;
	model = modelTransfer(mode, pCamIntUni);
	modelSave(cfg._path_to_save_model.c_str(), model, mode);
	delete pCamIntUni->dCurve;
	return 0;
}

/**
* @brief print help message
* @return void return
*/
static void printHelp()
{
	if (0 == helpFlag)
	{
		printf("-------------------------------------------------------\n");
		printf("Usage: ./CamTransfer configFile\n");
		printf("-------------------------------------------------------\n");
		printf("# Before you start using this tool, please prepare a \n  config contains following config terms:\n");
		printf("# _help                 print help message or not, could \n                        be [tree] or [false]\n");
		printf("# _path_to_ori_model    required, path to model to be \n                        transfered\n");
		printf("# _path_to_save_model   optional, path to save calculated \n                        model\n");
		printf("# _target_model_type    target model type, could be:\n");
		printf("#                       1. UNIVERSAL\n");
		printf("#                       2. KANNALA_BRANDT\n");
		printf("# _show_offset          show disortion curve offset or \n                        not,could be [tree] or [false]\n");
		printf("# _log_level = 0        log print level,could be:\n");
		printf("#                       0   print nothing\n");
		printf("#                       1   print result only\n");
		printf("#                       2   print everything\n");
		printf("# _curve_size           Curve size, could be [NULL] or \n                        [a number], by default, it is 1001\n");
		printf("# _curve_step = NULL    Curve step(angle), coule be [NULL] \n                        or [a number], by default, it is 0.1 degree\n");
		printf("-------------------------------------------------------\n");
		helpFlag = 1;
	}
	return;
}

/**
* @brief load camera int data from specific path
* @param path [in] path to config file
* @return config data structure
*/
static CamInt loadCamInts(const char* path)
{
	using namespace std;
	CamInt cam;
	CONFIG cfg;
	cfg.setConfigGroup(NULL, "Global");
	cfg.loadConfig(path);
	string type;
	cfg.extractCfgValue(&type,"_TYPE","Global");
	if (type == "UNIVERSAL")
	{
		cfg.extractCfgValue(&cam.imgW, "_W", "Global");
		cfg.extractCfgValue(&cam.imgH, "_H", "Global");
		cfg.extractCfgValue(&cam.c, "_C", "Global");
		cfg.extractCfgValue(&cam.d, "_D", "Global");
		cfg.extractCfgValue(&cam.e, "_E", "Global");
		cfg.extractCfgValue(&cam.fu, "_FOV_AT_CU", "Global");
		cfg.extractCfgValue(&cam.fv, "_FOV_AT_CV", "Global");
		cfg.extractCfgValue(&cam.cu, "_CU", "Global");
		cfg.extractCfgValue(&cam.cv, "_CV", "Global");
		cfg.extractCfgValue(&cam.dStep, "_DISORT_STEP", "Global");
		cfg.extractCfgValue(&cam.dCurveSize, "_DISORT_SIZE", "Global");
		cam.dCurve = new float[cam.dCurveSize * 2];
		cfg.extractCfgValue(cam.dCurve, "_DISORT", "Global");
	}
	else if (type == "KANNALA_BRANDT")
	{
		CamIntKannalaBrandt targetModel;
		cfg.extractCfgValue(&targetModel.imgWidth, "_W", "Global");
		cfg.extractCfgValue(&targetModel.imgHeight, "_H", "Global");
		cfg.extractCfgValue(&targetModel.mu, "_MU", "Global");
		cfg.extractCfgValue(&targetModel.mv, "_MV", "Global");
		cfg.extractCfgValue(&targetModel.cu, "_CU", "Global");
		cfg.extractCfgValue(&targetModel.cv, "_CV", "Global");
		int searchFlag = 1;
		int kIdx = 0;
		while (1 == searchFlag)
		{
			kIdx++;
			float32_t kValue;
			char term[256];
			sprintf(term,"_K%d",kIdx);
			CONFIG_RET_CHECK ret = cfg.extractCfgValue(&kValue,term,"Global");
			if (CONFIG_RET_SUCCESS == ret)
			{
				targetModel.k.push_back(kValue);
			}
			else
			{
				searchFlag = 0;
			}
		}
		targetModel.order = targetModel.k.size();
		extractKannalaBrandt(&cam, &targetModel);
	}
	return cam;
}

/**
* @brief transfer camera model
* @param type [in] target camera model type, align with [TargetCameraModel]
* @param cam  [in] origin cam ints
* @return pointer to result
*/
static void* modelTransfer(int32_t type, CamInt* cam)
{
	void* targetModel = NULL;
	CFlags flagSuccess = CFALSE;
	switch (type)
	{
		case UNIVERSAL:
		{
			CLOG_I(2,"Converting to UNIVERSAL ... ...\n");
			targetModel = new CamInt;
			memcpy(targetModel,cam,sizeof(CamInt));
			flagSuccess = CTRUE;
			break;
		}
		case KANNALA_BRANDT:
		{
			CLOG_I(2,"Converting to KANNALA_BRANDT ... ...\n");
			targetModel = new CamIntKannalaBrandt;
			flagSuccess = fitKannalaBrandt((CamIntKannalaBrandt*)targetModel, cam, 5);
			break;
		}
		default:CLOG_E("Unsupported camera model type!\n"); break;
	}
	if (CFALSE == flagSuccess)
	{
		CLOG_I(0,"Could not complete the transform\n");
		targetModel = NULL;
	}
	return targetModel;
}

/**
* @brief save calculated model
* @param path  [in] save path
* @param model [in] calculated model
* @param type  [in] target camera model type, align with [TargetCameraModel]
* @return pointer to result
*/
static void modelSave(const char* path, void*model, int32_t type)
{
	if (NULL == model)
	{
		CLOG_I(0,"Model calculation error, exit\n");
	}
	else
	{
		switch (type)
		{
		case UNIVERSAL:
		{
			CamInt modelSave;
			memcpy(&modelSave, model, sizeof(CamInt));
			CLOG_I(1,"Saving to %s ... ...\n", path);
			FILE* file2Save = fopen(path, "w+");
			fprintf(file2Save, "# Following are all parameters required to describe a camera's intrinsic\n");
			fprintf(file2Save, "# _TYPE             parameter type\n");
			fprintf(file2Save, "# _W                image width, in pixel\n");
			fprintf(file2Save, "# _H                image height, in pixel\n");
			fprintf(file2Save, "# _CU               optic center, u, in pixel\n");
			fprintf(file2Save, "# _CV               optic center, v, in pixel\n");
			fprintf(file2Save, "# _C                skew parameter, c\n");
			fprintf(file2Save, "# _D                skew parameter, d\n");
			fprintf(file2Save, "# _E                skew parameter, e\n");
			fprintf(file2Save, "# _FOV_AT_CU        fov, at optic center, u, in rad\n");
			fprintf(file2Save, "# _FOV_AT_CV        fov, at optic center, v, in rad\n");
			fprintf(file2Save, "# _DISORT_STEP      disortion angle step, by default 0.1 degree\n");
			fprintf(file2Save, "# _DISORT_SIZE      disortion curve size\n");
			fprintf(file2Save, "# _DISORT           disortion, first column is angle, in degree, second row column is radius, in mm\n");

			fprintf(file2Save, "_TYPE = UNIVERSAL\n");
			fprintf(file2Save, "_W = %d\n", modelSave.imgW);
			fprintf(file2Save, "_H = %d\n", modelSave.imgH);
			fprintf(file2Save, "_CU = %f\n", modelSave.cu);
			fprintf(file2Save, "_CV = %f\n", modelSave.cv);
			fprintf(file2Save, "_C = %f\n", modelSave.c);
			fprintf(file2Save, "_D = %f\n", modelSave.d);
			fprintf(file2Save, "_E = %f\n", modelSave.e);
			fprintf(file2Save, "_FOV_AT_CU = %f\n", modelSave.fu);
			fprintf(file2Save, "_FOV_AT_CV = %f\n", modelSave.fv);
			fprintf(file2Save, "_DISORT_STEP = %f\n", modelSave.dStep);
			fprintf(file2Save, "_DISORT_SIZE = %d\n", modelSave.dCurveSize);
			fprintf(file2Save, "_DISORT = \n");
			for (int32_t idx = 0; idx < modelSave.dCurveSize;idx++)
			{
				fprintf(file2Save, "%f %f\n", *(modelSave.dCurve + idx * 2), *(modelSave.dCurve + idx * 2+1));
			}
			fclose(file2Save);
			break;
		}
		case KANNALA_BRANDT:
		{
			CamIntKannalaBrandt modelSave;
			memcpy(&modelSave, model, sizeof(CamIntKannalaBrandt));
			CLOG_I(1,"Saving to %s ... ...\n",path);
			FILE* file2Save = fopen(path,"w+");
			fprintf(file2Save, "# Following are all parameters required to describe a camera's intrinsic\n");
			fprintf(file2Save, "# _TYPE             parameter type\n");
			fprintf(file2Save, "# _W                image width, in pixel\n");
			fprintf(file2Save, "# _H                image height, in pixel\n");
			fprintf(file2Save, "# _CU               optic center, u, in pixel\n");
			fprintf(file2Save, "# _CV               optic center, v, in pixel\n");
			fprintf(file2Save, "# _MU               unit length per pixel, u, in mm/pixel\n");
			fprintf(file2Save, "# _MV               unit length per pixel, v, in mm/pixel\n");
			fprintf(file2Save, "_TYPE = KANNALA_BRANDT\n");
			fprintf(file2Save, "_W = %d\n", modelSave.imgWidth);
			fprintf(file2Save, "_H = %d\n", modelSave.imgHeight);
			for (int32_t coefId = 0; coefId < modelSave.order; coefId++)
			{
				fprintf(file2Save, "_K%d = %f\n", coefId + 1, modelSave.k[coefId]);
			}
			fprintf(file2Save, "_CU = %f\n", modelSave.cu);
			fprintf(file2Save, "_CV = %f\n", modelSave.cv);
			fprintf(file2Save, "_MU = %f\n", modelSave.mu);
			fprintf(file2Save, "_MV = %f\n", modelSave.mv);
			/*fprintf(file2Save, "%%YAML:1.0\n");
			fprintf(file2Save, "model_type: KANNALA_BRANDT\n");
			fprintf(file2Save, "camera_name: camera_0\n");
			fprintf(file2Save, "image_width: %d\n", modelSave.imgWidth);
			fprintf(file2Save, "image_height: %d\n", modelSave.imgHeight);
			fprintf(file2Save, "projection_parameters:\n");
			for (int32_t coefId = 0; coefId < modelSave.order; coefId++)
			{
				fprintf(file2Save, "   k%d : %f\n", coefId + 1, modelSave.k[coefId]);
			}
			fprintf(file2Save, "   cu : %f\n", modelSave.cu);
			fprintf(file2Save, "   cv : %f\n", modelSave.cv);
			fprintf(file2Save, "   mu : %f\n", modelSave.mu);
			fprintf(file2Save, "   mv : %f\n", modelSave.mv);*/
			fclose(file2Save);
			break;
		}
		default:CLOG_E("Unsupported camera model type!\n"); break;
		}
	}
	return;
}