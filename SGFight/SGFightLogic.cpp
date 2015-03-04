#include "SGFightLogic.h"
#include <algorithm>
#include <Windows.h>


extern int Random_Int(int min,int max);

char * ConverUtf8ToGBK(const char* szSrc)
{
	int len = MultiByteToWideChar(CP_UTF8,0,szSrc,-1,NULL,0);

	WCHAR *pWchar = new WCHAR[len+1];

	MultiByteToWideChar(CP_UTF8,0,szSrc,-1,pWchar,len);

	char * pGBk = NULL;

	len = WideCharToMultiByte(CP_ACP,0,pWchar,-1,NULL,0,NULL,NULL);

	pGBk = new char[len+1];

	WideCharToMultiByte(CP_ACP,0,pWchar,-1,pGBk,len,NULL,NULL);

	delete[] pWchar;

	return pGBk;
}


static FightManager * m_pShareFightManager = NULL;

void DeleteRole(SGRole* role)
{
	CC_SAFE_DELETE(role);
}

void DeleteFight(std::map<unsigned int,FightLogic*>::value_type iter)
{
	CC_SAFE_DELETE(iter.second);
}

FightLogic::FightLogic(unsigned int idx)
:m_nIdx(idx)
{

	//m_nCount = 0;
	memset(m_nHpList,0,sizeof(int)*MAX_COUNT);
	memset(m_nAttackSeq,0,sizeof(int)*MAX_COUNT);

	m_nEnemyIdx = 0;
	m_nHeroIdx = 0;
	m_nIndex = 0;
}

FightLogic::~FightLogic()
{
	for_each(m_pDelete.begin(),m_pDelete.end(),DeleteRole);
	m_pDelete.clear();


	for_each(m_pHero.begin(),m_pHero.end(),DeleteRole);
	m_pHero.clear();


	for_each(m_pEnemy.begin(),m_pEnemy.end(),DeleteRole);
	m_pDelete.clear();
}

void FightLogic::SetHeroInfo( SGRole **pHero,int nCount )
{
	for(int i = 0; i < nCount && i < 6; ++i)
	{
		if (pHero[i])
		{
			//m_pHeroArrays[i] = pHero[i]->clone();
			m_pHero.push_back(pHero[i]->clone());
		}
	}
}

void FightLogic::SetEnemyInfo( SGRole **pEnemy,int nCount )
{
	for(int i = 0; i < nCount && i < 6; ++i)
	{
		if (pEnemy[i])
		{
			//m_pEnemyArrays[i] = pEnemy[i]->clone();
			m_pEnemy.push_back(pEnemy[i]->clone());
		}
	}
}

int FightLogic::StartBattle()
{
	DWORD dwBegin = GetTickCount();

	int nResult = NULL_WIN;

	m_nIndex = 0;

	while(m_nIndex < 2*MAX_COUNT)
	{

		//
		nResult = JudgeResult();
		if( nResult != NULL_WIN)
			break;

		if (HeroAttack2() == false)
			break;

		nResult = JudgeResult();
		if( nResult != NULL_WIN)
			break;

		//¿ªÊ¼Ë­ÏÈ¹¥»÷enemy->attack->hero
		if (EnemyAttack2() == false)
			break;

		nResult = JudgeResult();
		if( nResult != NULL_WIN)
			break;
	}

	DWORD dwEnd = GetTickCount();

	int delta = dwEnd - dwBegin;

	printf("fight cost:%d\n",delta);
	printf("‘ðôY»ØºÏ:%d\n",m_nIndex/2);

	return nResult;
}

bool FightLogic::HeroAllDie()
{
	return m_pHero.size() == 0;
}

bool FightLogic::EnemyAllDie()
{
	return m_pEnemy.size() == 0;
}

int FightLogic::JudgeResult()
{
	int nResult = NULL_WIN;

	if (HeroAllDie())
		nResult = ENEMY_WIN;

	if (EnemyAllDie())
		nResult = HERO_WIN;

	return nResult;
}

//Íæ¼Ò¹¥»÷
bool FightLogic::HeroAttack1()
{
	SGRole *pTmpHero = NULL;
	SGRole *pByAttack = NULL;

	if (m_nHeroIdx >= m_pHero.size())
		m_nHeroIdx = 0;

	//¹¥»÷
	pTmpHero = m_pHero.at(m_nHeroIdx);
	pByAttack = m_pEnemy.at(0);
	
	++m_nHeroIdx;			//ÏÂÒ»¸öÓ¢ÐÛ¹¥»÷

	if (pTmpHero && pByAttack)
	{
		m_nAttackSeq[m_nIndex++] = pTmpHero->GetAttackSeq();
		m_nAttackSeq[m_nIndex] = pByAttack->GetAttackSeq();

		int nLastCurHp = pByAttack->GetCurHp();			//ÑªÁ¿

		pByAttack->AddHp(-pTmpHero->GetAttack());			//¿Û³ýÑªÁ¿

		//ÑªÁ¿¼ÆËã
		m_nHpList[m_nIndex/2] = pByAttack->GetCurHp();

		if (pByAttack->GetCurHp() <= 0)
		{
			m_pEnemy.erase(m_pEnemy.begin());
			m_pDelete.push_back(pByAttack);
		}

		int nLoseHp = nLastCurHp - pByAttack->GetCurHp();

		printf("hero%d:%s->enemy%d:%s,enemy lose hp:%d\n",pTmpHero->GetAttackSeq(),ConverUtf8ToGBK(pTmpHero->GetName().c_str()),pByAttack->GetAttackSeq(),ConverUtf8ToGBK(pByAttack->GetName().c_str()),nLoseHp);

		return true;
	}
	else
		return false;
}

bool FightLogic::EnemyAttack1()
{
	SGRole *pTmpHero = NULL;
	SGRole *pByAttack = NULL;

	if (m_nEnemyIdx >= m_pEnemy.size())
		m_nEnemyIdx = 0;

	//¹¥»÷
	pTmpHero = m_pEnemy.at(m_nEnemyIdx);
	pByAttack = m_pHero.at(0);

	++m_nEnemyIdx;

	if (pTmpHero && pByAttack)
	{
		m_nAttackSeq[m_nIndex++] = pTmpHero->GetAttackSeq();
		m_nAttackSeq[m_nIndex] = pByAttack->GetAttackSeq();

		int nLastCurHp = pByAttack->GetCurHp();			//ÑªÁ¿

		pByAttack->AddHp(-pTmpHero->GetAttack());			//¿Û³ýÑªÁ¿

		//ÑªÁ¿¼ÆËã
		m_nHpList[m_nIndex/2] = pByAttack->GetCurHp();

		if (pByAttack->GetCurHp() <= 0)
		{
			m_pHero.erase(m_pHero.begin());
			m_pDelete.push_back(pByAttack);
		}

		int nLoseHp = nLastCurHp - pByAttack->GetCurHp();

		printf("enemy%d:%s->hero%d:%s,hero lose hp:%d\n",pTmpHero->GetAttackSeq(),ConverUtf8ToGBK(pTmpHero->GetName().c_str()),pByAttack->GetAttackSeq(),ConverUtf8ToGBK(pByAttack->GetName().c_str()),nLoseHp);

		return true;
	}
	else
		return false;
}

bool FightLogic::HeroAttack2()
{
	SGRole *pTmpHero = NULL;
	SGRole *pByAttack = NULL;

	int nCount = m_pHero.size();
	if (m_nHeroIdx >= nCount)
		m_nHeroIdx = 0;

	//¹¥»÷
	pTmpHero = m_pHero.at(m_nHeroIdx);


	int nTagertIndex = Random_Int(0,m_pEnemy.size()-1);

	pByAttack = m_pEnemy.at(nTagertIndex);

	++m_nHeroIdx;			//ÏÂÒ»¸öÓ¢ÐÛ¹¥»÷

	if (pTmpHero && pByAttack)
	{
		m_nAttackSeq[m_nIndex++] = pTmpHero->GetAttackSeq();
		m_nAttackSeq[m_nIndex] = pByAttack->GetAttackSeq();

		int nLastCurHp = pByAttack->GetCurHp();			//ÑªÁ¿

		pByAttack->AddHp(-pTmpHero->GetAttack());			//¿Û³ýÑªÁ¿

		//ÑªÁ¿¼ÆËã
		m_nHpList[m_nIndex/2] = pByAttack->GetCurHp();

		if (pByAttack->GetCurHp() <= 0)
		{
			m_pEnemy.erase(m_pEnemy.begin()+nTagertIndex);
			m_pDelete.push_back(pByAttack);
		}

		int nLoseHp = nLastCurHp - pByAttack->GetCurHp();

		printf("hero%d:%s->enemy%d:%s,enemy lose hp:%d\n",pTmpHero->GetAttackSeq(),ConverUtf8ToGBK(pTmpHero->GetName().c_str()),pByAttack->GetAttackSeq(),ConverUtf8ToGBK(pByAttack->GetName().c_str()),nLoseHp);

		return true;
	}
	else
		return false;
}

bool FightLogic::EnemyAttack2()
{
	SGRole *pTmpHero = NULL;
	SGRole *pByAttack = NULL;

	int nCount = m_pEnemy.size();
	if (m_nEnemyIdx >= nCount)
		m_nEnemyIdx = 0;

	//¹¥»÷
	pTmpHero = m_pEnemy.at(m_nEnemyIdx);

	int nTargetIndex = Random_Int(0,m_pHero.size()-1);
	pByAttack = m_pHero.at(nTargetIndex);

	++m_nEnemyIdx;

	if (pTmpHero && pByAttack)
	{
		m_nAttackSeq[m_nIndex++] = pTmpHero->GetAttackSeq();
		m_nAttackSeq[m_nIndex] = pByAttack->GetAttackSeq();

		int nLastCurHp = pByAttack->GetCurHp();			//ÑªÁ¿

		pByAttack->AddHp(-pTmpHero->GetAttack());			//¿Û³ýÑªÁ¿

		//ÑªÁ¿¼ÆËã
		m_nHpList[m_nIndex/2] = pByAttack->GetCurHp();

		if (pByAttack->GetCurHp() <= 0)
		{
			m_pHero.erase(m_pHero.begin()+nTargetIndex);
			m_pDelete.push_back(pByAttack);
		}

		int nLoseHp = nLastCurHp - pByAttack->GetCurHp();

		printf("enemy%d:%s->hero%d:%s,hero lose hp:%d\n",pTmpHero->GetAttackSeq(),ConverUtf8ToGBK(pTmpHero->GetName().c_str()),pByAttack->GetAttackSeq(),ConverUtf8ToGBK(pByAttack->GetName().c_str()),nLoseHp);

		return true;
	}
	else
		return false;
}

//°´ÕÕÌØ¶¨µÄË³Ðò¹¥»÷
bool FightLogic::HeroAttack3()
{
	SGRole *pTmpHero = NULL;
	SGRole *pByAttack = NULL;

	int nCount = m_pHero.size();
	if (m_nHeroIdx >= nCount)
		m_nHeroIdx = 0;

	//¹¥»÷
	pTmpHero = m_pHero.at(m_nHeroIdx);


	int nTagertIndex = Random_Int(0,m_pEnemy.size()-1);

	pByAttack = m_pEnemy.at(nTagertIndex);

	++m_nHeroIdx;			//ÏÂÒ»¸öÓ¢ÐÛ¹¥»÷

	if (pTmpHero && pByAttack)
	{
		m_nAttackSeq[m_nIndex++] = pTmpHero->GetAttackSeq();
		m_nAttackSeq[m_nIndex] = pByAttack->GetAttackSeq();

		int nLastCurHp = pByAttack->GetCurHp();			//ÑªÁ¿

		pByAttack->AddHp(-pTmpHero->GetAttack());			//¿Û³ýÑªÁ¿

		//ÑªÁ¿¼ÆËã
		m_nHpList[m_nIndex/2] = pByAttack->GetCurHp();

		if (pByAttack->GetCurHp() <= 0)
		{
			m_pEnemy.erase(m_pEnemy.begin()+nTagertIndex);
			m_pDelete.push_back(pByAttack);
		}

		int nLoseHp = nLastCurHp - pByAttack->GetCurHp();

		printf("hero%d:%s->enemy%d:%s,enemy lose hp:%d\n",pTmpHero->GetAttackSeq(),ConverUtf8ToGBK(pTmpHero->GetName().c_str()),pByAttack->GetAttackSeq(),ConverUtf8ToGBK(pByAttack->GetName().c_str()),nLoseHp);

		return true;
	}
	else
		return false;
}

bool FightLogic::EnemyAttack3()
{
	SGRole *pTmpHero = NULL;
	SGRole *pByAttack = NULL;

	int nCount = m_pEnemy.size();
	if (m_nEnemyIdx >= nCount)
		m_nEnemyIdx = 0;

	//¹¥»÷
	pTmpHero = m_pEnemy.at(m_nEnemyIdx);

	int nTargetIndex = Random_Int(0,m_pHero.size()-1);
	pByAttack = m_pHero.at(nTargetIndex);

	++m_nEnemyIdx;

	if (pTmpHero && pByAttack)
	{
		m_nAttackSeq[m_nIndex++] = pTmpHero->GetAttackSeq();
		m_nAttackSeq[m_nIndex] = pByAttack->GetAttackSeq();

		int nLastCurHp = pByAttack->GetCurHp();			//ÑªÁ¿

		pByAttack->AddHp(-pTmpHero->GetAttack());			//¿Û³ýÑªÁ¿

		//ÑªÁ¿¼ÆËã
		m_nHpList[m_nIndex/2] = pByAttack->GetCurHp();

		if (pByAttack->GetCurHp() <= 0)
		{
			m_pHero.erase(m_pHero.begin()+nTargetIndex);
			m_pDelete.push_back(pByAttack);
		}

		int nLoseHp = nLastCurHp - pByAttack->GetCurHp();

		printf("enemy%d:%s->hero%d:%s,hero lose hp:%d\n",pTmpHero->GetAttackSeq(),ConverUtf8ToGBK(pTmpHero->GetName().c_str()),pByAttack->GetAttackSeq(),ConverUtf8ToGBK(pByAttack->GetName().c_str()),nLoseHp);

		return true;
	}
	else
		return false;
}



FightManager* FightManager::shareFightManager()
{
	if (m_pShareFightManager == NULL)
	{
		m_pShareFightManager = new FightManager();
	}

	return m_pShareFightManager;
}



FightManager::FightManager()
{
	m_nIdx = 0;
}

FightManager::~FightManager()
{
	RemoveAllFight();
}

void FightManager::purge()
{
	CC_SAFE_DELETE(m_pShareFightManager);
}

unsigned int FightManager::GenIdx()
{
	++m_nIdx;

	if (m_nIdx == 0xFFFFFFFF)
		m_nIdx = 1;

	return m_nIdx;
}

FightLogic * FightManager::CreateFight()
{
	unsigned int nIdx = GenIdx();
	FightLogic * pFight = new FightLogic(nIdx);


	std::map<unsigned int,FightLogic*>::iterator iter = FightMap.find(nIdx);
	if (iter != FightMap.end())
	{
		iter->second = pFight;
	}
	else
		FightMap.insert(std::make_pair(nIdx,pFight));

	return pFight;
}

void FightManager::RemoveFight( FightLogic* pFight )
{
	if (pFight == NULL)
		return;

	unsigned int nIdx = pFight->GetFightIdx();

	RemoveFight(nIdx);
}

void FightManager::RemoveFight(unsigned int nIdx)
{
	std::map<unsigned int,FightLogic*>::iterator iter = FightMap.find(nIdx);
	if (iter != FightMap.end())
	{
		FightLogic *pFight = iter->second;

		FightMap.erase(iter);

		CC_SAFE_DELETE(pFight);
	}
}

void FightManager::RemoveAllFight()
{
	for_each(FightMap.begin(),FightMap.end(),DeleteFight);
	FightMap.clear();
}





