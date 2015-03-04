// SGFight.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SGData.h"
#include "SGFightLogic.h"


int _tmain(int argc, _TCHAR* argv[])
{
	JsonHelp::shareJsonHelp()->LoadAllFile();

	SGRole *pHero[6];
	SGRole *pEnemy[6];

	for (int i = 0; i < 6; ++i)
	{
		pHero[i] = new SGRole();
		pHero[i]->RandomRoleData(i+1);
	}

	for (int i = 0; i < 6; ++i)
	{
		pEnemy[i] = new SGRole();
		pEnemy[i]->RandomRoleData(i+7);
	}

	FightLogic * pFight = FightManager::shareFightManager()->CreateFight();

	if (pFight)
	{
		pFight->SetHeroInfo(pHero,6);
		pFight->SetEnemyInfo(pEnemy,6);

		int nResult =  pFight->StartBattle();
	
		if (nResult == HERO_WIN)
		{
			printf("HERO WIN!");
		}
		else if (nResult == ENEMY_WIN)
		{
			printf("ENEMY WIN!");
		}
		else 
		{
			printf("NONE WIN!");
		}
	
	}

	FightManager::shareFightManager()->RemoveFight(pFight);


	system("pause");

	return 0;
}

