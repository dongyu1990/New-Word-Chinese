//#include "stdafx.h"
#include <iostream>
#include "Interface.h"
#include "SegHMM.h"
#include "str_function.h"
#include "function.h"
namespace Seg
{
	//bool reivse(HMMSeg & hmm, vector<string>& res);
	string segregate(bool is_utf8, HMMSeg & hmm,ReSeg & reseg)
	{
		PvecToStr_wxy  pvecToStr_wxy;
		PstrToVec  pstrToVec;

		if(is_utf8)
		{
			pvecToStr_wxy = vecToUtf8;
			pstrToVec = utf8ToVec;
		}
		else
		{
			pvecToStr_wxy = vecToGbk;
			pstrToVec = gbkToVec;
		}

		vector<string> vecString;
		unsigned int start;
		unsigned int end;
		vector<uint16_t> &res = reseg.res;
		map<unsigned int , unsigned int> &pos = reseg.position;
		map<unsigned int , unsigned int>::const_iterator iter;
		map<unsigned int , unsigned int>::const_iterator iter_pre;
		iter=pos.begin();
		map<unsigned int , unsigned int> seg_pos;//重要辅助容器
		if (res.size()==0)
		{
			return false;
		}
		else
		{
			while(iter != pos.end())
			{
				if (iter->first == iter->second - 1)// 单字
				{
					if (getEmitProb(hmm.emitProbVec[0], res[iter->first], MIN_DOUBLE) != MIN_DOUBLE)//是中文
					{
						iter_pre = iter;
						if (iter_pre==pos.begin())
						{

							seg_pos[iter->first] = iter->second;
						}
						else
						{
							iter_pre--;	
							if (iter_pre->second == iter->first)//是连续单字
							{
								seg_pos[iter->first] = iter->second;
							} 
							else//非连续单字
							{
								//输出前面的连续单字
								if (seg_pos.size()!=0)//辅助序列已经不为空
								{
									if(seg_pos.size()==1)//只添加了一个
									{
										start=(seg_pos.begin())->first;
										end = start +1;
										vecString.push_back(pvecToStr_wxy(res,start,end));
									}
									else//添加了多个单中文 需要分词
									{
										start=(seg_pos.begin())->first;
										end=(--seg_pos.end())->second;
										cut(is_utf8,hmm,res,start,end ,vecString);
									}
									//清空序列
									seg_pos.clear();
								}
								seg_pos[iter->first] = iter->second;
							}
						}

					} 
					else//不是中文
					{
						if (seg_pos.size()!=0)//辅助序列已经不为空
						{
							if(seg_pos.size()==1)//只添加了一个
							{
								start=(seg_pos.begin())->first;
								end = start +1;
								vecString.push_back(pvecToStr_wxy(res,start,end));
							}
							else//添加了多个单中文 需要分词
							{
								start=(seg_pos.begin())->first;
								end=(--seg_pos.end())->second;
								cut(is_utf8 , hmm,res,start,end ,vecString);
							}
							//清空序列
							seg_pos.clear();
						}

						//直接添加入非中文单字
						start=iter->first; //当前iter,可以直接用
						end=start +1 ;
						vecString.push_back(pvecToStr_wxy(res,start,end));

					}
				}
				else // 非单字
				{
					if (seg_pos.size()!=0)//辅助序列已经不为空
					{
						if(seg_pos.size()==1)//只添加了一个
						{
							start=(seg_pos.begin())->first;
							end = start +1;
							vecString.push_back(pvecToStr_wxy(res,start,end));
						}
						else//添加了多个单中文 需要分词
						{
							start=(seg_pos.begin())->first;
							end=(--seg_pos.end())->second;
							cut(is_utf8,hmm,res,start,end ,vecString);
						}
						//清空序列
						seg_pos.clear();
					}
					//直接添加入非单字
					start=iter->first;
					end=iter->second;
					vecString.push_back(pvecToStr_wxy(res,start,end));
				}

				iter++;
			}
			//最后中文序列的压栈
			if (seg_pos.size()!=0)
			{
				start=(seg_pos.begin())->first;
				end=(--seg_pos.end())->second;
				cut(is_utf8,hmm,res,start,end ,vecString);
			}

			string finalresult;
			for (int i = 0; i < vecString.size() ; i++)
				finalresult+=vecString[i]+"/";
			return finalresult;
		}
	}

	bool revise(bool is_utf8, HMMSeg & hmm, ReSeg & reseg)
	{
		PvecToStr_wxy  pvecToStr_wxy;
		PstrToVec  pstrToVec;

		if(is_utf8)
		{
			pvecToStr_wxy = vecToUtf8;
			pstrToVec = utf8ToVec;
		}
		else
		{
			pvecToStr_wxy = vecToGbk;
			pstrToVec = gbkToVec;
		}


		vector<uint16_t> &res = reseg.res;
		vector<string> vecString;
		map<unsigned int , unsigned int> &pos = reseg.position;
		map<unsigned int , unsigned int>::const_iterator iter;
		map<unsigned int , unsigned int>::const_iterator iter_pre;
		iter_pre=pos.begin();
		unsigned int start=(iter_pre)->first;
		unsigned int end=iter_pre->second;
		for(iter = ++pos.begin(); iter_pre != pos.end();)
		{
			if ((iter_pre)->first + 1 == (iter_pre)->second)//单字
			{
				start=(iter_pre)->first;
				if (iter != pos.end() && iter->first -1 == iter_pre->first)
				{
					while(iter->first -1 == iter_pre->first)
					{
						end = iter->first;
						iter_pre = iter;
						if (++iter == pos.end())
						{
							if (iter_pre->first +1 == iter_pre->second)
							{
								end = iter_pre->second;
								iter_pre =pos.end(); // last word
							}
							break;
						}

					}
					cut(is_utf8,hmm,res,start,end ,vecString);
				} 
				else
				{
					if (iter != pos.end())
					{
						end=(iter_pre)->second;
						vecString.push_back(pvecToStr_wxy(res,start,end));
						iter_pre = iter;
						++iter;
					}
					else
					{
						end=(iter_pre)->second;
						vecString.push_back(pvecToStr_wxy(res,start,end));
						iter_pre = iter;
					}

				}






			}
			else//非单字
			{
				start=(iter_pre)->first;
				end=(iter_pre)->second;
				vecString.push_back(pvecToStr_wxy(res,start,end));
				iter_pre = iter;
				if (iter != pos.end())
				{
					++iter;
				}

			}
		}
		for (int i = 0; i < vecString.size() ; i++)
			cout<<vecString[i]<<"/";
		cout<<endl;
		return true;
	}

}
