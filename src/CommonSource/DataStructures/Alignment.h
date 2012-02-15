// ***************************************************************************
// Alignment.h - stores everything related to alignments.
// ---------------------------------------------------------------------------
// (c) 2006 - 2009 Michael Str�mberg
// Marth Lab, Department of Biology, Boston College
// ---------------------------------------------------------------------------
// Dual licenced under the GNU General Public License 2.0+ license or as
// a commercial license with the Marth Lab.
// ***************************************************************************

#ifndef _ALIGNMENT_H_
#define _ALIGNMENT_H_

#include <cmath>
#include <string>

#include "Mosaik.h"
#include "MosaikString.h"
#include "SequencingTechnologies.h"
#include "StrandChecker.h"

using namespace std;

// our alignment structure [all members identified as (temp) are not serialized to disk]
struct Alignment {

	unsigned int MateReferenceBegin;   // required for SAM/BAM
	unsigned int MateReferenceEnd;     // required for SAM/BAM
	unsigned int MateReferenceIndex;   // required for SAM/BAM
	unsigned int ReferenceBegin;
	unsigned int ReferenceEnd;
	unsigned int ReferenceIndex;
	unsigned int Owner;                // the temporary file that contains the alignment
	unsigned int ReadGroupCode;        // the read group code (temp)
	unsigned int NumMapped;            // the total number of mapped alignments
	double NumHash;
	int FragmentLength;                // the fragment length with its pair
	unsigned short QueryLength;        // used during filtering (temp)
	unsigned short NumMismatches;      // number of mismatches
	unsigned short QueryBegin;
	unsigned short QueryEnd;
	unsigned short MappedLength;       // the length of mapped bases
	unsigned short NumCigarOperation;
	unsigned short NumLongestMatchs;
	unsigned char Quality;             // alignment quality
	unsigned char NextBestQuality;     // the next best alignment quality
	unsigned char RecalibratedQuality; // recalibrated quality
	float Entropy;
	float SwScore;
	float NextSwScore;
	bool CanBeMappedToSpecialReference;// can the sequence be mapped to special references?`
	bool IsFilteredOut;                // is mapped to somewhere but cannot pass the filters
	bool IsFirstMate;                  // is this alignment from the first mate in a paired-end read
	bool IsJunk;                       // are the fileds in this alignment used for other propose, e.g. counting total numbers of alignments?
	bool IsMateReverseStrand;          // read orientation for the mate
	bool IsPairedEnd;                  // is the read sequenced as a paired-end read
	bool IsResolvedAsPair;             // is the alignment part of resolved paired-end read
	bool IsResolvedAsProperPair;       // is the alignment resolved as proper pair
	bool IsReverseStrand;              // read orientation
	bool IsMappedSpecialReference;     // is this alignment mapped to the special references which is defined by "-sref"? 
	bool IsMapped;                     // is this alignment mapped? If true, it also means IsJunk.
	bool IsMateMapped;                 // is its mate mapped?
	bool WasRescued;                   // was the alignment rescued during local alignment search
	char* ReferenceName;               // only filled via CAlignmentReader (temp)
	CMosaikString Reference;
	CMosaikString Query;
	CMosaikString BaseQualities;
	CMosaikString Name;                // the read name
	string CsQuery;                    // colorspace read sequence
	string CsBaseQualities;            // colorspace base qualities
	string Cigar;                      // the cigar string
	string ReadGroup;                  // the read group string
	string SpecialCode;                // 2 letters to indicate the sequence can be mapped to which special reference
	string MdString;
	string EncodedQuery;
	string PackedCigar;

	bool Mark;

	// constructors
	Alignment(void)
		: MateReferenceBegin(0)
		, MateReferenceEnd(0)
		, MateReferenceIndex(0)
		, ReferenceBegin(0)
		, ReferenceEnd(0)
		, ReferenceIndex(0)
		, Owner(0)
		, ReadGroupCode(0)
		, NumMapped(1)
		, NumHash(0.0)
		, FragmentLength(0)
		, QueryLength(0)
		, NumMismatches(0)
		, QueryBegin(0)
		, QueryEnd(0)
		, MappedLength(0)
		, NumCigarOperation(0)
		, NumLongestMatchs(0)
		, Quality(0)
		, NextBestQuality(0)
		, RecalibratedQuality(0)
		, Entropy(0)
		, SwScore(0)
		, NextSwScore(0)
		, CanBeMappedToSpecialReference(false)
		, IsFilteredOut(false)
		, IsFirstMate(false)
		, IsJunk(false)
		, IsMateReverseStrand(false)
		, IsPairedEnd(false)
		, IsResolvedAsPair(false)
		, IsResolvedAsProperPair(false)
		, IsReverseStrand(false)
		, IsMappedSpecialReference(false)
		, IsMapped(true)
		, IsMateMapped(false)
		, WasRescued(false)
		, ReferenceName(NULL)
		, Reference()
		, Query()
		, BaseQualities()
		, Name()
		, CsQuery()
		, CsBaseQualities()
		, Cigar()
		, ReadGroup()
		, SpecialCode()
		, MdString()
		, EncodedQuery()
		, PackedCigar()
		, Mark(false)
	{}

	bool operator==( const Alignment& al) const {
		if ( ReferenceBegin  != al.ReferenceBegin )  return false;
		if ( ReferenceEnd    != al.ReferenceEnd )    return false;
		if ( ReferenceIndex  != al.ReferenceIndex )  return false;
		if ( ReadGroupCode   != al.ReadGroupCode )   return false;
		if ( QueryBegin      != al.QueryBegin )      return false;
		if ( QueryEnd        != al.QueryEnd )        return false;
		if ( Quality         != al.Quality )         return false;
		if ( IsFirstMate     != al.IsFirstMate )     return false;
		if ( IsReverseStrand != al.IsReverseStrand ) return false;

		return true;
	}

	// our less-than operator
	bool operator<(const Alignment& al) const {
		if(ReferenceIndex == al.ReferenceIndex) return ReferenceBegin < al.ReferenceBegin;
		return ReferenceIndex < al.ReferenceIndex;
	}

	
	// return true when they are a proper pair; otherwise return false
	// Note: please check IsFirstMate and IsReverseStrand are set before applying this function
	bool SetPairFlagsAndFragmentLength ( const Alignment& pairMate, const int& minFragmentLength, const int& maxFragmentLength, const SequencingTechnologies& tech ) {
		bool isLeft = (ReferenceBegin <= pairMate.ReferenceBegin) ? true : false;
		unsigned int leftmost  = isLeft ? ReferenceBegin : pairMate.ReferenceBegin;
		unsigned int rightmost = isLeft ? pairMate.ReferenceEnd : ReferenceEnd;
		if (ReferenceIndex == pairMate.ReferenceIndex) {
			if (isLeft) FragmentLength = rightmost - leftmost;
			else        FragmentLength = leftmost - rightmost;
		} else {
			FragmentLength = 0;
		}
		
		if ( !StrandChecker::isProperOrientation( IsReverseStrand, pairMate.IsReverseStrand, ReferenceBegin, pairMate.ReferenceBegin, IsFirstMate, tech ) )
			IsResolvedAsProperPair = false;
		else if ( ReferenceIndex != pairMate.ReferenceIndex )
			IsResolvedAsProperPair = false;
		else {
			int absFl = abs( FragmentLength );
			if ( ( minFragmentLength < absFl ) && ( absFl < maxFragmentLength ) )
				IsResolvedAsProperPair = true;
			else
				IsResolvedAsProperPair = false;
		}

		return IsResolvedAsProperPair;
	}

	// recalibrate mapping qaulity for paired-end alignments
	bool RecalibrateQuality ( const bool isUU, const bool isMM ) {
		
		int aq = Quality;
		if ( isUU )      aq = (int) ( UU_COEFFICIENT * aq + UU_INTERCEPT );
		else if ( isMM ) aq = 0;
		//else if ( isMM ) aq = (int) ( MM_COEFFICIENT * aq + MM_INTERCEPT );
		else             aq = (int) ( UM_COEFFICIENT * aq + UM_INTERCEPT );

		if(aq < 0)       aq = 0;
		else if(aq > 99) aq = 99;

		RecalibratedQuality = aq;

		return true;
	}
};

struct Alignment_LessThanMq {
	bool operator() (const Alignment& al1, const Alignment& al2) {
		if (al1.SwScore == al2.SwScore) {
			if (al1.Quality == al2.Quality) {
				if (al1.NumLongestMatchs == al2.NumLongestMatchs)
					return al1.ReferenceIndex > al2.ReferenceIndex;
				else
					return al1.NumLongestMatchs < al2.NumLongestMatchs;
			} else { 
				return al1.Quality < al2.Quality;
			}
		} else { 
			return al1.SwScore < al2.SwScore;
		}
	}

	bool operator() (const Alignment* al1, const Alignment* al2) {
		if (al1->SwScore == al2->SwScore) {
			if (al1->Quality == al2->Quality) {
				if (al1->NumLongestMatchs == al2->NumLongestMatchs)	
					return al1->ReferenceIndex > al2->ReferenceIndex;
				else
					return al1->NumLongestMatchs < al2->NumLongestMatchs;
			} else {
				return al1->Quality < al2->Quality;
			}
		} else {
			return al1->SwScore < al2->SwScore;
		}
	}
};

struct Alignment_LessThanPosition {
  bool operator() (const Alignment* al1, const Alignment* al2) {
    if(al1->ReferenceIndex == al2->ReferenceIndex) return al1->ReferenceBegin < al2->ReferenceBegin;
    return al1->ReferenceIndex < al2->ReferenceIndex;
  }
};


#endif
