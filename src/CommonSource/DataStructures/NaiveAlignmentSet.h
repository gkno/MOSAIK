// ***************************************************************************
// CNaiveAlignmentSet - essentially a very naive mechanism for ensuring that
//                      duplicates alignments are not reported.
// ---------------------------------------------------------------------------
// (c) 2006 - 2009 Michael Str�mberg
// Marth Lab, Department of Biology, Boston College
// ---------------------------------------------------------------------------
// Dual licenced under the GNU General Public License 2.0+ license or as
// a commercial license with the Marth Lab.
// ***************************************************************************

#ifndef NAIVEALIGNMENTSET_H_
#define NAIVEALIGNMENTSET_H_

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include "Alignment.h"

using namespace std;

struct MhpOccupancyPosition {
	unsigned short Begin;
	unsigned short End;
	//double Occupancy;

	// our less-than operator
	//bool operator<(const MhpOccupancyPosition& al) const {
	//	return Occupancy > al.Occupancy;
	//}
};

struct MhpOccupancyRegion {
	unsigned short Begin;
	unsigned short End;
	double BeginOccupancy;
	double EndOccupancy;

	// constructor
	MhpOccupancyRegion(const unsigned short begin, const unsigned short end, const double occupancy)
		: Begin(begin)
		, End(end)
		, BeginOccupancy(occupancy)
		, EndOccupancy(occupancy)
	{}
};

typedef vector<Alignment> AlignmentSet;
typedef list<MhpOccupancyPosition> MhpOccupancyList;

class CNaiveAlignmentSet {
public:
	// constructor
	CNaiveAlignmentSet(unsigned int refLen, bool usingIllumina);
	// destructor
	//~CNaiveAlignmentSet(void);
	// check if there are any alignments sitting in the given region
	bool CheckExistence ( const unsigned int& refIndex, const unsigned int& begin, const unsigned int& end );
	// adds an alignment to the set
	bool Add(Alignment& al);
	// calculates the alignment qualities for each alignment in the set
	void CalculateAlignmentQualities(const bool calculateCorrectionCoefficient, const unsigned short minSpanLength);
	// resets the counter and stored alignments
	void Clear(void);
	// dumps the contents of the alignment set to standard output
	void Dump(void) const;
	// retrieves the mhp occupancy list for the forward read
	MhpOccupancyList* GetFwdMhpOccupancyList(void);
	// retrieves the mhp occupancy list for the reverse read
	MhpOccupancyList* GetRevMhpOccupancyList(void);
	// retrieves the alignment set
	AlignmentSet* GetSet(void);
	void GetSet(vector<Alignment*>* alSet);
	// returns the number of alignments in the set
	inline unsigned int GetCount(void) const {return (unsigned int)mAlignments.size(); };
	// retrieves the long alignment flag
	inline bool HasLongAlignment(void) const { return mHasLongAlignment; };
	// returns true if the alignment set is empty
	inline bool IsEmpty(void) const { return mAlignments.empty(); };
	// returns true if the alignment set contains only one entry
	inline bool IsUnique(void) const { return (mAlignments.size() == 1 ? true : false); };
	// returns true if the alignment set contains more than one entry
	inline bool IsMultiple (void) const { return (mAlignments.size() > 1 ? true : false); };
	inline float GetHighestSwScore(void) const { return highestSmithWatermanScore; };

private:
	// calculates the correction coefficient
	//static double CalculateCorrectionCoefficient(const unsigned short queryBegin, const unsigned short queryEnd, const MhpOccupancyList& mhpOccupancyList, const unsigned short minSpanLength);
	// checks if the current alignment is a subset of another alignment
	static bool CheckOverlap(const Alignment& al1, AlignmentSet::iterator& al2);
	// stores our vector
	AlignmentSet mAlignments;
	// stores our mhp occupancy lists
	MhpOccupancyList mFwdMhpOccupancyList;
	MhpOccupancyList mRevMhpOccupancyList;
	// sets the flag when we have long alignments
	bool mHasLongAlignment;
	float highestSmithWatermanScore;
};

#endif // NAIVEALIGNMENTSET_H_
