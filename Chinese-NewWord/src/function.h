#ifndef _FUNCTION_H
#define _FUNCTION_H
#include<iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Type.h"
#include<ctime>
#include "Trie.h"
#include <stdint.h>
#include "str_function.h"
// 提供各种通用函数, 供整个系统调用
using namespace std;;


namespace Seg
{	

	Trie  Initialization();
	bool Loaddict(Trie& mytree,bool if_utf8,const char * const filePath);


	//Termlistmap get_allterm(string str);

	//string str_segement(string str,const char * const  dict_path);
	double geteverywordfre(bool if_utf8,Trie& mytree,string str);
	string segement(bool if_utf8,Trie& mytree,string str);
	
	Termlistmap getallterm(Trie& mytree,bool if_utf8,string str);
	ReSeg str_segementutf8tohmm(bool if_utf8,Trie& mytree,string str);


}

#endif