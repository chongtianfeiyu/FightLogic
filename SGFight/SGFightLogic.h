#ifndef SGFIGHTLOGIC_H_
#define SGFIGHTLOGIC_H_

#include <vector>
#include <map>
#include "SGData.h"

enum
{
	MODE1,
	MODE2,
	MODE3,
	MODE_MAX,
};

enum
{
	NULL_WIN,
	HERO_WIN,
	ENEMY_WIN,	
};


#define MAX_COUNT 100

class FightLogic
{
public:
	FightLogic(unsigned int  idx);
	~FightLogic();

	void SetHeroInfo(SGRole **pHero,int nCount);						//设置玩家的消息。
	void SetEnemyInfo(SGRole **pEnemy,int nCount);					//设置怪物的消息,6个位置

	
	int  StartBattle();						//开始战斗

	bool HeroAttack1();						//方式1:攻击顺序1-6，目标顺序1-6
	bool EnemyAttack1();					//
	
	bool HeroAttack2();						//方式2:攻击顺序1-6，目标顺序随机
	bool EnemyAttack2();					//

	bool HeroAttack3();						//方式3:按照固定的顺序，目标也是按照固定的顺序
	bool EnemyAttack3();					//

	
	int	 JudgeResult();						//判断顺序
	bool HeroAllDie();						//英雄全部死亡
	bool EnemyAllDie();						//怪物全部死亡



	//unsigned int m_nIdx;					//战斗逻辑

	//SGRole * m_pHeroArrays[6];
	//SGRole * m_pEnemyArrays[6];				//排列组合
	CC_SYNTHESIZE_READONLY(unsigned int,m_nIdx,FightIdx);
	//CC_SYNTHESIZE_READONLY(int,m_nCount,Count);

	int m_nAttackSeq[2*MAX_COUNT];			//攻击顺序
	int m_nHpList[MAX_COUNT];				//血量列表

private:
	std::vector<SGRole*> m_pHero;
	std::vector<SGRole*> m_pEnemy;
	std::vector<SGRole*> m_pDelete;
	int		m_nIndex;		//		索引
	int		m_nHeroIdx;		//		hero
	int		m_nEnemyIdx;	//		enemy

	//最多一个100个回合,先不要考虑群攻
	//int m_nCount;							//战斗顺序
};


class FightManager
{
public:
	FightManager();
	~FightManager();

	void purge();

	unsigned int GenIdx();
	static FightManager* shareFightManager();

	FightLogic * CreateFight();

	void  RemoveFight(FightLogic* pFight);
	void  RemoveFight(unsigned int nIdx);

	void  RemoveAllFight();
private:
	unsigned int m_nIdx;
	std::map<unsigned int, FightLogic*> FightMap;
};
#endif