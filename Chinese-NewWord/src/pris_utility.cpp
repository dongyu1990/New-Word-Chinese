//#include "StdAfx.h"
#include "pris_utility.h"
#include <string>
//using std::string;


namespace pris
{

	using namespace std;  // it's ok to do this in .cpp file


	/*****************************
	����: ȥ��stringǰ��Ŀհ��ַ�
	˵��: �հ��ַ�����'\n''\t'' '
	*****************************/
	string& strip(string &s)
	{
		string::size_type beg, end;
		string::size_type length = s.size();
		for(beg=0; isspace(s[beg])&&beg!=length; ++beg);

		if(beg == length) 
		{
			s.clear();
			return s;
		}

		for(end=beg; !isspace(s[end])&&end!=length; ++end);

		s = s.substr(beg, end-beg);
		return s;
	}

	/**************************
	����: int����תstring
	**************************/
	string itos(int n)
	{
		string s;
		int sign = 0;
		if( (sign=n) < 0)
			n = -n;
		do
		{
			s.push_back(n%10 + '0');
		}while((n/=10) > 0);

		if(sign < 0) s.push_back('-');

		string::size_type i = 0, j = s.size()-1;
		char c;
		for(; i<j; ++i, --j)
		{
			c = s[i];
			s[i] = s[j];
			s[j] = c;
		}
		return s;
	}

	/**************************
	����: long����תstring
	**************************/
	string ltos(long n)
	{
		return tos(n);
	}

	/******************************
	����: stringתint
	˵��: ����ģ�庯��ston(...)ʵ��
	******************************/
	int stoi(const string &s)
	{
		return ston<int>(s);
	}
    float stof(const string &s)
    {
        float f;
        f = atof(s.c_str());
        return f;
    }
	/******************************
	����: stringתlong
	˵��: ����ģ�庯��ston(...)ʵ��
	******************************/
	long stol(const string &s)
	{
		return ston<long>(s);
	}
	
	/******************************
	����: CstringתString
	˵��: 
	******************************/
	/*
	string CStr2Str(CString cstr)
	{  
		char *chr=new char[cstr.GetLength()];
		WideCharToMultiByte(CP_ACP,0,cstr.GetBuffer(),-1,chr,cstr.GetLength(),NULL,NULL);
		string str = chr;
		return str;
	}
	*/

//	string CStr2Str(CString cstr)
//	{  
//		string str(cstr.GetBuffer());
//		cstr.ReleaseBuffer();
//		return str;
//	}

	/******************************
	����: StringתCString
	˵��: 
	******************************/
//	CString Str2CStr(string str)
//	{
//		
//		CString cstr=str.c_str();
//		return cstr;
//	}
	/******************************
	����: Stringתchar*
	˵��: 
	******************************/
	char * Str2Char(string str)
	{
		const char * cpcr = str.c_str();
        int lenth = strlen(cpcr);
		char * pcr = new char[strlen(cpcr) + 1];
		strcpy(pcr,cpcr);
		return pcr;

	}
	/******************************
	����: CStringתint
	˵��: 
	******************************/
//	int CStr2int(CString str)
//	{
//		int i=_ttoi(str);
//		return i;
//	}
}
