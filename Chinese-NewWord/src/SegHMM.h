#include "Type.h" 
namespace Seg
{
	bool loadModel(const char* const filePath , HMMSeg & hmm);
	bool loadEmitProb(const string& line, EmitProbMap& mp);
	bool decodeOne(const string& str, uint16_t& res);
	double getEmitProb(const EmitProbMap* ptMp, uint16_t key, double defVal);
	bool viterbi(HMMSeg &hmm, const vector<uint16_t>& unico, vector<unsigned int>& status);
	bool cut(bool is_utf8, HMMSeg & hmm, const string& str, vector<string>& res);
	bool cut(bool is_utf8, HMMSeg & hmm, vector<uint16_t> & src_unico, const unsigned int& start, const unsigned int& end, vector<string>& res);
}