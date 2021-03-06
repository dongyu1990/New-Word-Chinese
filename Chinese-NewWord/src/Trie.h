#ifndef _TRIE_H
#define _TRIE_H
#include "Type.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <stdint.h>
namespace Seg
{
	struct TrieNode//树节点信息
	{
		TrieNodeMap node_map;//存储孩子节点
		bool if_Leaf;
		bool if_wors;
		int nodeInfo_pos;//记录该词在链表中的位置
		TrieNode()
		{
			if_Leaf = true;
			nodeInfo_pos = 0;
			if_wors=false;
		}
	
	};

	struct  TrieNodeInfo//节点信息，存在列表里
	{
		string word;//词
		size_t wLen;// the word's len , not string.length(), 
		size_t freq;//词频
		string tag;//词性
		TrieNodeInfo():wLen(0),freq(0)
		{
		}

	};
	class Trie
	{   
     public:
	    TrieNode* root;	
		vector<TrieNodeInfo> node_infomation;
		Trie();
		//~Trie();
		bool init();//
		bool loadDict(bool if_utf8,const char * const filePath);//
		void dispose();//
		// //bool Trie::insert(const TrieNodeInfo& nodeInfo)
		bool deleteNode(TrieNode* node);
		bool insert(bool if_utf8,TrieNodeInfo& nodeInfo);
		
		// 查找词语，返回所在的列表中的偏移量
		TrieNodeInfo* find(bool if_utf8,string str);
	};
	
}

#endif