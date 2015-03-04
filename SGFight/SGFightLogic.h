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

	void SetHeroInfo(SGRole **pHero,int nCount);						//������ҵ���Ϣ��
	void SetEnemyInfo(SGRole **pEnemy,int nCount);					//���ù������Ϣ,6��λ��

	
	int  StartBattle();						//��ʼս��

	bool HeroAttack1();						//��ʽ1:����˳��1-6��Ŀ��˳��1-6
	bool EnemyAttack1();					//
	
	bool HeroAttack2();						//��ʽ2:����˳��1-6��Ŀ��˳�����
	bool EnemyAttack2();					//

	bool HeroAttack3();						//��ʽ3:���չ̶���˳��Ŀ��Ҳ�ǰ��չ̶���˳��
	bool EnemyAttack3();					//

	
	int	 JudgeResult();						//�ж�˳��
	bool HeroAllDie();						//Ӣ��ȫ������
	bool EnemyAllDie();						//����ȫ������



	//unsigned int m_nIdx;					//ս���߼�

	//SGRole * m_pHeroArrays[6];
	//SGRole * m_pEnemyArrays[6];				//�������
	CC_SYNTHESIZE_READONLY(unsigned int,m_nIdx,FightIdx);
	//CC_SYNTHESIZE_READONLY(int,m_nCount,Count);

	int m_nAttackSeq[2*MAX_COUNT];			//����˳��
	int m_nHpList[MAX_COUNT];				//Ѫ���б�

private:
	std::vector<SGRole*> m_pHero;
	std::vector<SGRole*> m_pEnemy;
	std::vector<SGRole*> m_pDelete;
	int		m_nIndex;		//		����
	int		m_nHeroIdx;		//		hero
	int		m_nEnemyIdx;	//		enemy

	//���һ��100���غ�,�Ȳ�Ҫ����Ⱥ��
	//int m_nCount;							//ս��˳��
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