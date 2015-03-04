#ifndef SG_DATA_H_
#define SG_DATA_H_

#include <iostream>
#include <vector>

#ifndef CC_SYNTHESIZE
#define CC_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType Get##funName(void) const { return varName; }\
public: virtual void Set##funName(varType var){ varName = var; }
#endif

#ifndef CC_SYNTHESIZE_READONLY
#define CC_SYNTHESIZE_READONLY(varType, varName, funName)\
private: varType varName;\
public: virtual varType Get##funName(void) const { return varName; }
#endif

#ifndef CC_SAFE_DELETE
#define CC_SAFE_DELETE(p) if (p){ delete p; p = NULL;}
#endif

#ifndef CC_SAFE_DELETE_ARRAY
#define CC_SAFE_DELETE_ARRAY(p) if (p){ delete [] p; p = NULL;}
#endif

#ifndef NULL
#define NULL 0
#endif

struct BaseRoleData
{
	int idx;
	std::string name;
	std::string png;		//对应的图片
	int nAttack;			//攻击
	int nMapHp;				//血量
	int nAttackEffect;		//攻击效果
	float fAttackAddtion;	//攻击伤害
	int nSkillEffect;		//技能效果
	float fEffectAddtion;	//技能附加
	std::string skillPath;	//技能图片
};


class SGRole
{
public:
	SGRole();
	~SGRole();
	
	SGRole * clone();				//克隆一个
	int  GetMapHp();
	int  GetAttack();			//攻击
	bool IsDie();				//是否死亡
	int	 AddHp(int nHp);		//返回剩余血量
	std::string GetName();
	
	void RandomRoleData(int index);		//随机玩家

	CC_SYNTHESIZE(int,m_nAttackSeq,AttackSeq);				//在战斗的攻击位置,hero 1-6 enemy 7-12
	CC_SYNTHESIZE(BaseRoleData*,m_pBaseData,BaseRoleData);	//基础数据
	CC_SYNTHESIZE(int,m_nCurHp,CurHp);						//当前血量
//private:
//	int m_nAttackSeq;				//在战斗的攻击位置,hero 1-6 enemy 7-12
//	BaseRoleData * m_pBaseData;		//基础数据
//	int m_nCurHp;					//hp
};



class JsonHelp
{
public:
	JsonHelp();
	~JsonHelp();

	static JsonHelp * shareJsonHelp();

	void purge();

	unsigned char * getFileData(const char* szFile,const char * szMode,unsigned long * size);		//读取文件

	void LoadAllFile();									//加载需要的文件
	
	void LoadBaseRoleJsonFile(const char* szFile);		//json file

	BaseRoleData * GetBaseRoleData(int index);			//role

	BaseRoleData * RandomBaseRoleData();
public:
	std::vector<BaseRoleData*>  BaseRoleVector;	
};

#endif