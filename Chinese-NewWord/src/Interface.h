#include "Type.h" 
namespace Seg
{
	string segregate(bool is_utf8, HMMSeg & hmm,ReSeg & reseg);
	bool reivse(bool is_utf8, HMMSeg & hmm, vector<string>& res);
	bool revise(bool is_utf8, HMMSeg & hmm, ReSeg & reseg);
}