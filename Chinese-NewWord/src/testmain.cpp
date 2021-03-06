#include <iostream>
#include "TrainHMM.h"
#include "SegHMM.h"
#include "Interface.h"
#define TrainSourcePath "raw.dic" //char *
#define ModelPath "hmm_model.utf8" //char *
using namespace Seg;

int main()
{
	// //string filepath(TrainSourcePath);
	// //RawDicProcess(TrainSourcePath);
	// HMMSeg hmm;
	// loadModel(ModelPath,hmm);
	 vector<string> res;

	// cut(hmm,"微博", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"可爱乡村非主流", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"高端大气上档次", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"狂拽酷霸屌炸天", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"低调奢华有内涵", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"装模作样绿茶婊", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"低端粗俗甩节操", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"贵族王朝杀马特", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"可爱乡村非主流", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"卖萌嘟嘴剪刀手", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;

	// cut(hmm,"土憋矫情无下限", res);
	// for (int i = 0; i < res.size() ; i++)
	// 	cout<<res[i]<<"/";
	// cout<<endl;
	HMMSeg hmm;
	ReSeg reviseSeg;
	loadModel(ModelPath,hmm);
	reviseSeg.res.push_back(0x4E00);
	reviseSeg.res.push_back(0x4E07);
	reviseSeg.res.push_back(0x4E08);
	reviseSeg.res.push_back(0x4E00);
	reviseSeg.res.push_back(0x9648);
	reviseSeg.res.push_back(0x5C0F);
	reviseSeg.res.push_back(0x6167);

	reviseSeg.position[0]=1;
	reviseSeg.position[1]=2;
	reviseSeg.position[2]=4;
	reviseSeg.position[4]=5;
	reviseSeg.position[5]=6;
	reviseSeg.position[6]=7;
	
	revise(hmm,reviseSeg);
    
    reviseSeg.res.clear();
	reviseSeg.position.clear();
	reviseSeg.res.push_back(0x5F90);
	reviseSeg.res.push_back(0x524D);
	reviseSeg.res.push_back(0x65B9);
	reviseSeg.position[0]=1;
	reviseSeg.position[1]=2;
	reviseSeg.position[2]=3;

	revise(hmm,reviseSeg);

	 cut(hmm,"唐波的同学陈小慧不认识陈光的朋友徐前方", res);
	 for (int i = 0; i < res.size() ; i++)
	 	cout<<res[i]<<"/";
	 cout<<endl;
	  cut(hmm,"唐波动不了王钦龙虎视眈眈", res);
	 for (int i = 0; i < res.size() ; i++)
	 	cout<<res[i]<<"/";
	 cout<<endl;

	 cut(hmm,"保罗乔治今天遇到科比布莱恩特", res);
	 for (int i = 0; i < res.size() ; i++)
	 	cout<<res[i]<<"/";
	 cout<<endl;

	 // cut(hmm,"王星雅各布不认为齐爱芹菜鸟", res);
	 // for (int i = 0; i < res.size() ; i++)
	 // 	cout<<res[i]<<"/";
	 // cout<<endl;

	 // cut(hmm,"黄楚楚可怜", res);
	 // for (int i = 0; i < res.size() ; i++)
	 // 	cout<<res[i]<<"/";
	 // cout<<endl;
	 cut(hmm,"黄楚楚可怜王非", res);
	 for (int i = 0; i < res.size() ; i++)
	 	cout<<res[i]<<"/";
	 cout<<endl;

	 // cut(hmm,"一次性交齐", res);
	 // for (int i = 0; i < res.size() ; i++)
	 // 	cout<<res[i]<<"/";
	 // cout<<endl;

	 cut(hmm,"李纪超来到了网易杭研大厦", res);
	 for (int i = 0; i < res.size() ; i++)
	 	cout<<res[i]<<"/";
	 cout<<endl;
	 // cut(hmm,"他", res);
	 // for (int i = 0; i < res.size() ; i++)
	 // 	cout<<res[i]<<"/";
	 // cout<<endl;
	 // cut(hmm,"微博", res);
	 // for (int i = 0; i < res.size() ; i++)
	 // 	cout<<res[i]<<"/";
	 // cout<<endl;
	 // cut(hmm,"陈小慧", res);
	 // for (int i = 0; i < res.size() ; i++)
	 // 	cout<<res[i]<<"/";
	 // cout<<endl;
	 // cut(hmm,"王星雅俗共赏", res);
	 // for (int i = 0; i < res.size() ; i++)
	 // 	cout<<res[i]<<"/";
	 // cout<<endl;
	 //  cut(hmm,"王星雅俗气", res);
	 // for (int i = 0; i < res.size() ; i++)
	 // 	cout<<res[i]<<"/";
	 // cout<<endl;
	 cut(hmm,"陈光", res);
	 for (int i = 0; i < res.size() ; i++)
	 	cout<<res[i]<<"/";
	 cout<<endl;
	 cut(hmm,"陈光明天上飞机", res);
	 for (int i = 0; i < res.size() ; i++)
	 	cout<<res[i]<<"/";
	 cout<<endl;

	 // cut(hmm,"赵永江狂拽", res);
	 // for (int i = 0; i < res.size() ; i++)
	 // 	cout<<res[i]<<"/";
	 // cout<<endl;
	
	return 0;
}