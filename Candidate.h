#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "Vocabulary.h"

class Candidate : public Vocabulary
{
public:
	Candidate();
	Candidate(Vocabulary& voc);
	~Candidate();

	int position;
};

#endif

