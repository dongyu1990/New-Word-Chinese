#include<iostream>
#include "Trie.h"
#include "function.h"
#include "GST.h"
#include<map>
#include "str_function.h"
#include "Type.h"
#include "entry.h"
#include <ctime>
#include <math.h>
#include<stdlib.h>
#include <fstream>
#include <sstream>
using namespace std;
using namespace Seg;
int main()
{ 	
	wordlist1 NewWord=GetnewWord(0, "..\\dicts\\jieba.dict.gbk");
	

	

   /*************新词发现******************************************************************/

	system("pause");
  

	return 0;
}
	//Trie mytree;
	//const char * filePath="H:\\C++\\dicts\\jieba.dict.gbk";
	//bool OK=Seg::Loaddict(mytree,0,filePath);
	
	/*
	ifstream ifile("H:\\C++\\Seg\\txt\\D.txt");
	//cout<<"1";
	string str="";
	string ttt="";
	ofstream fout("H:\\C++\\Seg\\txt\\output.txt");
	//fout.open("H:\\C++\\Seg\\txt\\output.txt");
	while(getline(ifile, str))
	{  	
		if(str.size()<1)
			continue;
		ttt=Seg::segement(0,mytree,str);
		fout<<ttt;
	}
	
	*/	
	//ReSeg R=Seg::str_segementutf8tohmm(0,mytree,str);
	/*
	CPI将步入3%-3.5%区间
	ifstream in("H:\\C++\\Seg\\txt\\C.txt", ios::in);
	istreambuf_iterator<char> beg(in), end;
	string str(beg, end);
	in.close();   
	cout<<str;
	*/
	//ReSeg R=Seg::str_segementutf8tohmm(0,mytree,str);
	/*
	map<unsigned int,unsigned int>::iterator it=R.position.begin();
	for(;it!=R.position.end();++it)
	{
		cout<<"beg  "<<it->first<<"end  "<<it->second<<endl;

	}

	*/

