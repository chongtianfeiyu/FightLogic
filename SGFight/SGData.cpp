#include "SGData.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <algorithm>

static JsonHelp * m_pJsonHelp = NULL;

extern int Random_Int(int min,int max);

SGRole::SGRole()
{
	m_nAttackSeq = 0;
	m_nCurHp = 100;
	m_pBaseData = NULL;
}

SGRole::~SGRole()
{

}

int SGRole::GetMapHp()
{
	if (m_pBaseData)
	{
		return m_pBaseData->nMapHp;
	}
	else
		return 100;
}

int SGRole::GetAttack()
{
	if (m_pBaseData)
	{
		return m_pBaseData->nAttack;
	}
	else
		return 1;
}

bool SGRole::IsDie()
{
	if (m_nCurHp <= 0)
		return true;
	else
		return false;
}

int SGRole::AddHp( int nHp )
{
	if (nHp >= 0)
	{
		m_nCurHp += nHp;

		if (m_nCurHp > GetMapHp())
			m_nCurHp = GetMapHp();
	}
	else
	{
		if (abs(nHp) >= m_nCurHp)
			m_nCurHp = 0;
		else
			m_nCurHp += nHp;
	}

	return m_nCurHp;
}


void SGRole::RandomRoleData(int index)
{
	BaseRoleData *pData = JsonHelp::shareJsonHelp()->RandomBaseRoleData();
	SetBaseRoleData(pData);
	SetCurHp(GetMapHp());
	SetAttackSeq(index);		//设置位置
}

SGRole * SGRole::clone()
{
	SGRole *pRole = new SGRole();
	memcpy(pRole,this,sizeof(SGRole));

	return pRole;
}

std::string SGRole::GetName()
{
	if (m_pBaseData)
		return m_pBaseData->name;
	else
		return "";
}

void Delete(BaseRoleData*pData)
{
	CC_SAFE_DELETE(pData);
}

JsonHelp::JsonHelp()
{
	BaseRoleVector.clear();
}

JsonHelp::~JsonHelp()
{
	//释放
	for_each(BaseRoleVector.begin(),BaseRoleVector.end(),Delete);		
}

JsonHelp * JsonHelp::shareJsonHelp()
{
	if(m_pJsonHelp == NULL)
	{
		m_pJsonHelp = new JsonHelp();
	}

	return m_pJsonHelp;
}

void JsonHelp::LoadAllFile()
{
	LoadBaseRoleJsonFile("Resources/role.json");		//json数据
}

void JsonHelp::LoadBaseRoleJsonFile( const char* szFile )
{
	rapidjson::Document doc;
	unsigned long size = 0;

	unsigned char * pBuffer = getFileData(szFile,"r",&size);

	if (pBuffer == NULL)
		return;
	else
	{
		std::string loadstr((char*)pBuffer,size);

		CC_SAFE_DELETE(pBuffer);

		doc.Parse<0>(loadstr.c_str());

		//解析错误
		if (doc.HasParseError())
			return;

		//是否是数组
		if (!doc.IsArray())
			return;

		const rapidjson::Value &pArray = doc;

		for (int i = 0; i < pArray.Size(); ++i)
		{
			const rapidjson::Value &pTmp = pArray[i];

			BaseRoleData *pData = new BaseRoleData();

			pData->idx = i+1;

			pData->name = pTmp["name"].GetString();
			pData->png = pTmp["halfPath"].GetString();
			pData->skillPath = pTmp["skillPath"].GetString();
			pData->nAttack = pTmp["attack"].GetInt();
			pData->nMapHp = pTmp["hp"].GetInt();
			pData->nAttackEffect = pTmp["attackEffect"].GetInt();
			pData->fAttackAddtion = pTmp["attackAddition"].GetDouble();
			pData->nSkillEffect = pTmp["skillEffect"].GetInt();
			pData->fEffectAddtion = pTmp["skillAddition"].GetDouble();

			BaseRoleVector.push_back(pData);
		}
	}
}

unsigned char * JsonHelp::getFileData( const char* szFile,const char * szMode,unsigned long * size )
{
	if (szFile == NULL || szMode == NULL || size == NULL)
		return NULL;
	else
	{
		FILE * fp = fopen(szFile,szMode);
		if (fp == NULL)
			return NULL;
		else
		{
			//读取文件
			fseek(fp,0,SEEK_END);		//指针至于尾端。
			*size = ftell(fp);			//与文件头的偏移位置

			fseek(fp,0,SEEK_SET);		//设回开头

			unsigned char * pBuffer = new unsigned char[*size];

			*size = fread(pBuffer,sizeof(unsigned char),*size,fp);		//读取文件

			fclose(fp);

			return pBuffer;
		}
	}
}

BaseRoleData * JsonHelp::GetBaseRoleData( int index )
{
	int nCount = BaseRoleVector.size();

	if (index < nCount)
	{
		return BaseRoleVector.at(index);
	}
	else
		return NULL;
}

BaseRoleData * JsonHelp::RandomBaseRoleData()
{
	int nCount = BaseRoleVector.size();

	int nRandom = Random_Int(0,nCount-1);

	return BaseRoleVector.at(nRandom);
}

void JsonHelp::purge()
{
	//释放内存
	if(m_pJsonHelp)
	{
		CC_SAFE_DELETE(m_pJsonHelp);
	}
}






