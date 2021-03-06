//#include "StdAfx.h"
#include "function.h"
#include <string>
#include "str_function.h"
#include "Type.h"
#include<ctime>
#include <math.h>
#include<stdlib.h>
#include <algorithm>
using namespace std;
namespace Seg
{   
	
	/**********************************************
	功能：初始化分词，读取字典
	参数：字典路径，类型参数
	返回值：根节点指针
	***********************************************/
	Trie Initialization()
	{
		Trie mytree;

		  
		return mytree;
	}

	bool Loaddict(Trie& mytree,bool if_utf8,const char * const dict_path)
	{
		if(0==if_utf8)
		{
			mytree.loadDict(0,dict_path);
		}
		else
		{
			mytree.loadDict(1,dict_path);
		}
		return true;
	}

	/**********************************************
	功能：获取字符中每个字的平均词频
	***********************************************/
	double geteverywordfre(bool if_utf8,Trie& mytree,string str)
	{
		double r=0;
		Unicode temp;
		bool ifok=strTovec(if_utf8,str,temp);
		if(false==ifok)
		{
			cout<<"hehe3"<<endl;
		}
		if(0==temp.size())
		{
			return 0;
		}
		for(int i=0;i<temp.size();++i)
		{  if(mytree.find(if_utf8,vecTostr(if_utf8,temp,i,i+1))!=NULL)
		    {
			  r+=mytree.find(if_utf8,vecTostr(if_utf8,temp,i,i+1))->freq;
		    }
		    else
			{
				r+=1000000;
		    }

			
		}
		r=r/temp.size();
		return r;

	}
	/**********************************************
	功能：消除树
	***********************************************/
	void Finish (Trie& mytree)
	{   
		mytree.~Trie();
	}	
	/**********************************************
	功能：分词
	参数：字符串
	返回值：所有分词集合（词，词频，偏移量）
	***********************************************/
	
    Termlistmap get_alltermutf8(Trie& mytree,string str)
	{
		Termlistmap allterm;
		Unicode temp;
		if(!utf8ToVec(str,temp))
		{
			cout<<"转码错误"<<endl;
		}		
		//Trie mytree;
		//mytree.loadDict(1,dict_path);
		//TrieNode* p =mytree.root;
		TrieNode* p=mytree.root;
		int cut=0;//the last cut postion
		int pos=0;//the cut postion
		int wordsnum=0;//the found numbers

		for(unsigned int j=0;j<temp.size();++j)/////scan all words by word
		{   wordsnum=0;
		for(unsigned int i=j;i<temp.size();i++)
		{ 
			if(p->node_map.end() == p->node_map.find(temp[i]))//can't find
			{  				
				p=mytree.root;
				pos=0;
				break;
			}

			else//found
			{   
				TrieNodeMap::iterator it=p->node_map.find(temp[i]);
				p=it->second;
				pos++;
				if(p->if_Leaf==true)
				{   
					cut=j;
				    string temp_term=vecToUtf8(temp,cut,pos+cut);
			    	p=mytree.root;	
				    pair<int,int> a;						
				    a.first=mytree.node_infomation[p->nodeInfo_pos].freq;
			     	a.second=j;
				    allterm.push_back(pair<string,pair<int,int> >(temp_term,a));
				    wordsnum++;
				    pos=0;
				    break;
				}

				else
				{
					if(p->if_wors==true)
					{					
						cut=j;
						string temp_term=vecToUtf8(temp,cut,pos+cut);
						////////选择添加的词语
						if(temp_term.size()>2)
						{
							pair<int,int> a;						
							a.first=mytree.node_infomation[p->nodeInfo_pos].freq;
							a.second=j;
							allterm.push_back(pair<string,pair<int,int> >(temp_term,a));
							wordsnum++;
						}
					}
				}


			}//found
	    }
	}

		return allterm;
	}



	Termlistmap get_allterm(Trie& mytree,string str)
	{
		Termlistmap allterm;
		Unicode temp;
		if(!gbkToVec(str,temp))
		{
			cout<<"转码错误"<<endl;
		}		
		//Trie mytree;
		//mytree.loadDict(0,dict_path);
		
		TrieNode* p =mytree.root;
		
		int cut=0;//the last cut postion
		int pos=0;//the cut postion
		int wordsnum=0;//the found numbers
		
 		for(unsigned int j=0;j<temp.size();++j)/////scan all words by word
		{   wordsnum=0;
			for(unsigned int i=j;i<temp.size();i++)
			{ 
				if(p->node_map.end() == p->node_map.find(temp[i]))//can't find
				{  				
					p=mytree.root;
					pos=0;
					break;
				}

				else//found
				{   
					TrieNodeMap::iterator it=p->node_map.find(temp[i]);
					p=it->second;
					pos++;
					if(p->if_Leaf==true)
					{   cut=j;
						string temp_term=vecToGbk(temp,cut,pos+cut);
						p=mytree.root;	
						pair<int,int> a;						
						a.first=mytree.node_infomation[p->nodeInfo_pos].freq;
						a.second=j;
						allterm.push_back(pair<string,pair<int,int> >(temp_term,a));
						wordsnum++;
						pos=0;
						break;
					}
					
					else
					{
						if(p->if_wors==true)
						{					
						cut=j;
						string temp_term=vecToGbk(temp,cut,pos+cut);
						////////选择添加的词语
						if(temp_term.size()>2)
						   {
							pair<int,int> a;						
							a.first=mytree.node_infomation[p->nodeInfo_pos].freq;
							a.second=j;
							allterm.push_back(pair<string,pair<int,int> >(temp_term,a));
							wordsnum++;
						   }
						}
					}
					
				
				}//found
			}
		}
		//test
		return allterm;
	}
	/***************************************************************************************


	/**********************************************
	功能：对指定的字符串分词，得到最大可能分词结果
	参数：字符串(GBK)
	返回值：分词结果
	注：还需添加多个参数，实现定制化分词
	***********************************************/
	string segement(bool if_utf8,Trie& mytree,string str)//// A const pointer to const data
	{   string str_return="";
		vector<uint16_t> line;
		bool if_ok=strTovec(if_utf8,str, line);
		if(!if_ok)
		{
			cout<<"error1"<<endl;
		}
		TrieNode* p =mytree.root;
 		int begpos=0;
		int endpos=0;
		int if_englishname=0;
		map<string,unsigned int>namefre;
		map<string,unsigned int>secondnamefre;
		bool if_getname=getnamefre ("Chinese.name",namefre);
		bool if_secondgetname=getnamefre ("Chinese.secondname",secondnamefre);
		if((false==if_getname)||(false==if_secondgetname))
		{
			cout<<"getname error";
		}
		//
		unsigned int max_long=0;//记录向右切割的最大偏移量
		map<int,int> pos_map;//recod the cut pos,the first is begin,second is end 
		//标点符号
	
	   /***************查字典树，对输入的长句进行切分********************************************/	
		
	   for(unsigned int i=0;i<line.size();++i)
	   {  
		   string te=vecTostr(if_utf8,line,i,i+1);

		   if((i-1)==max_long)//切割
		   {
			   endpos=i;
			   if(endpos>begpos)
			   {
				    pos_map.insert(pair<int,int>(begpos,endpos));
			   }
			  
			   if_englishname=0;
			   if(i<line.size())
			   {
				   begpos=i;

			   }
			   else
			   { 
				   break;
			   }
		   }

		   if(i==line.size()-1)
		   {   
			   if(false==if_Punctuation(if_utf8,line,i))
			   {
				   endpos=i+1;
				   pos_map.insert(pair<int,int>(begpos,endpos));
				   if_englishname=0;
			   }
			   else
			   {
				   endpos=i;
				   if(begpos<endpos)
				   {
					   pos_map.insert(pair<int,int>(begpos,endpos));
					   if_englishname=0;
					   i++;
				   }

				   break;
			   }
			  
		   }
		   
		   if(false==if_Punctuation(if_utf8,line,i))//是否为标点
		   {

		   }

		   else
		   {
			   endpos=i;
			   if(i+1<line.size())
			   {
				   begpos=i+1;
			   }
			   else
			   { 
				   break;
			   }
		   }//为标点
		   
		   if((line[i]=='-')||(line[i]=='－')||(line[i]=='·')||(line[i]=='•'))
		   {
			   if_englishname=1;
			   max_long=i+1;		       
			   continue;
		   }

		   if(true==if_connect(if_utf8,line,i))
		   {
			   if(false==if_Punctuation(if_utf8,line,i+1))//是否为标点
			   {   
				              
				   if(true==if_connect(if_utf8,line,i+1))
				   {						   
					   max_long=i+1;	
				   }


			   }

			   else
			   {
				   max_long=i;
			   }
			   continue;
		   }

		   /////////////////////////////////////////////////////////////////////



		   for(unsigned int j=i;j<line.size();++j)//以每个字为单位，向后查找，得到最大偏移量
		   {
			   if(p->node_map.end() == p->node_map.find(line[j]))//can't find,cut the word
			   {
				   if((line[j]=='-')||(line[j]=='－')||(line[j]=='•')||(line[i]=='·'))
				   {
					   if_englishname=1;
					   max_long=j+1;
				   }				  
				   p=mytree.root;
				   break;
			   }//找不到改字


			  else//找到该字
			  {
			  
				TrieNodeMap::iterator it=p->node_map.find(line[j]);
    
				if(if_englishname==0)
				{
				  if((j==endpos)||(true==if_Punctuation(if_utf8,line,j-1)))//判断首字是否为姓氏，一
				  {
						
				    if(false==if_name(if_utf8,line,j))
				    {
						//不在姓氏表中
				    }

			        else //在姓氏表中
				    {
					  TrieNode *qq=it->second;
					  if((true==if_connect(if_utf8,line,j+1))||(true==if_Punctuation(if_utf8,line,j+1)))
					  {

					  }
					  
					  else
					  {	   /*姓氏后面跟着称谓词*/
						   int t_wordlong=1;
						   int t_temp01=j+1;
						   string wordflg="";
						   bool if_nameend=check_ifword(if_utf8,p,mytree,wordflg,t_temp01,line);
						   if((true==if_nameend)&&(wordflg.find("nd")!=wordflg.npos))
						   {
							  t_wordlong+=(t_temp01-j)-2;
							  max_long=j+t_wordlong;
							  i+=t_wordlong;
							  if(i==line.size()-1)
							  {
								  pos_map.insert(pair<int,int>(begpos,i+1));
							  }

					    	   p=mytree.root;
						       break;
					        }
						   else
						   {
					/*在姓氏表中而且能够成词的情况*/
					//1、词语长度大于3
					//2、词语长度=3
					//3、词语长度=2
					/*在姓氏表中而且能够成词的情况*/

						  int t_wordlong=1;
						  int t_temp01=j+1;
						  string wordflg="";
		
						 bool cword=check_ifnameword(if_utf8,qq,mytree,wordflg,t_temp01,line);//判断是否和后面成词语

						 if(true==cword)
						 {   
							 t_wordlong+=(t_temp01-j);
						 }

						 if(t_wordlong>=3)
						 {
							 //
						 }
						 else 
						 {
							 
						   if((j+t_wordlong<line.size()-1)&&((line[j+t_wordlong]=='-')||(line[j+t_wordlong]=='－')))//词语后面是破折号
							 {
							   //不处理
							 }
						   else//后面不是破折号
							 {
						        if(j==i)
								 {
								   p=mytree.root;
								   if(j+2<line.size())
								    {   
									 bool if_wait=false;//判断是否待定
									 //第三字与后面不能成词，单独成词概率小N=3
									 //第3字与后面能成词，且单词概率小N=2
									 //第3字与后面能成词，且单词概率大 待定
									 int temp_i=j+2;
									 int N=1;
									 //下2位是破折号
									 if(((line[temp_i]=='-')||(line[temp_i]=='－')))
									 {
										 if(max_long<j+N+1)
										 {
											 max_long=j+N+1;  
										 }
										 p=mytree.root;
										 break;
									 }
									else  if(true==(if_Punctuation(if_utf8,line,temp_i))||(if_connect(if_utf8,line,temp_i)))//是否为符号
									 {
										 //下2位是标点
									    if(max_long<j+N)
										 {
											 max_long=j+N;  
										 }
										 p=mytree.root;
										 break;
									 }

									 else//下2位不是标点和破折号
									 {
								    	 if(p->node_map.end() == p->node_map.find(line[temp_i]))
										 {
											 N++;
										 }

									     else
										 {
											 TrieNodeMap::iterator it=p->node_map.find(line[temp_i]);
											 p=it->second;
											 TrieNode *pp=mytree.root;
											 /////////////////////////////////////////判断第3个字是否和后面成词
											 bool checkword=false;
											 int temp22=temp_i;
											 string wflg="";
							             	 checkword=check_ifword(if_utf8,pp,mytree,wflg,temp22,line);	
											 //
											 if((true==checkword)&&(temp22-temp_i>1))//
											 {
												 checkword=true;

											 }
											 else
											 {
												 checkword=false;//排除第一个字的情况（曾在青岛）
											 }
				                             if((false==checkword)||(pp->node_map.end() == pp->node_map.find(line[temp_i])))//后面不能能成词
									           {
										          N++;
									           }//case 1
	
				 	                         unsigned int _namef=countnamefre(if_utf8,line,temp_i,namefre); //*获取该字成姓名的概率
											 if((true==checkword)&&(_namef<200))
											   {
													//
											   }//case 2
										 	if((true==checkword)&&(_namef>=200))
											  {
												 if_wait=true;
											  }
										}
									}//下2位不是标点和破折号

							
						 if(max_long<j+N)
							{
								 max_long=j+N;
							}
					
						 
						 
						 //这里加上一定条件：
						 if(false==if_wait)					
							{	
								//if(N)
								i+=N;
						        if(i==line.size()-1)
								{
								  pos_map.insert(pair<int,int>(begpos,i+1));
								}
						  						  
						    }//将姓名和可能分为姓名的2个或3个字切开
						 else
							{
								i+=1;
								 max_long+=1;
						   }


						 }//if(j+2<line.size())

						
						else
						{
							 endpos=line.size()-1;
							 pos_map.insert(pair<int,int>(begpos,endpos));
					    } 
						
						p=mytree.root;
						break;
				   }// if(j==i)
								 
				}//后面不是破折号
							 
							 
			 }
			} /////////////////////////////////////////////////////////////////////////////////////////
					
			}
			}//在姓氏表中


		}//if((j==endpos)||(true==if_Punctuationgbk(line,j-1)))
						
	}//if(if_englishname==0)


				   //判断是否成词语
                   
				   			   
				   ////
				   if(max_long<j)
				   {    
					   if(true==it->second->if_wors)
					      { max_long=j;}
					   if(j==line.size()-1)
					   {
						   endpos=j+1;
						   pos_map.insert(pair<int,int>(begpos,endpos));

					   }
				   }
				   p=it->second;
				  // break;
			   }
		   }

	   }
	    //查字典树，对输入的长句进行切分



	   //测试切分结果
	   /*
	    map<int,int>::iterator it;
		for(it=pos_map.begin();it!=pos_map.end();it++)
		{
			cout<<"begin"<<it->first<<"     end"<<it->second<<"  "<<vecTostr(if_utf8,line,it ->first,it->second)<<endl;
			
		}
	   */
/************************根据切分的结果集，依次分词取出最大概率**************/
		
		map<int,int>::iterator it1;
		for(it1=pos_map.begin();it1!=pos_map.end();it1++)
		{
			//对单句进行切分，取出得分最高的情况
			int K=it1->second-it1->first;
		   
			if(K<=2)//2个一下的字
		     {
				 string sttrr=vecTostr(if_utf8,line,it1->first,it1->second);
				 str_return+=sttrr;
				 str_return+="/ ";
				 continue;
  
		     }
            //大于2个字，但是一个词的情况
			/*
			if(K==3)
			{   //判断前两字是否结合
				TrieNode *w=mytree.root;
				if(w->node_map.end()==w->node_map.find(line[it1->first]))
				{

				}
				else
				{
					TrieNodeMap::iterator itw = w->node_map.find(line[it1->first]);				
					w=itw->second;
					/////////////////////////////////////////////////////////////////
						//判断是否为姓氏

						if(false==if_name(if_utf8,line,it1->first))
						{

						}
						else//判断是3个字or 2
						{   int temp=0;
						//判断后两字是否成词
						
						TrieNode *R=mytree.root;

						//判断第3字是否独立成词
						//R=mytree.root;
						if(R->node_map.end() == R->node_map.find(line[it1->first+2]))
						{
							temp=2;
						}
						else
						{
							TrieNodeMap::iterator ittt=R->node_map.find(line[it1->first+2]);
							R=ittt->second;
							//加上定义，这里地个字不能为名词
							if((R->if_wors==1)&&(mytree.node_infomation[R->nodeInfo_pos].freq>10000)&&(mytree.node_infomation[R->nodeInfo_pos].tag!="n"))//自定义规则
								temp=1;
							else
								temp=2;
						}
						if(2==temp)
						{
							string sttrr=vecTostr(if_utf8,line,it1->first,it1->second);
							str_return+=sttrr;
							str_return+="/ ";
							continue;
						}
						else 
						{
							string sttrr=vecTostr(if_utf8,line,it1->first,it1->second-1);
							str_return+=sttrr;
							str_return+="/ ";

							sttrr=vecTostr(if_utf8,line,it1->second-1,it1->second);
							str_return+=sttrr;
							str_return+="/ ";
							continue;

						}
					}				
				
				}
				

			}//K==3
			
			*/
							
				double ** D = new double *[K];
				for(int i=0;i<K;i++)				
				{
					D[i] = new double[K];
				}
				//初始化矩阵
				for(int v=0;v<K;v++)
				{
					for(int x=0;x<K;x++)
					{
						D[v][x]=8000000;
					}
				}
				//初始化矩阵
			
				for(int n=it1->first,h=0;n<it1->second;++n,++h)//对每一个短语进行分词
				{   int nn=n;
					p =mytree.root;
					while(p->node_map.end()!=p->node_map.find(line[nn]))
					{
						TrieNodeMap::iterator it2=p->node_map.find(line[nn]);

						p=it2->second;
						if(p->if_wors==true)
						{										
						  double temp=mytree.node_infomation[p->nodeInfo_pos].freq;
						  temp=temp/400000000000;
						  temp=log(temp)/sqrt(double(mytree.node_infomation[p->nodeInfo_pos].wLen));
						  D[h][nn-it1->first]=0-temp;
						 }
				   
						if(nn+1<it1->second)
						{
							nn++;
						}
								
					}

				}
				/*
				for(int v=0;v<K;v++)
				{
					for(int x=0;x<K;x++)
					{
						cout<<D[v][x]<<" ";
					}
					cout<<endl;
				}
				*/
          /**************************通过姓氏修改矩阵*****************************************/
				
				if(true==if_name(if_utf8,line,it1->first))
				{   //1、姓氏加称谓
					
					p =mytree.root;
					int t_wordlong=1;
					int t_temp01=it1->first+1;
					string word_flg="";
					bool ifnameend=check_ifword(if_utf8,p,mytree,word_flg,t_temp01,line);
					if((true==ifnameend)&&(word_flg.find("nd")!=word_flg.npos))
					{
						t_wordlong+=(t_temp01-it1->first)-2; 
						if(t_wordlong<=it1->second)
						{
							D[0][t_wordlong]=1;
						}						
					}

					else
					{   double _secondnamefre=0;
						for(int jj=1;jj<3;++jj)
						{				
							double _namefre1=countnamefre(if_utf8,line,it1->first+jj,namefre);
							if(1==jj)
							{ 
	                          	_secondnamefre=countnamefre(if_utf8,line,it1->first+jj,secondnamefre);
							}
					
							if(0==_namefre1)
							{	}
							else
							{
								if(D[0][jj]>1000)
								{ 
									//这里需要改进算法
									/*
									if(1==jj)
										D[0][jj]=10000/_namefre1;
									else
									{   
										TrieNode *R=mytree.root;
    									bool temp=if_nameendisword(R,mytree,it1->first+2,line);
										if(false==temp)
									     {
										   if(0!=_secondnamefre)
										   {
											   D[0][jj]=(8000/_secondnamefre)*8000/_namefre1;
										   }
									     }
									   
									
									}   
									*/
									//double n=1/3;
									if(1==jj)
										
										D[0][jj]=350/(pow(_namefre1,0.4));
									else
									{   
										TrieNode *R=mytree.root;
										bool temp=if_nameendisword(R,mytree,it1->first+2,line);
										if(false==temp)
										{
											if(0!=_secondnamefre)
											{
												D[0][jj]=(200/pow(_secondnamefre,0.4))*110/pow(_namefre1,0.4);
											}
										}


									}   


								}

							}
						}
					}

					/*
					for(int jj=1;jj<3;++jj)
					{				
						double _namefre1=0;
						string _strname=vecTostr(if_utf8,line,it1->first+jj,it1->first+jj+1);
						map<string,unsigned int>::iterator findname=namefre.find(_strname);

						if(findname!=namefre.end())
						{
							_namefre1=findname->second;
						}
						if(0==_namefre1)
						{	}
						else
						{
							if(D[0][jj]>1000)
							{ 
								//这里需要改进算法
								if(1==jj)
									D[0][jj]=2000/_namefre1;
								else
									D[0][jj]=D[0][1]*200/_namefre1;
							}

						}
					}*/
				}
          /*****************通过姓氏修改矩阵**************************************************/

			//test D
			/*
			for(int v=0;v<K;v++)
			{
				for(int x=0;x<K;x++)
				{
					cout<<D[v][x]<<" ";
				}
				cout<<endl;
			}
			*/
			

			//对矩阵求最小路径(0---->K-1)
				if(D[0][K-1]<100)
				{
					string sttrr=vecTostr(if_utf8,line,it1->first,it1->second);
					str_return+=sttrr;
					str_return+="/ ";

				}

				else
				{
					double score=0;
					map<double,string>dmap;//存储源点到达该点的距离，路径
					double *A=new double[K];//
					string  *B=new string[K];
					double W=0;
					string w_str="";
					for(int ii=0;ii<K;++ii)
					{

						W=D[0][ii];

						stringstream ss;  
						ss<<ii;  
						w_str=ss.str();
						
						for(int pp=0;pp<ii;++pp)
						{   
							double xx=D[pp+1][ii];
							if(W>(A[pp]+D[pp+1][ii]))
							{
								W=A[pp]+D[pp+1][ii];//加入路径
								ss.str("");
								ss<<ii;  
								w_str=B[pp]+ss.str();		
							
							}

						}

						A[ii]=W;
						B[ii]=w_str+"/";
						
					}
                				 
				string wa=B[K-1];
				int temp_n=0;
				while(string::npos!=wa.find("/"))
				{
					string temp_sttr=string(wa,0,wa.find("/"));
					wa=string(wa,wa.find("/")+1);
					int tem_int=atoi(temp_sttr.c_str());
					str_return+= vecTostr(if_utf8,line,it1->first+temp_n,it1->first+(tem_int+1));   
					str_return+="/ ";
					temp_n=tem_int+1;
				}

    			delete []A;
				delete []B;
				
				}
			
				//对矩阵求最小路径
				for(int vv=0;vv<K;++vv)
				{
					delete[]D[vv];
				}
				delete []D;

				

        }
	   	return str_return;
	}



	/*
	功能：获取所有匹配词
	参数：编码类型（0 gbk,1 utf-8),输入字符,字典路径
	返回值：所有匹配词
	*/
	Termlistmap getallterm(Trie& mytree,bool if_utf8,string str)
	{
		Termlistmap returnmap;
		if(0==if_utf8)
		{
			returnmap=get_allterm(mytree,str);
		}
		else
		{
			returnmap=get_alltermutf8(mytree,str);
		}

		return returnmap;

	}

	

    ReSeg str_segementutf8tohmm(bool if_utf8,Trie& mytree,string str)
	{	
    	ReSeg returnReg;
	 	bool if_ok=strTovec(if_utf8,str, returnReg.res);
		if(!if_ok)
		{
			cout<<"error1"<<endl;
		}
		TrieNode* p =mytree.root;
 		int begpos=0;
		int endpos=0;
		int if_englishname=0;
		map<string,unsigned int>namefre;
		map<string,unsigned int>secondnamefre;
		bool if_getname=getnamefre ("Chinese.name",namefre);
		bool if_secondgetname=getnamefre ("Chinese.secondname",secondnamefre);
		if((false==if_getname)||(false==if_secondgetname))
		{
			cout<<"getname error";
		}
		//
		unsigned int max_long=0;//记录向右切割的最大偏移量
		map<int,int> pos_map;//recod the cut pos,the first is begin,second is end 
		//标点符号
	
	   /***************查字典树，对输入的长句进行切分********************************************/	
		
	   for(unsigned int i=0;i<returnReg.res.size();++i)
	   {  
		   string te=vecTostr(if_utf8,returnReg.res,i,i+1);

		   if((i-1)==max_long)//切割
		   {
			   endpos=i;
			   if(endpos>begpos)
			   {
				    pos_map.insert(pair<int,int>(begpos,endpos));
			   }
			  
			   if_englishname=0;
			   if(i<returnReg.res.size())
			   {
				   begpos=i;

			   }
			   else
			   { 
				   break;
			   }
		   }

		   if(i==returnReg.res.size()-1)
		   {   
			   if(false==if_Punctuation(if_utf8,returnReg.res,i))
			   {
				   endpos=i+1;
				   pos_map.insert(pair<int,int>(begpos,endpos));
				   if_englishname=0;
			   }
			   else
			   {
				   endpos=i;
				   if(begpos<endpos)
				   {
					   pos_map.insert(pair<int,int>(begpos,endpos));
					   if_englishname=0;
					   i++;
				   }

				   break;
			   }
			  
		   }
		   
		   if(false==if_Punctuation(if_utf8,returnReg.res,i))//是否为标点
		   {

		   }

		   else
		   {
			   endpos=i;
			   if(i+1<returnReg.res.size())
			   {
				   begpos=i+1;
			   }
			   else
			   { 
				   break;
			   }
		   }//为标点
		   
		   if((returnReg.res[i]=='-')||(returnReg.res[i]=='－')||(returnReg.res[i]=='·')||(returnReg.res[i]=='•'))
		   {
			   if_englishname=1;
			   max_long=i+1;		       
			   continue;
		   }

		   if(true==if_connect(if_utf8,returnReg.res,i))
		   {
			   if(false==if_Punctuation(if_utf8,returnReg.res,i+1))//是否为标点
			   {   
				              
				   if(true==if_connect(if_utf8,returnReg.res,i+1))
				   {						   
					   max_long=i+1;	
				   }


			   }

			   else
			   {
				   max_long=i;
			   }
			   continue;
		   }

		   /////////////////////////////////////////////////////////////////////



		   for(unsigned int j=i;j<returnReg.res.size();++j)//以每个字为单位，向后查找，得到最大偏移量
		   {
			   if(p->node_map.end() == p->node_map.find(returnReg.res[j]))//can't find,cut the word
			   {
				   if((returnReg.res[j]=='-')||(returnReg.res[j]=='－')||(returnReg.res[j]=='•')||(returnReg.res[i]=='·'))
				   {
					   if_englishname=1;
					   max_long=j+1;
				   }				  
				   p=mytree.root;
				   break;
			   }//找不到改字


			  else//找到该字
			  {
			  
				TrieNodeMap::iterator it=p->node_map.find(returnReg.res[j]);
    
				if(if_englishname==0)
				{
				  if((j==endpos)||(true==if_Punctuation(if_utf8,returnReg.res,j-1)))//判断首字是否为姓氏，一
				  {
						
				    if(false==if_name(if_utf8,returnReg.res,j))
				    {
						//不在姓氏表中
				    }

			        else //在姓氏表中
				    {
					  TrieNode *qq=it->second;
					  if((true==if_connect(if_utf8,returnReg.res,j+1))||(true==if_Punctuation(if_utf8,returnReg.res,j+1)))
					  {

					  }
					  
					  else
					  {	   /*姓氏后面跟着称谓词*/
						   int t_wordlong=1;
						   int t_temp01=j+1;
						   string wordflg="";
						   bool if_nameend=check_ifword(if_utf8,p,mytree,wordflg,t_temp01,returnReg.res);
						   if((true==if_nameend)&&(wordflg.find("nd")!=wordflg.npos))
						   {
							  t_wordlong+=(t_temp01-j)-2;
							  max_long=j+t_wordlong;
							  i+=t_wordlong;
							  if(i==returnReg.res.size()-1)
							  {
								  pos_map.insert(pair<int,int>(begpos,i+1));
							  }

					    	   p=mytree.root;
						       break;
					        }
						   else
						   {
					/*在姓氏表中而且能够成词的情况*/
					//1、词语长度大于3
					//2、词语长度=3
					//3、词语长度=2
					/*在姓氏表中而且能够成词的情况*/

						  int t_wordlong=1;
						  int t_temp01=j+1;
						  string wordflg="";
		
						 bool cword=check_ifnameword(if_utf8,qq,mytree,wordflg,t_temp01,returnReg.res);//判断是否和后面成词语

						 if(true==cword)
						 {   
							 t_wordlong+=(t_temp01-j);
						 }

						 if(t_wordlong>=3)
						 {
							 //
						 }
						 else 
						 {
							 
						   if((j+t_wordlong<returnReg.res.size()-1)&&((returnReg.res[j+t_wordlong]=='-')||(returnReg.res[j+t_wordlong]=='－')))//词语后面是破折号
							 {
							   //不处理
							 }
						   else//后面不是破折号
							 {
						        if(j==i)
								 {
								   p=mytree.root;
								   if(j+2<returnReg.res.size())
								    {   
									 bool if_wait=false;//判断是否待定
									 //第三字与后面不能成词，单独成词概率小N=3
									 //第3字与后面能成词，且单词概率小N=2
									 //第3字与后面能成词，且单词概率大 待定
									 int temp_i=j+2;
									 int N=1;
									 //下2位是破折号
									 if(((returnReg.res[temp_i]=='-')||(returnReg.res[temp_i]=='－')))
									 {
										 if(max_long<j+N+1)
										 {
											 max_long=j+N+1;  
										 }
										 p=mytree.root;
										 break;
									 }
									else  if(true==(if_Punctuation(if_utf8,returnReg.res,temp_i))||(if_connect(if_utf8,returnReg.res,temp_i)))//是否为符号
									 {
										 //下2位是标点
									    if(max_long<j+N)
										 {
											 max_long=j+N;  
										 }
										 p=mytree.root;
										 break;
									 }

									 else//下2位不是标点和破折号
									 {
								    	 if(p->node_map.end() == p->node_map.find(returnReg.res[temp_i]))
										 {
											 N++;
										 }

									     else
										 {
											 TrieNodeMap::iterator it=p->node_map.find(returnReg.res[temp_i]);
											 p=it->second;
											 TrieNode *pp=mytree.root;
											 /////////////////////////////////////////判断第3个字是否和后面成词
											 bool checkword=false;
											 int temp22=temp_i;
											 string wflg="";
							             	 checkword=check_ifword(if_utf8,pp,mytree,wflg,temp22,returnReg.res);	
											 //
											 if((true==checkword)&&(temp22-temp_i>1))//
											 {
												 checkword=true;

											 }
											 else
											 {
												 checkword=false;//排除第一个字的情况（曾在青岛）
											 }
				                             if((false==checkword)||(pp->node_map.end() == pp->node_map.find(returnReg.res[temp_i])))//后面不能能成词
									           {
										          N++;
									           }//case 1
	
				 	                         unsigned int _namef=countnamefre(if_utf8,returnReg.res,temp_i,namefre); //*获取该字成姓名的概率
											 if((true==checkword)&&(_namef<200))
											   {
													//
											   }//case 2
										 	if((true==checkword)&&(_namef>=200))
											  {
												 if_wait=true;
											  }
										}
									}//下2位不是标点和破折号

							
						 if(max_long<j+N)
							{
								 max_long=j+N;
							}
					
						 
						 
						 //这里加上一定条件：
						 if(false==if_wait)					
							{	
								//if(N)
								i+=N;
						        if(i==returnReg.res.size()-1)
								{
								  pos_map.insert(pair<int,int>(begpos,i+1));
								}
						  						  
						    }//将姓名和可能分为姓名的2个或3个字切开
						 else
							{
								i+=1;
								 max_long+=1;
						   }


						 }//if(j+2<returnReg.res.size())

						
						else
						{
							 endpos=returnReg.res.size()-1;
							 pos_map.insert(pair<int,int>(begpos,endpos));
					    } 
						
						p=mytree.root;
						break;
				   }// if(j==i)
								 
				}//后面不是破折号
							 
							 
			 }
			} /////////////////////////////////////////////////////////////////////////////////////////
					
			}
			}//在姓氏表中


		}//if((j==endpos)||(true==if_Punctuationgbk(returnReg.res,j-1)))
						
	}//if(if_englishname==0)


				   //判断是否成词语
                   
				   			   
				   ////
				   if(max_long<j)
				   {    
					   if(true==it->second->if_wors)
					      { max_long=j;}
					   if(j==returnReg.res.size()-1)
					   {
						  endpos=j+1;
						   pos_map.insert(pair<int,int>(begpos,endpos));
						   i=returnReg.res.size()-1;
						   break;					   }
				   }
				   p=it->second;
				  // break;
			   }
		   }

	   }
	    //查字典树，对输入的长句进行切分



	   //测试切分结果
	   /*
	    map<int,int>::iterator it;
		for(it=pos_map.begin();it!=pos_map.end();it++)
		{
			cout<<"begin"<<it->first<<"     end"<<it->second<<"  "<<vecTostr(if_utf8,returnReg.res,it ->first,it->second)<<endl;
			
		}
	   */
/************************根据切分的结果集，依次分词取出最大概率**************/
		
		map<int,int>::iterator it1;
		for(it1=pos_map.begin();it1!=pos_map.end();it1++)
		{
			//对单句进行切分，取出得分最高的情况
			int K=it1->second-it1->first;
		   
			if(K<=2)//2个一下的字
		     {
				// string sttrr=vecTostr(if_utf8,returnReg.res,it1->first,it1->second);
				// str_return+=sttrr;
				// str_return+="/ ";
				 returnReg.position.insert(pair<unsigned int , unsigned int>(it1->first,it1->second));
				 continue;
  
		     }
            //大于2个字，但是一个词的情况
			
							
				double ** D = new double *[K];
				for(int i=0;i<K;i++)				
				{
					D[i] = new double[K];
				}
				//初始化矩阵
				for(int v=0;v<K;v++)
				{
					for(int x=0;x<K;x++)
					{
						D[v][x]=8000000;
					}
				}
				//初始化矩阵
			
				for(int n=it1->first,h=0;n<it1->second;++n,++h)//对每一个短语进行分词
				{   int nn=n;
					p =mytree.root;
					while(p->node_map.end()!=p->node_map.find(returnReg.res[nn]))
					{
						TrieNodeMap::iterator it2=p->node_map.find(returnReg.res[nn]);

						p=it2->second;
						if(p->if_wors==true)
						{										
						  double temp=mytree.node_infomation[p->nodeInfo_pos].freq;
						  temp=temp/400000000000;
						  temp=log(temp)/sqrt(double(mytree.node_infomation[p->nodeInfo_pos].wLen));
						  D[h][nn-it1->first]=0-temp;
						 }
				   
						if(nn+1<it1->second)
						{
							nn++;
						}
								
					}

				}
				/*
				for(int v=0;v<K;v++)
				{
					for(int x=0;x<K;x++)
					{
						cout<<D[v][x]<<" ";
					}
					cout<<endl;
				}
				*/
          /**************************通过姓氏修改矩阵*****************************************/
				
				if(true==if_name(if_utf8,returnReg.res,it1->first))
				{   //1、姓氏加称谓
					
					p =mytree.root;
					int t_wordlong=1;
					int t_temp01=it1->first+1;
					string word_flg="";
					bool ifnameend=check_ifword(if_utf8,p,mytree,word_flg,t_temp01,returnReg.res);
					if((true==ifnameend)&&(word_flg.find("nd")!=word_flg.npos))
					{
						t_wordlong+=(t_temp01-it1->first)-2; 
						if(t_wordlong<=it1->second)
						{
							D[0][t_wordlong]=1;
						}						
					}

					else
					{   double _secondnamefre=0;
						for(int jj=1;jj<3;++jj)
						{				
							double _namefre1=countnamefre(if_utf8,returnReg.res,it1->first+jj,namefre);
							if(1==jj)
							{ 
	                          	_secondnamefre=countnamefre(if_utf8,returnReg.res,it1->first+jj,secondnamefre);
							}
					
							if(0==_namefre1)
							{	}
							else
							{
								if(D[0][jj]>1000)
								{ 
									
									//double n=1/3;
									if(1==jj)
										
										D[0][jj]=350/(pow(_namefre1,0.4));
									else
									{   
										TrieNode *R=mytree.root;
										bool temp=if_nameendisword(R,mytree,it1->first+2,returnReg.res);
										if(false==temp)
										{
											if(0!=_secondnamefre)
											{
												D[0][jj]=(200/pow(_secondnamefre,0.4))*110/pow(_namefre1,0.4);
											}
										}


									}   


								}

							}
						}
					}

					
				}
          /*****************通过姓氏修改矩阵**************************************************/

			//test D
			/*
			for(int v=0;v<K;v++)
			{
				for(int x=0;x<K;x++)
				{
					cout<<D[v][x]<<" ";
				}
				cout<<endl;
			}
			*/
			

			//对矩阵求最小路径(0---->K-1)
				if(D[0][K-1]<100)
				{
					//string sttrr=vecTostr(if_utf8,returnReg.res,it1->first,it1->second);
					//str_return+=sttrr;
					//str_return+="/ ";
					returnReg.position.insert(pair<unsigned int , unsigned int>(it1->first,it1->second));
				}

				else
				{
					double score=0;
					map<double,string>dmap;//存储源点到达该点的距离，路径
					double *A=new double[K];//
					string  *B=new string[K];
					double W=0;
					string w_str="";
					for(int ii=0;ii<K;++ii)
					{

						W=D[0][ii];

						stringstream ss;  
						ss<<ii;  
						w_str=ss.str();
						
						for(int pp=0;pp<ii;++pp)
						{   
							double xx=D[pp+1][ii];
							if(W>(A[pp]+D[pp+1][ii]))
							{
								W=A[pp]+D[pp+1][ii];//加入路径
								ss.str("");
								ss<<ii;  
								w_str=B[pp]+ss.str();		
							
							}

						}

						A[ii]=W;
						B[ii]=w_str+"/";
						
					}
                				 
				string wa=B[K-1];
				int temp_n=0;
				while(string::npos!=wa.find("/"))
				{
					string temp_sttr=string(wa,0,wa.find("/"));
					wa=string(wa,wa.find("/")+1);
					int tem_int=atoi(temp_sttr.c_str());
					//str_return+= vecTostr(if_utf8,returnReg.res,it1->first+temp_n,it1->first+(tem_int+1));   
					//str_return+="/ ";

					returnReg.position.insert(pair<unsigned int , unsigned int>(it1->first+temp_n,it1->first+(tem_int+1)));
					temp_n=tem_int+1;
				}

    			delete []A;
				delete []B;
				
				}
			
				//对矩阵求最小路径
				for(int vv=0;vv<K;++vv)
				{
					delete[]D[vv];
				}
				delete []D;

				

        }

			
			

        
	

	return returnReg;



	}
	

}


