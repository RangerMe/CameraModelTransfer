/**
* Author: Yihui Yu
* Date: 2020 - 02 - 06
* Description: Load config file
*/
#pragma once
#include <vector>
#include <string>
using namespace std;

/***********************************************************
* Enums
***********************************************************/
enum CONFIG_LOAD
{
    REQUIRED = 0,      // MUST load
    OMIT_WITH_WARNING, // NOT NECESSARY, but with warning
    OMIT               // NOT NECESSARY
};

enum CONFIG_RET_CHECK {
	CONFIG_RET_SUCCESS = 0,						// 0, return success
	CONFIG_RET_FAIL,						    // 1, return with fail flag (target function failed)
	CONFIG_RET_NOTSURE							// 2, return with not sure flag
};
/***********************************************************
* Struct
***********************************************************/
template <typename Object>
struct CONFIG_ELEM_DATA
{
	vector<vector<Object>> data;
};

typedef struct _CONFIG_ELEM
{
    string configName;					// config name
	CONFIG_ELEM_DATA<string> configValue; // value in string
} configElem;

typedef struct _CONFIG_GROUP_ELEM
{
    int                iCntConfig;
    CONFIG_LOAD        necessary;
    string             name;
    vector<configElem> vConfigList;
} configGroupElem;

typedef struct _CONFIG_GROUP
{
    int                     iCntGroup;
    vector<configGroupElem> vConfigGroup;
} configGroup;

namespace temp{
	template <typename Object> 
	CONFIG_RET_CHECK formConfig(Object* cfgValue, vector<vector<string>> cfgString)
	{
		CONFIG_RET_CHECK ret = CONFIG_RET_SUCCESS;
		/* cfgString contains config values in a matrix */
		/* when it only contains one value, we wish the output to contain one value only */
		if (1 == cfgString.size())
		{
			if (1 == cfgString[0].size())
			{
				if((typeid(Object) == typeid(int))||(typeid(Object) == typeid(unsigned int)))
				{
					*cfgValue = stoi(cfgString[0][0],nullptr);	
				}
				if((typeid(Object) == typeid(float))||(typeid(Object) == typeid(double)))
				{
					*cfgValue = stof(cfgString[0][0],nullptr);	
				}
			}
			else
			{
				for (int colIdx = 0; colIdx < cfgString[0].size();colIdx++)
				{
					if((typeid(Object) == typeid(int))||(typeid(Object) == typeid(unsigned int)))
					{
						*(cfgValue + colIdx) = stoi(cfgString[0][colIdx]);	
					}
					if((typeid(Object) == typeid(float))||(typeid(Object) == typeid(double)))
					{
						*(cfgValue + colIdx) = stof(cfgString[0][colIdx]);	
					}
				}
			}
		}
		else
		{
			for (int rowIdx = 0; rowIdx < cfgString.size(); rowIdx++)
			{
				for (int colIdx = 0; colIdx < cfgString[0].size(); colIdx++)
				{
					if((typeid(Object) == typeid(int))||(typeid(Object) == typeid(unsigned int)))
					{
						*(cfgValue + rowIdx*cfgString[rowIdx].size() + colIdx) = stoi(cfgString[rowIdx][colIdx]);	
					}
					if((typeid(Object) == typeid(float))||(typeid(Object) == typeid(double)))
					{
						*(cfgValue + rowIdx*cfgString[rowIdx].size() + colIdx) = stof(cfgString[rowIdx][colIdx]);	
					}
				}
			}
		}
		return ret;
	}
	template <> inline
	CONFIG_RET_CHECK formConfig(string* cfgValue, vector<vector<string>> cfgString)
	{
		CONFIG_RET_CHECK ret = CONFIG_RET_SUCCESS;
		/* cfgString contains config values in a matrix */
		/* string type only support one word */
		if (1 == cfgString.size())
		{
			if (1 == cfgString[0].size())
			{
				*cfgValue = cfgString[0][0];
			}
			else
			{
				ret = CONFIG_RET_FAIL;
				printf("Error! only support one-word string type config-1\n");
			}
		}
		else
		{
			ret = CONFIG_RET_FAIL;
			printf("Error! only support one-word string type config-2\n");
		}
		return ret;
	}
}
/***********************************************************
* Class
***********************************************************/
class CONFIG
{
public:
    /**
    * Constructor, load config and save to vConfigBank
    * @return no return
    */
    CONFIG();

    /**
    * DeConstructor, release all memories
    */
    ~CONFIG();

    /**
    * Set config group properties
    * @param configLoatNecessary [in] config necessary flag, indicates
    *                                 the importance of the config file
    * @oaram configGroupName     [in] config group name, could be null
    * @return void return
    */
    void setConfigGroup(CONFIG_LOAD* configLoadNecessary,const char* configGroupName);

    /**
    * Get idx th cfg group
    * @param cfgGroup [out] found config group
    * @param idx      [in]  target cfg group
    * @return stanadard return check
    */
	CONFIG_RET_CHECK getConfigGroup(configGroupElem& cfgGroup, int idx);
    /**
    * Load config file and printf necessary messages
    * @param  configPath      [in] full path to the config file
    * @return standard return check
    */
	CONFIG_RET_CHECK loadConfig(const char *configPath);

    /**
    * Find config group in config bank
    * @param vConfigList [out] found config group
    * @param groupName   [in]  target group name
    * @return standard return check
    */
	CONFIG_RET_CHECK findConfigGroup(vector<configElem>& vConfigList, const char* groupName);

	/**
	* Check whether one config term's value equals to someting
	* @param cfgName  	[in] config term name
	* @param cfgCheck   [in] target config check name
	* @param cfgGroup  [in]  config group name
	* @return return result
	*/
	CONFIG_RET_CHECK checkCfgValue(const char *cfgName, const char *cfgCheck, const char *cfgGroup);

	/**
	* Extract config value fron config bank
	* @param cfgValue  [out] extracted config values in string
	* @param cfgName   [in]  target cfg name
	* @param cfgGroup  [in]  config group name
	* @return return extracted cfg value
	*/
	CONFIG_RET_CHECK extCfgString(vector<vector<string>>& cfgValue, const char *cfgName, const char *cfgGroup);

	/**
	* Extract config value fron config bank
	* @param cfgValue  [out] extracted config values
	* @param cfgName   [in]  target cfg name
	* @param cfgGroup  [in]  config group name
	* @return return extracted cfg value
	*/
	template <typename Object>
	CONFIG_RET_CHECK extractCfgValue(Object* cfgValue, const char *cfgName, const char *cfgGroup)
	{
		CONFIG_RET_CHECK ret = CONFIG_RET_FAIL;
		vector<vector<string>> cfgString;
		ret = extCfgString(cfgString,cfgName,cfgGroup);
		if (CONFIG_RET_SUCCESS == ret)
		{
			ret = temp::formConfig(cfgValue,cfgString);
		}
		else
		{
			cfgValue = NULL;
		}
		return ret;
	}
private:
    vector<string>  vLineCommentSymbol;
    configGroup     vConfigBank;
    string          sGroupName;
    CONFIG_LOAD     configLoadNecessary;
};