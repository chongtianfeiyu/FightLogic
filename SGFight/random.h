#ifndef _RANDOM_H_
#define _RANDOM_H_
#include <windows.h>


class CRandom
{
public:
	CRandom();
	~CRandom();
	
	void  Random_Seed (int seed);
	int   Random_Int  (int min, int max);
	float Random_Float(float min, float max);
	int	  RandomProb(int* Prob, unsigned int ProbCount);//取概率，Prob指向INI数组存放各概率值，ProbCount为总数目
	
private:
	unsigned int     m_wSeed;
	CRITICAL_SECTION m_xSectionSeed;
};

#endif