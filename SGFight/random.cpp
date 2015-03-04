#include "random.h"
#include <windows.h>


int Random_Int();
float Random_Float(float min, float max);
int RandomProb(int* Prob, unsigned int ProbCount);
int Random_Int(int min, int max);

CRandom::CRandom()
{
	InitializeCriticalSection(&m_xSectionSeed);
	m_wSeed = 0;
}

CRandom::~CRandom()
{
	DeleteCriticalSection(&m_xSectionSeed);
}

void CRandom::Random_Seed(int seed)
{
	if(!seed) 
		m_wSeed = GetTickCount();
	else 
		m_wSeed = seed;	
}

int CRandom::Random_Int(int min, int max)
{
	if(min > max)
	{
		int nTemp = max;
		min = nTemp;
		max = min;
	}
	EnterCriticalSection(&m_xSectionSeed);
	m_wSeed = 214013*m_wSeed + 2531011;
	int result = min + (m_wSeed ^ m_wSeed>>15)%(max-min+1);
	LeaveCriticalSection(&m_xSectionSeed);
	return result;
}

float CRandom::Random_Float(float min, float max)
{
	if(min > max)
	{
		float fTemp = max;
		min = fTemp;
		max = min;
	}
    EnterCriticalSection(&m_xSectionSeed);
	m_wSeed = 214013*m_wSeed + 2531011;
	float result = min + (m_wSeed>>16)*(1.0f/65535.0f)*(max-min);
	LeaveCriticalSection(&m_xSectionSeed);
	return result;
}

int CRandom::RandomProb( int* Prob, unsigned int ProbCount )
{
	if ( !Prob )  return -1;

	int n = Random_Int(1,100);
	int CurNum = 0;
	for(int i =0; i < ProbCount; i++)
	{
		CurNum += Prob[i];
		if ( n <= CurNum )
			return i;
	}
}



CRandom g_pRandom;

void Random_Seed(int seed)
{
	g_pRandom.Random_Seed(seed);
}

int Random_Int()
{
	return g_pRandom.Random_Int(0,32767);
}

int Random_Int(int min, int max)
{
	return g_pRandom.Random_Int(min,max);
}

float Random_Float(float min, float max)
{
	return g_pRandom.Random_Float(min, max);
}

int RandomProb(int* Prob, unsigned int ProbCount)
{
	if ( !Prob )  return -1;
	return g_pRandom.RandomProb(Prob,ProbCount);
}

