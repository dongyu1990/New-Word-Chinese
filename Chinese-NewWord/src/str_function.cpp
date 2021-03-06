#include "str_function.h"
#include <stddef.h>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include<stdlib.h>
#include <algorithm>
using namespace std;// size_t Unsigned integral type 便于不同位系统移植
namespace Seg
{
	void trim(string &s)
	{
		string whitespaces(" \t\f\v\n\r");//
		size_t start = s.find_first_not_of(whitespaces);
		if(start==string::npos) // 说明为空行
		{
			s.clear();//清空 否则
			return;
		}
		size_t len = s.find_last_not_of(whitespaces) - s.find_first_not_of(whitespaces) + 1;
		s.assign(s, start, len);
		//cout<<s.length()<<endl;
		//return s;
	}
	bool strExtract(string &s)
	{
		trim(s);
		if(s.empty()||(s[0]=='#'))// 以#为注释部分
			return false;
		else
			return true;
	}

	bool getLine(ifstream& ifile, string& s)// 提取一条有用的字符串
	{
		while(!ifile.eof())
		{
			getline(ifile,s);
			if(strExtract(s))
				return true;
			else
				continue;
		}

		return false;

	}
	bool splitStr(const string& source, vector<string>& res, const string& pattern)
	{
		if(source.empty())
		{
			return false;
		}
		res.clear();

		size_t start = source.find_first_not_of(pattern);
		size_t end;
		if(string::npos == start)
		{
			return false;
		}
		while(string::npos != start)
		{
			end = source.find_first_of(pattern, start);
			if(string::npos == end)
			{
				res.push_back(source.substr(start));
				return true;
			}
			res.push_back(source.substr(start, end - start));
			start = source.find_first_not_of(pattern,  end);
		}
		return true;
	}

	bool utf8ToVec(const string& str, vector<uint16_t>& vec)
	{
		char ch1, ch2;
		if(str.empty())
		{
			return false;
		}
		vec.clear();
		size_t siz = str.size();
		for(unsigned int i = 0;i < siz;)
		{
			if(!(str[i] & 0x80)) // 0xxxxxxx 1Byte
			{
				vec.push_back(str[i]);
				i++;
			}
			else if ((unsigned char)str[i] <= 0xdf && i + 1 < siz) // 110xxxxxx 2B
			{
				ch1 = (str[i] >> 2) & 0x07;
				ch2 = (str[i+1] & 0x3f) | ((str[i] & 0x03) << 6 );
				vec.push_back(twocharToUint16(ch1, ch2));
				i += 2;
			}
			else if((unsigned char)str[i] <= 0xef && i + 2 < siz)// 1110xxx 3B
			{
				ch1 = (str[i] << 4) | ((str[i+1] >> 2) & 0x0f );
				ch2 = ((str[i+1]<<6) & 0xc0) | (str[i+2] & 0x3f); 
				vec.push_back(twocharToUint16(ch1, ch2));
				i += 3;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	string vecToUtf8(Unicode::const_iterator begin, Unicode::const_iterator end)
	{
		if(begin >= end)
		{
			return "";
		}
		string res;
		uint16_t ui;
		while(begin != end) // 不包括 end
		{
			ui = *begin;
			if(ui <= 0x7f)
			{
				res += char(ui);
			}
			else if(ui <= 0x7ff)
			{
				res += char(((ui>>6) & 0x1f) | 0xc0);
				res += char((ui & 0x3f) | 0x80);
			}
			else
			{
				res += char(((ui >> 12) & 0x0f )| 0xe0);
				res += char(((ui>>6) & 0x3f )| 0x80 );
				res += char((ui & 0x3f) | 0x80);
			}
			begin ++;
		}
		return res;
	}
	string vecToGbk(Unicode::const_iterator begin, Unicode::const_iterator end)
	{
		if(begin >= end)
		{
			return "";
		}
		pair<char, char> pa;
		string res;
		while(begin != end)
		{
			pa = int16ToChar2(*begin);
			if(pa.first & 0x80)
			{
				res += pa.first;
				res += pa.second;
			}
			else
			{
				res += pa.second;
			}
			begin++;
		}
		return res;
	}
	/***********************************************************************************************************************/

	/**********************************************
	功能：切分读取字典的每一行
	参数：（单行字符，vector)
	返回值：vector
	***********************************************/
	bool split(string str, vector<string>& v)//单个字符切割函数
	{   string temp="";
	    char s=' ';
	    for(unsigned int i=0;i<str.size();i++)
		{
			if(str[i]!=s)
			  {
				  temp+=str[i];
				  
			  }
			else
			  {
				  v.push_back(temp);
				  temp="";
				  
			  }
		}
		v.push_back(temp);
		temp="";
		return true;
	}

	void printtime()
	{
		time_t now_time;
		now_time = time(NULL);//就是个大整数
		cout<<now_time<<endl;
	}


	/**********************************************
	功能：读取gbk文件到vector
	参数：（str，vector)
	返回值：vector存入每一个汉字的编码值
	***********************************************/
	bool gbkToVec(const string& str, vector<uint16_t>& vec)
	{  
		vec.clear();
		if(str.empty())
		{
			return false;
		}
		//uint16_t
		unsigned int i = 0;
		while(i < str.size())
		{
			if(0 == (str[i] & 0x80))
			{
				vec.push_back(uint16_t(str[i]));
				i++;
			}
			else
			{
				if(i + 1 < str.size()) //&& (str[i+1] & 0x80))
				{
					vec.push_back(twocharToint16(str[i], str[i + 1]));
					i += 2;
				}
				else
				{
					return false;
				}
			}
		}
		return true;
	}


	/**********************************************
	功能：vector to Utf8
	参数：（单行字符，vector)
	返回值：汉字字符串
	***********************************************/
	string vecToUtf8(Unicode vec, int bg,int ed)
	{
		if(bg >= ed)
		{
			return "";
		}
		string res;
		uint16_t ui;
		while(bg != ed)
		{
			ui = vec[bg];
			if(ui <= 0x7f)
			{
				res += char(ui);
			}
			else if(ui <= 0x7ff)
			{
				res += char(((ui>>6) & 0x1f) | 0xc0);
				res += char((ui & 0x3f) | 0x80);
			}
			else
			{
				res += char(((ui >> 12) & 0x0f )| 0xe0);
				res += char(((ui>>6) & 0x3f )| 0x80 );
				res += char((ui & 0x3f) | 0x80);
			}
			bg++;
		}
		return res;
	}

	
	/**********************************************
	功能：vector to gbk
	参数：（单行字符，vector)
	返回值：汉字字符串
	***********************************************/
	string vecToGbk(Unicode vec, int bg,int ed)
	{
		if(bg > ed)
		{
			return "";
		}
		pair<char, char> pa;
		string res;
		while(bg != ed)
		{
			pa = int16ToChar2(vec[bg]);
			if(pa.first & 0x80)
			{
				res += pa.first;
				res += pa.second;
			}
			else
			{
				res += pa.second;
			}
			bg++;
		}
		return res;
	}
	bool if_Punctuationgbk(vector<uint16_t> &line,unsigned int i)//检查是否为标点
	{
		string biao_dian=",;，：.．•:“”<>‘’、~～ \n  ｛｝【】[]/ ()@#@#＠＃……¨〖々〗…＆×（）、（）×＆……＃＠。。！＿_＋，｀。；？?!！《》";
		vector<uint16_t> simbol;
		bool if_simbolok=gbkToVec(biao_dian, simbol);
		if(!if_simbolok)
		{
			cout<<"error2";
		}
		if(line[i]==32)
			return true;
		vector<uint16_t>::iterator simresult = find( simbol.begin(),simbol.end(),line[i]); //查找3
		if ( simresult == simbol.end())
		{
			return false;
		}
		else
			return true;

	}
	bool if_Punctuationutf8(vector<uint16_t> &line,unsigned int i)
	{   
		string biao_dian=",;，：.．•:“”<>‘’、~～ \n  ｛｝【】[]/ ()@#@#＠＃……¨〖々〗…＆×（）、（）×＆……＃＠。。！＿_＋，｀。；？?!！《》";
		vector<uint16_t> simbol;
		bool if_simbolok=utf8ToVec(biao_dian, simbol);
		if(!if_simbolok)
		{
			cout<<"error2";
		}
		if(line[i]==32)
			return true;
		vector<uint16_t>::iterator simresult = find( simbol.begin(),simbol.end(),line[i]); //查找3
		if ( simresult == simbol.end())
		{
			return false;
		}
		else
			return true;
	}
	
	bool if_Punctuation(bool if_utf8,vector<uint16_t> &line,unsigned int i)
	{
		bool if_Punc;
		if(false==if_utf8)
		{
			if_Punc=if_Punctuationgbk(line,i);
		}
		else
		{
			if_Punc=if_Punctuationutf8(line,i);
		}
		return if_Punc;
	}
	bool if_connectgbk(vector<uint16_t> &line,unsigned int i)
	{
		string biao_dian="$%￥％0123456789·•０１２３４５６７８９qwertyuioplkjhgfdsazxcvbnm/QWERTYUIOPLKJHGFDSAZXCVBNM";
		vector<uint16_t> simbol;
		bool if_simbolok=gbkToVec(biao_dian, simbol);
		if(!if_simbolok)
		{
			cout<<"error2";
		}

		vector<uint16_t>::iterator simresult = find( simbol.begin(),simbol.end(),line[i]); //查找3
		if ( simresult == simbol.end())
		{
			return false;
		}
		else
			return true;
	}
	bool if_connectutf8(vector<uint16_t> &line,unsigned int i)
	{
		string biao_dian="$%￥％0123456789０１２３４·•５６７８９qwertyuioplkjhgfdsazxcvbnm/QWERTYUIOPLKJHGFDSAZXCVBNM";
		vector<uint16_t> simbol;
		bool if_simbolok=utf8ToVec(biao_dian, simbol);
		if(!if_simbolok)
		{
			cout<<"error2";
		}

		vector<uint16_t>::iterator simresult = find( simbol.begin(),simbol.end(),line[i]); //查找3
		if ( simresult == simbol.end())
		{
			return false;
		}
		else
			return true;
	}
	bool if_connect(bool if_utf8,vector<uint16_t> &line,unsigned int i)
	{
		bool if_con;
		if(false==if_utf8)
		{
			if_con=if_connectgbk(line,i);
		}
		else
		{
			if_con=if_connectutf8(line,i);
		}
		return if_con;
	}	
	bool if_namegbk(vector<uint16_t> &line,unsigned int i)
	{
		string chinaname="敖白班包宝保鲍毕边卞柏蔡曹岑柴昌常车陈成程迟池褚楚储淳崔刀邓狄刁丁董窦杜端段樊范方房斐丰封冯凤伏傅甘高耿龚宫勾苟辜谷顾官关管桂郭韩杭郝禾何贺衡洪侯胡浣花黄稽姬纪季简翦姜江金蒋焦晋靳荆居康空孔匡邝况蓝郎朗乐冷李理厉励连廉练良梁廖凌刘柳隆龙楼娄吕陆路林骆马麻满茅毛梅孟苗缪闵明牟倪聂牛钮农潘庞裴彭皮朴平蒲溥浦戚祁钱强秦丘邱仇裘屈瞿权饶任荣容阮瑞芮赛单商邵佘申沈盛石寿舒宋苏孙邰谭谈汤唐陶滕田佟仝屠涂汪王韦魏危卫蔚闻翁巫邬武吴奚习夏鲜席向冼项萧解辛邢幸熊徐许宣薛荀颜阎言严彦晏燕杨阳姚叶蚁易殷银尹应英游于於鱼虞俞余禹喻郁尉袁岳云臧曾查翟湛张赵甄郑钟周朱竺祝庄卓宗邹祖左肖";
		vector<uint16_t> C_name;
		bool getname=gbkToVec(chinaname,  C_name);
		vector<uint16_t>::iterator nameresult = find( C_name.begin(),C_name.end(),line[i]); //查找中文姓氏
		if (nameresult==C_name.end())
		{
			return false;
		}
		else
		{
			return true;
		}

	}
	bool if_nameutf8(vector<uint16_t> &line,unsigned int i)
	{

		string chinaname="敖白班包宝保鲍毕边卞柏蔡曹岑柴昌常车陈成程迟池褚楚储淳崔刀邓狄刁丁董窦杜端段樊范方房斐丰封冯凤伏傅甘高耿龚宫勾苟辜谷顾官关管桂郭韩杭郝禾何贺衡洪侯胡浣花黄稽姬纪季简翦姜江金蒋焦晋靳荆居康空孔匡邝况蓝郎朗乐冷李理厉励连廉练良梁廖凌刘柳隆龙楼娄吕陆路林骆马麻满茅毛梅孟苗缪闵明牟倪聂牛钮农潘庞裴彭皮朴平蒲溥浦戚祁钱强秦丘邱仇裘屈瞿权饶任荣容阮瑞芮赛单商邵佘申沈盛石寿舒宋苏孙邰谭谈汤唐陶滕田佟仝屠涂汪王韦魏卫蔚闻翁巫危邬武吴奚习夏鲜席向冼项萧解辛邢幸熊徐许宣薛荀颜阎言严彦晏燕杨阳姚叶蚁易殷银尹应英游于於鱼虞俞余禹喻郁尉袁岳云臧曾查翟湛张赵甄郑钟周朱竺祝庄卓宗邹祖左肖";
		vector<uint16_t> C_name;
		bool getname=utf8ToVec(chinaname,  C_name);
		vector<uint16_t>::iterator nameresult = find( C_name.begin(),C_name.end(),line[i]); //查找中文姓氏
		if (nameresult==C_name.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool if_name(bool if_utf8,vector<uint16_t> &line,unsigned int i)
	{
		bool ifname;
		if(false==if_utf8)
		{
			ifname=if_namegbk(line,i);
		}
		else
		{
			ifname=if_nameutf8(line,i);
		}
		return ifname;
	}
	bool getnamefre (const char * const name_path,map<string,unsigned int>& namefre)
	{
		//检查目录等等
		ifstream ifile(name_path);
		string line;
		vector<string> temp;
		
		while(getline(ifile, line))
		{
			temp.clear();
			Seg::split(line, temp);
			if(2< temp.size())
			{
				return false;
			}
			//切分每一行
			namefre.insert(pair<string,unsigned int>(temp[0],atoi(temp[1].c_str())));
	    }
		return true;
	}
	
	bool strTovec(bool if_utf8,const string& str, vector<uint16_t>& vec)
	{  
		bool if_OK;
		if(false==if_utf8)
			if_OK=gbkToVec(str,vec);
		else
			if_OK=utf8ToVec(str,vec);
		return if_OK;
	}
	string vecTostr(bool if_utf8,Unicode vec, int bg,int ed)
	{
		string re_str="";
		if(false==if_utf8)
		{
			re_str=vecToGbk(vec,bg,ed);
		}
		else
		{
			re_str=vecToUtf8(vec,bg,ed);
		}
		return re_str;
	}
	/*
	功能：判断第3个字是否和后面成词
	参数：字符类型，
	
	*/
    bool check_ifword(bool if_utf8,TrieNode* tp,Trie& mytree,string& flg,int& i,vector<uint16_t>& vec)
	{
		/////////////////////////////////////////判断第3个字是否和后面成词
		bool checkword=false;
		if(true==if_name(if_utf8,vec,i))
		{  
			i+=2;
			return true;
		}
		while(i+1<vec.size())
		{		 
			if(tp->node_map.end() == tp->node_map.find(vec[i]))
			{												
				break;
			}
			else
			{
				TrieNodeMap::iterator temp33=tp->node_map.find(vec[i]);
				if(true==temp33->second->if_wors)
				{  
					checkword=true;
				   	flg=mytree.node_infomation[temp33->second->nodeInfo_pos].tag;
					i++;
					tp=temp33->second;
				}
				else
				{
					i++;
					tp=temp33->second;
				}
			}
		}//while检查是否为词语
	
		return checkword;

	}
	unsigned int countnamefre(bool if_utf8,vector<uint16_t>line,int i,map<string,unsigned int>name)
	{;
		unsigned int _namef=0;
		string _strname=vecTostr(if_utf8,line,i,i+1);
		map<string,unsigned int>::iterator findname=name.find(_strname);
		if(findname!=name.end())
		{
			_namef=findname->second;
		}
		return _namef;
	}
	bool if_nameendisword(TrieNode* R,Trie& mytree,int i,vector<uint16_t>& line)
	{
		//TrieNode *R=mytree.root;

		//判断第3字是否独立成词
		bool temp=false;;
		if(R->node_map.end() == R->node_map.find(line[i]))
		{
			//temp=1;
		}
		else
		{
			TrieNodeMap::iterator ittt=R->node_map.find(line[i]);
			R=ittt->second;
			//加上定义，这里地个字不能为名词
			if((R->if_wors==1)&&(mytree.node_infomation[R->nodeInfo_pos].freq>10000)&&(mytree.node_infomation[R->nodeInfo_pos].tag!="n"))//自定义规则
				temp=true;
			else
				temp=false;
		}
		return temp;
	}
	/*判断姓氏是否和后面成词*/
	bool check_ifnameword(bool if_utf8,TrieNode* tp,Trie& mytree,string& flg,int& i,vector<uint16_t>& vec)
	{
		
		bool checkword=false;
		while(i<vec.size())
		{		 
			if(tp->node_map.end() == tp->node_map.find(vec[i]))
			{												
				break;
			}
			else
			{
				TrieNodeMap::iterator temp33=tp->node_map.find(vec[i]);
				if(true==temp33->second->if_wors)
				{
					checkword=true;
					flg=mytree.node_infomation[temp33->second->nodeInfo_pos].tag;
					//break;
					i++;
					tp=temp33->second;
				}
				else
				{
					i++;
					tp=temp33->second;
				}
			}
		}//while检查是否为词语

		return checkword;

	}

	char * Str2Char(string str)
	{
		const char * cpcr = str.c_str();
		int lenth = strlen(cpcr);
		char * pcr = new char[strlen(cpcr) + 1];
		strcpy(pcr,cpcr);
		return pcr;

	}
	bool ifMeasureword(string str)
	{
		string measureword="次串部个份件亩双节幅栋幢口句台只名吨间届把捆排摊摞场块垛堆斤本根棵片章盆盏盘粒筒篇箱颗袋亿 " ;
		if(measureword.find(str)!=string::npos)
		{
			return true;
		}
		return false;
	}

}




























