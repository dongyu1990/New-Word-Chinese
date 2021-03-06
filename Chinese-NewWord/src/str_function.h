#ifndef _STR_FUNCTION_H
#define _STR_FUNCTION_H
#include <iostream>	// for getline
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h> // uint16_t
#include "Type.h"
#include<ctime>
#include "Trie.h"
#include <sstream>
#include <string>
#include <vector>
#include "Type.h"
#include<ctime>
#include "Trie.h"
#include <stdint.h>
using namespace std;
namespace Seg
{
	/****************************************************************@唐波*/
	bool strExtract(string &s);
	void trim(string &s);
	bool getLine(ifstream& ifile, string& s);
	bool splitStr(const string& source, vector<string>& res, const string& pattern);
	inline uint16_t twocharToUint16(char high, char low)
	{
		return (((uint16_t(high) & 0x00ff ) << 8) | (uint16_t(low) & 0x00ff));
	}
	bool utf8ToVec(const string& str, vector<uint16_t>& vec);
	string vecToUtf8(Unicode::const_iterator begin, Unicode::const_iterator end);
	bool gbkToVec(const string& str, vector<uint16_t>& vec);
	string vecToGbk(Unicode::const_iterator begin, Unicode::const_iterator end);

	typedef bool (* PstrToVec)(const string& str, vector<uint16_t>& vec);
	typedef string (*PvecToStr_tb)(Unicode::const_iterator begin, Unicode::const_iterator end);
	typedef string (*PvecToStr_wxy)(Unicode vec, int bg,int ed);

	inline bool strStartsWith(const string& str, const string& prefix)
	{
		//return str.substr(0, prefix.size()) == prefix;
		if(prefix.length() > str.length())
		{
			return false;
		}
		return 0 == str.compare(0, prefix.length(), prefix);
	}

	
	/******************************************************************@唐波*/

	/******************************************************************@王星雅*/
	inline uint16_t twocharToint16(char high, char low)
	{
		return (((uint16_t(high) & 0x00ff ) << 8) | (uint16_t(low) & 0x00ff));
	}	
	inline pair<char, char> int16ToChar2(uint16_t in)
	{
		pair<char, char> res;
		res.first = (in>>8) & 0x00ff; //high
		res.second = (in) & 0x00ff; //low
		return res;
	}
	bool split(string str, vector<string>& v);
	bool gbkToVec(const string& str, vector<uint16_t>& vec);	
	//bool utf8ToVec(const string& str, vector<uint16_t>& vec);
	string vecToUtf8(Unicode vec, int bg,int ed);
	string vecToGbk(Unicode vec, int bg,int ed);
	void printtime();
	bool if_Punctuationgbk(vector<uint16_t> &line,unsigned int i);//检查是否为标点

	bool if_Punctuationutf8(vector<uint16_t> &line,unsigned int i);
	bool if_Punctuation(bool if_utf8,vector<uint16_t> &line,unsigned int i);

	bool if_connectgbk(vector<uint16_t> &line,unsigned int i);//检查是否为标点

	bool if_connectutf8(vector<uint16_t> &line,unsigned int i);
	bool if_connect(bool if_utf8,vector<uint16_t> &line,unsigned int i);

	bool if_namegbk(vector<uint16_t> &line,unsigned int i);	
	bool if_nameutf8(vector<uint16_t> &line,unsigned int i);
	bool if_name(bool if_utf8,vector<uint16_t> &line,unsigned int i);
	bool getnamefre(const char * const name_path,map<string,unsigned int>& namefre);
	bool strTovec(bool if_utf8,const string& str, vector<uint16_t>& vec);
	string vecTostr(bool if_utf8,Unicode vec, int bg,int ed);
	bool check_ifword(bool if_utf8,TrieNode* tp,Trie& mytree,string& flg,int& i,vector<uint16_t>& vec);//检查是否和后面成词
	unsigned int countnamefre(bool if_utf8,vector<uint16_t>line,int i,map<string,unsigned int>name);
	bool if_nameendisword(TrieNode* R,Trie& mytree,int i,vector<uint16_t>& line);//第3个字是否独立成词
	bool check_ifnameword(bool if_utf8,TrieNode* tp,Trie& mytree,string& flg,int& i,vector<uint16_t>& vec);//判断姓氏是否和后面成词
	char * Str2Char(string str);
	bool ifMeasureword(string str);
	/******************************************************************@王星雅*/


}
#endif