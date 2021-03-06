#ifndef _TYPE_H
#define _TYPE_H
#include <map>
#include <vector>
#include <string>
#include <stdint.h>
#include <stdint.h> // uint16_t
#include <map>
#include <vector>
#include <string>
namespace Seg
{   using namespace std;
	typedef std::vector<uint16_t> Unicode;
	typedef map<uint16_t, struct TrieNode* > TrieNodeMap;
	typedef vector<pair<string,pair<int,int> > >Termlistmap;
	//typedef pair<unsigned int, unsigned int> wint;
	typedef struct
	{unsigned int wordf;
	 unsigned int LeftCh;
	 unsigned int RightCh;
	}wfinfo;
	typedef struct
	{unsigned int wordf;
	unsigned int LeftCh;
	unsigned int RightCh;
	unsigned int endfre;
	double f;
	}winf1;
	
    typedef map<string,wfinfo>WordList;
	typedef map<string,winf1>wordlist1;
	typedef vector<string>str_vector;

	//储存分出的所有词,依次为词频，在文中偏移量，词语
	typedef struct 
	{
		vector<uint16_t> res;
		map<unsigned int , unsigned int> position;

	} ReSeg;
	typedef map<uint16_t, double> EmitProbMap;//发射概率
	typedef std::vector<uint16_t> Unicode; //字符编码 2字节
	enum {B=0,E=1,M=2,S=3};
	typedef struct HMM
	{
		/*
		 * STATUS:
		 * 0:B, 1:E, 2:M, 3:S
		 * */
		enum {STATUS_SUM = 4  };
		unsigned int STATUS_NUM;
		double startProb[STATUS_SUM];	//	4
		double transProb[STATUS_SUM][STATUS_SUM]; //	4*4;
		EmitProbMap emitProbB;//	Begin  <code16,prob>
		EmitProbMap emitProbE;//	End	   <code16,prob>
		EmitProbMap emitProbM;//	Middle <code16,prob>
		EmitProbMap emitProbS;//	Single <code16,prob>
		/*
		* vector 可以动态添加
		*B <>,<>,...<>
		*E <>,<>,...<>
		*M <>,<>,...<>
		*S <>,<>,...<>
		**/
		vector<EmitProbMap* > emitProbVec;
		HMM():STATUS_NUM(STATUS_SUM)
		{
			emitProbVec.push_back(&emitProbB);
			emitProbVec.push_back(&emitProbE);
			emitProbVec.push_back(&emitProbM);
			emitProbVec.push_back(&emitProbS);
		}

	} HMMSeg;

	const double MIN_DOUBLE = -3.14e+100;
	const double MAX_DOUBLE = 3.14e+100;


}
#endif