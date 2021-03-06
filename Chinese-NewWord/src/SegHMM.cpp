//#include "stdafx.h"
#include <iostream>	// for getline
#include <fstream>
#include <stdlib.h> // for exit(1) atof
#include <string>
#include "TrainHMM.h"
#include "str_function.h"
#include "SegHMM.h"

namespace Seg
{
	bool loadModel(const char* const filePath , HMMSeg & hmm)
	{
		ifstream ifile(filePath);
		string line;
		vector<string> tmp;
		if(!ifile)
		{
			cout<<"open "<<filePath<<" error!"<<endl;
			exit(1); 
		}
		//第一行： 初始概率	
		if(!getLine(ifile,line)) 
		{
			return false;
		}
		splitStr(line, tmp, " ");
		if(tmp.size() != hmm.STATUS_NUM)
		{
			cout<<"start_p illegal"<<endl;
			return false;
		}
		for(size_t j = 0; j< tmp.size(); j++)
		{
			hmm.startProb[j] = atof(tmp[j].c_str());
		}

		// 转移概率
		for(size_t i = 0; i < hmm.STATUS_NUM; i++)
		{
			if(!getLine(ifile, line))
			{
				return false;
			}
			splitStr(line, tmp, " ");
			if(tmp.size() != hmm.STATUS_NUM)
			{
				cout<<"trans_p illegal"<<endl;
				return false;
			}
			for(size_t j =0; j < hmm.STATUS_NUM; j++)
			{
				hmm.transProb[i][j] = atof(tmp[j].c_str());
			}
		}

		//load _emitProbB
		if(!getLine(ifile, line) || !loadEmitProb(line, hmm.emitProbB))
		{
			return false;
		}

		//load _emitProbE
		if(!getLine(ifile, line) || !loadEmitProb(line, hmm.emitProbE))
		{
			return false;
		}

		//load _emitProbM
		if(!getLine(ifile, line) || !loadEmitProb(line, hmm.emitProbM))
		{
			return false;
		}

		//load _emitProbS
		if(!getLine(ifile, line) || !loadEmitProb(line, hmm.emitProbS))
		{
			return false;
		}

		cout<<"loadModel "<<filePath<<" end."<<endl;

		return true;
	}	



	bool loadEmitProb(const string& line, EmitProbMap& mp)
	{
		if(line.empty())
		{
			return false;
		}
		vector<string> tmp, tmp2;
		uint16_t unico;
		splitStr(line, tmp, ",");
		for(size_t i = 0; i < tmp.size(); i++)
		{
			splitStr(tmp[i], tmp2, ":");
			if(2 != tmp2.size())
			{
				cout<<"_emitProb illegal."<<endl;
				return false;
			}
			if(!decodeOne(tmp2[0], unico))
			{
				cout<<"TransCode failed."<<endl;
				return false;
			}
			mp[unico] = atof(tmp2[1].c_str());
		}
		return true;
	}
	bool decodeOne(const string& str, uint16_t& res)
	{
		vector<uint16_t> ui16;
		if(!utf8ToVec(str, ui16) || ui16.size() != 1)
		{
			return false;
		}
		res = ui16[0];
		return true;
	}
	double getEmitProb(const EmitProbMap* ptMp, uint16_t key, double defVal)
	{
		EmitProbMap::const_iterator cit = ptMp->find(key);
		if(cit == ptMp->end())
		{
			return defVal;
		}
		return cit->second;

	}

	bool viterbi(HMMSeg &hmm, const vector<uint16_t>& unico, vector<unsigned int>& status)
	{

		if(unico.empty())
		{
			return false;
		}

		size_t X = hmm.STATUS_NUM;
		size_t T = unico.size();
		int ** path;
		double ** weight;
		unsigned int maxvalind, stat;
		double tmp, endE, endS;
		double  maxval;
		try
		{

			path = new int* [T];
			weight = new double* [T];
			for(int p = 0; p< T; p++)
			{
				path[p]  = new int[X];
				weight[p]= new double[X];
			}

		}
		catch(const std::bad_alloc&)
		{
			cout<<"bad_alloc"<<endl;
			return false;
		}
		if(NULL == path || NULL == weight)
		{
			cout<<"bad_alloc"<<endl;
			return false;
		}

		/* 1. Initialization  */
		for(unsigned int x = 0; x < X; x++)
		{
			weight[0][x] = hmm.startProb[x] + getEmitProb(hmm.emitProbVec[x], unico[0], MIN_DOUBLE);// 按照log 来算 * 变 +
			path[0][x] = -1;
			//cout<<"weight["<<0<<"]"<<"["<<x<<"]"<<"  "<<weight[0][x]<<endl;
		}

		/* 2. Recursion */
		for(unsigned int t = 1; t < T; t++) //观测时间变化
		{
			for(unsigned int x = 0; x < X; x++) //状态
			{
				maxval = MIN_DOUBLE;
				maxvalind = -1;
				//path[t][x] = E; 
				for(unsigned int prex = 0; prex < X; prex++)
				{
					tmp = weight[t-1][prex] + hmm.transProb[prex][x];

					if(tmp > maxval)
					{
						maxval = tmp;
						maxvalind = prex;
					}
				}

				weight[t][x]= maxval +getEmitProb(hmm.emitProbVec[x], unico[t], MIN_DOUBLE);
				//cout<<"weight["<<t<<"]"<<"["<<x<<"]"<<"  "<<weight[t][x]<<endl;
				path[t][x]= maxvalind;
				//cout<<"path["<<t<<"]"<<"["<<x<<"]"<<"  "<<path[t][x]<<endl;

			}
		}

		/* 3. Termination */
		/*B E M S 只选择结尾是E or S 的可能性*/
		endE = weight[T-1][E];
		endS = weight[T-1][S];
		stat = 0;
		if(endE > endS)
		{
			stat = E;
		}
		else
		{
			stat = S;
		}

		status.assign(T, 0);
		for(int t = T-1 ; t >= 0; t--)
		{
			status[t] = stat;
			stat = path[t][stat];
		}
		for(int p = 0; p< T; p++)
		{
			delete []path[p];
			delete []weight[p];
		}
		delete []path;
		delete []weight;
		return true;
	}
	bool cut(bool is_utf8, HMMSeg & hmm ,const string& str, vector<string>& res)
	{
		res.clear();
		if(str.empty())
		{
			return false;
		}

		PvecToStr_tb  pvecToStr_tb;
		PstrToVec  pstrToVec;

		if(is_utf8)
		{
			pvecToStr_tb = vecToUtf8;
			pstrToVec = utf8ToVec;
		}
		else
		{
			pvecToStr_tb = vecToGbk;
			pstrToVec = gbkToVec;
		}


		vector<uint16_t> unico;
		vector<unsigned int > status; 
		vector<uint16_t>::iterator begin, left, right;
		if(!pstrToVec(str, unico))

		{
			cout<<"TransCode failed."<<endl;
			return false;
		}

		if(!viterbi(hmm,unico, status))
		{
			cout<<"viterbi failed."<<endl;
			return false;
		}

		begin = unico.begin();
		left = begin;
		res.clear();
		for(unsigned int i =0; i< status.size(); i++)
		{
			if(status[i] % 2) //if(E == status[i] || S == status[i])
			{
				right = begin + i + 1;
				res.push_back(vecToUtf8(left, right));
				left = right;
			}

		}

		return true;
	}
	bool cut(bool is_utf8, HMMSeg & hmm, vector<uint16_t> & src_unico, const unsigned int& start, const unsigned int& end, vector<string>& res)
	{
		if(src_unico.empty())
		{
			return false;
		}

		PvecToStr_tb  pvecToStr_tb;
		PstrToVec  pstrToVec;

		if(is_utf8)
		{
			pvecToStr_tb = vecToUtf8;
			pstrToVec = utf8ToVec;
		}
		else
		{
			pvecToStr_tb = vecToGbk;
			pstrToVec = gbkToVec;
		}

		vector<unsigned int > status; 
		vector<uint16_t> unico;
		vector<uint16_t>::iterator begin, left, right;
		for (unsigned int i=start; i< end; i++)
		{
			unico.push_back(src_unico[i]);
		}

		if(!viterbi(hmm,unico, status))
		{
			cout<<"viterbi failed."<<endl;
			return false;
		}

		begin = unico.begin();
		left = begin;
		//res.clear();
		for(unsigned int i =0; i< status.size(); i++)
		{
			if(status[i] % 2) //if(E == status[i] || S == status[i])
			{
				right = begin + i + 1;
				res.push_back(pvecToStr_tb(left, right));
				left = right;
			}

		}

		return true;
	}

}
