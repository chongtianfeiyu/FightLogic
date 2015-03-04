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
	std::string png;		//��Ӧ��ͼƬ
	int nAttack;			//����
	int nMapHp;				//Ѫ��
	int nAttackEffect;		//����Ч��
	float fAttackAddtion;	//�����˺�
	int nSkillEffect;		//����Ч��
	float fEffectAddtion;	//���ܸ���
	std::string skillPath;	//����ͼƬ
};


class SGRole
{
public:
	SGRole();
	~SGRole();
	
	SGRole * clone();				//��¡һ��
	int  GetMapHp();
	int  GetAttack();			//����
	bool IsDie();				//�Ƿ�����
	int	 AddHp(int nHp);		//����ʣ��Ѫ��
	std::string GetName();
	
	void RandomRoleData(int index);		//������

	CC_SYNTHESIZE(int,m_nAttackSeq,AttackSeq);				//��ս���Ĺ���λ��,hero 1-6 enemy 7-12
	CC_SYNTHESIZE(BaseRoleData*,m_pBaseData,BaseRoleData);	//��������
	CC_SYNTHESIZE(int,m_nCurHp,CurHp);						//��ǰѪ��
//private:
//	int m_nAttackSeq;				//��ս���Ĺ���λ��,hero 1-6 enemy 7-12
//	BaseRoleData * m_pBaseData;		//��������
//	int m_nCurHp;					//hp
};



class JsonHelp
{
public:
	JsonHelp();
	~JsonHelp();

	static JsonHelp * shareJsonHelp();

	void purge();

	unsigned char * getFileData(const char* szFile,const char * szMode,unsigned long * size);		//��ȡ�ļ�

	void LoadAllFile();									//������Ҫ���ļ�
	
	void LoadBaseRoleJsonFile(const char* szFile);		//json file

	BaseRoleData * GetBaseRoleData(int index);			//role

	BaseRoleData * RandomBaseRoleData();
public:
	std::vector<BaseRoleData*>  BaseRoleVector;	
};

#endif