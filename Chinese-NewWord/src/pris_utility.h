#ifndef PRISUTILITIES_H
#define PRISUTILITIES_H
#include<iostream>
#include <sstream>
#include <string>
#pragma once
// �ṩ����ͨ�ú���, ������ϵͳ����
using std::string;
using std::stringstream;

namespace pris
{
	/*******************************
	����: ģ�庯��֮��������תstring
	˵��: sstream����ʵ��
	*******************************/	
	template <typename Type>
	string tos(const Type &T)
	{
		stringstream ss;
		ss << T;
		return ss.str();
	}
	
	/*******************************
	����: ģ�庯��֮stringת����
	*******************************/
	template <typename nType>
	nType ston(const string &s)
	{
		string::size_type i = 0, length = s.size();
		nType n = 0, sign = 1;
		for(i=0; isspace(s[i])&&i!=length; ++i);

		sign = (s[i] == '-') ? -1 : 1;

		if(s[i] == '+' || s[i] == '-')
			++i;

		for(n=0; isdigit(s[i])&&i!=length; ++i)
			n = n*10 + (s[i]-'0');

		return sign * n;
	}
	string& strip(string &s);
	string itos(int n);
	string ltos(long n);
	int stoi(const string &s);
	long stol(const string &s);
	//string CStr2Str(CString cstr);
	//CString Str2CStr(string str);
	char * Str2Char(string str);
    float stof(const string &s);
	//int CStr2int(CString);

}

#endif  
