/**
* Copyright: Yihui Yu. All rights reserved.
*
* Author: Yihui Yu
* Date: 2020 - 02 - 06
* Description: Load config file
*/
#include "config.h"
#include <fstream>
#include <sstream>
#include <memory.h>

/**
* Constructor, load config and save to vConfigBank
* @return no return
*/
CONFIG::CONFIG()
{
    // Support comment style "#" and "//" and "%"
    vLineCommentSymbol.push_back("#");
    vLineCommentSymbol.push_back("//");
	vLineCommentSymbol.push_back("%");

    vConfigBank.iCntGroup = 0;
    configLoadNecessary = REQUIRED;
    return;
}

CONFIG::~CONFIG()
{
    return;
}

/**
* Set config group properties
* @param configNecessary [in] config necessary flag, indicates
*                             the importance of the config file
* @param configGroup     [in] config group name, could be null
* @return void return
*/
void CONFIG::setConfigGroup(CONFIG_LOAD* configNecessary, const char *configGroup)
{
    if (NULL == configGroup)
    {
        sGroupName = "NULL";
    }
    else
    {
        sGroupName = configGroup;
    }
    if (NULL == configNecessary)
    {
        configLoadNecessary = REQUIRED;
    }
    else
    {
        configLoadNecessary = *configNecessary;
    }
    return;
}
/**
* Get idx th cfg group
* @param cfgGroup [out] found config group
* @param idx      [in]  target cfg group
* @return stanadard return check
*/
CONFIG_RET_CHECK CONFIG::getConfigGroup(configGroupElem& cfgGroup, int idx)
{
	CONFIG_RET_CHECK ret = CONFIG_RET_SUCCESS;
    if(idx >= vConfigBank.iCntGroup)
    {
		ret = CONFIG_RET_FAIL;
        printf("No config group [%d] saved !\n",idx);
    }
    else
    {
        cfgGroup = vConfigBank.vConfigGroup[idx];
    }
    return ret;
}

/**
* Load config file and printf necessary messages
* @param  configPath      [in] full path to the config file
* @return standard return check
*/
CONFIG_RET_CHECK CONFIG::loadConfig(const char *configPath)
{
	CONFIG_RET_CHECK ret = CONFIG_RET_SUCCESS;
    configGroupElem cfgTemp;
    cfgTemp.name = sGroupName;
    cfgTemp.necessary = configLoadNecessary;
    cfgTemp.iCntConfig = 0;
    ifstream configFile(configPath);
    if ((REQUIRED == configLoadNecessary) && (configFile.fail()))
    {
        ret = CONFIG_RET_FAIL;
        printf("Load config { %s } failed\n", configPath);
    }
    else if ((OMIT_WITH_WARNING == configLoadNecessary) && (configFile.fail()))
    {
        ret = CONFIG_RET_SUCCESS;
        vConfigBank.iCntGroup++;
        vConfigBank.vConfigGroup.push_back(cfgTemp);
        printf("Load config { %s } failed, omit --- \n", configPath);
    }
    else if ((OMIT == configLoadNecessary) && (configFile.fail()))
    {
        vConfigBank.iCntGroup++;
        vConfigBank.vConfigGroup.push_back(cfgTemp);
        ret = CONFIG_RET_SUCCESS;
    }
	else
	{
		string line;
		configElem cfgElemTemp;
		int finishFlag = 0;
		while (getline(configFile, line))
		{ // loop to load lines from config file
			for (int idx = 0; idx < vLineCommentSymbol.size(); idx++)
			{
				if (line.find_first_of("=") < line.find_first_of(vLineCommentSymbol[idx].c_str()))
				{/* if this line contains valid information split by "=" */
					finishFlag = 1;
					/* save current buffer */
					cfgTemp.vConfigList.push_back(cfgElemTemp);
					cfgTemp.iCntConfig++;
					/* start a new config term */
					cfgElemTemp.configValue.data.clear();
					cfgElemTemp.configName.clear();
					break;
				}
			}
			/* split line to words */
			stringstream phraseBuffer;phraseBuffer << line;
			string phrase;
			vector<string> data;
			
			int commentFlag = 0;
			int iCnt = 0;
			while (phraseBuffer >> phrase)
			{/* for each word */
				/* judge if it is a comment */
				for (int idx = 0; idx < vLineCommentSymbol.size(); idx++)
				{
					if (0 == strcmp((phrase.substr(0, vLineCommentSymbol[idx].size())).c_str(), vLineCommentSymbol[idx].c_str()))
					{
						commentFlag = 1;
						break;
					}
				}
				if (1 == commentFlag)
				{
					break;
				}
				iCnt++;
				if (1 == iCnt && phrase != "=")
				{/* if it is the first phrase (possible the config name )*/
					if (1 == finishFlag)
					{/* and we are starting a new config term */
						cfgElemTemp.configName = phrase;
					}
					else
					{/* this line contains data for last config term */
						data.push_back(phrase);
					}
					finishFlag = 0;
				}
				else if (phrase != "=")
				{
					data.push_back(phrase);
				}
			}
			if (data.size()>0)
			{
				cfgElemTemp.configValue.data.push_back(data);
			}
			
		}
		/* save config */
		cfgTemp.vConfigList.push_back(cfgElemTemp);
		cfgTemp.iCntConfig++;
		vConfigBank.iCntGroup++;
		vConfigBank.vConfigGroup.push_back(cfgTemp);
		configFile.close();
	}
	return ret;
}
/**
* Find config group in config bank
* @param vConfigList [out] found config group
* @param groupName   [in]  target group name
* @return standard return check
*/
CONFIG_RET_CHECK CONFIG::findConfigGroup(vector<configElem>& vConfigList, const char* groupName)
{
    CONFIG_RET_CHECK ret = CONFIG_RET_FAIL;
    int flagGroupExist = 0;
    for(int groupId = 0; groupId < vConfigBank.iCntGroup;groupId++)
    {
        if(0 == vConfigBank.vConfigGroup[groupId].name.compare(groupName))
        {
            flagGroupExist = 1;
            vConfigList = vConfigBank.vConfigGroup[groupId].vConfigList;
            break;
        }
        else
        {
            continue;
        }
    }
    if ((0 == flagGroupExist) && (REQUIRED == configLoadNecessary))
    {
        ret = CONFIG_RET_FAIL;
        printf("Could not find config group [%s], failed -- \n", (char*)sGroupName.c_str());
    }
    else if ((0 == flagGroupExist) && (OMIT_WITH_WARNING == configLoadNecessary))
    {
        ret = CONFIG_RET_SUCCESS;
        printf("Could not find config group [%s], omit -- \n", (char*)sGroupName.c_str());
    }
    else if ((0 == flagGroupExist) && (OMIT == configLoadNecessary))
    {
        ret = CONFIG_RET_SUCCESS;
    }
    else
    {
        ret = CONFIG_RET_SUCCESS;
    }
    return ret;
}
/**
* Check whether one config term's value equals to someting
* @param cfgName  	[in] config term name
* @param cfgCheck   [in] target config check name
* @param cfgGroup  [in]  config group name
* @return return result
*/
CONFIG_RET_CHECK CONFIG::checkCfgValue(const char *cfgName, const char *cfgCheck,const char *cfgGroup)
{
    CONFIG_RET_CHECK ret = CONFIG_RET_FAIL;
    vector<vector<string>> test;
    ret = extCfgString(test,cfgName,cfgGroup);
    if(test[0][0] == cfgCheck)
    {
        ret = CONFIG_RET_SUCCESS;
    }
    else if(ret == CONFIG_RET_FAIL)
    {
        ret = CONFIG_RET_NOTSURE;
    }
    else
    {
        ret = CONFIG_RET_FAIL;
    }
    return ret;
}
/**
* Extract config value fron config bank
* @param cfgValue  [out] extracted config values in string
* @param cfgName   [in]  target cfg name
* @param cfgGroup  [in]  config group name
* @return return extracted cfg value
*/
CONFIG_RET_CHECK CONFIG::extCfgString(vector<vector<string>>& cfgValue, const char *cfgName, const char *cfgGroup)
{
	CONFIG_RET_CHECK ret = CONFIG_RET_FAIL;
	vector<configElem> vConfigList;
	ret = findConfigGroup(vConfigList, cfgGroup);
	if (CONFIG_RET_SUCCESS == ret)
	{/* if we found target config group, then we try to find target config term */
		unsigned int matchFlag = 0;
		for (int cfgId = 0; cfgId < vConfigList.size(); cfgId++)
		{
			string cfgNameS = cfgName;
			if (0 == vConfigList[cfgId].configName.compare(cfgName))
			{/* found target config term */
				matchFlag = 1;
				for (int rowId = 0; rowId < vConfigList[cfgId].configValue.data.size(); rowId++)
				{/* read config value from the buffer */
					vector<string> col;
					for (int colId = 0; colId < vConfigList[cfgId].configValue.data[rowId].size(); colId++)
					{
						col.push_back(vConfigList[cfgId].configValue.data[rowId][colId]);
					}
					cfgValue.push_back(col);
				}
	    		break;
			}
		}
		/* Could not locate target config term */
		if ((0 == matchFlag) && (REQUIRED == configLoadNecessary))
        {
			ret = CONFIG_RET_FAIL;
			printf("Could not find config term [%s], failed -- \n", cfgName);
		}
		else if ((0 == matchFlag) && (OMIT_WITH_WARNING == configLoadNecessary))
		{
			ret = CONFIG_RET_SUCCESS;
			printf("Could not find config term [%s], omit -- \n", cfgName);
		}
		else if ((0 == matchFlag) && (OMIT == configLoadNecessary))
		{
			ret = CONFIG_RET_SUCCESS;
		}
		else
		{
			ret = CONFIG_RET_SUCCESS;
		}
	}
    return ret;
}