#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

bool check ( istringstream& record1, istringstream& record2, vector <unsigned int>& mqs, vector <unsigned int>& maps, const bool unmapped, unsigned int& correctPos, string& chr) {
	string chr1, chr2;
	unsigned int pos1, pos2;
	unsigned short mq1, mq2;

	record1 >> chr1;
	record2 >> chr2;
	record1 >> pos1;
	record2 >> pos2;
	record1 >> mq1;
	record2 >> mq2;

	if (unmapped) {
		return true;
	}
	
	maps[ mq2 ]++;
	correctPos = pos1;
	chr = chr1;


	if (( chr1 == chr2 ) && ( ( ( pos1 - 10 ) < pos2 ) && ( pos2 < ( pos1 + 10 ) ) ) )
	     //(( chr1 == chr2 ) && ( ( ( pos1 - 10 ) < (pos2 + 99) ) && ( (pos2 + 99) < ( pos1 + 10 ) ) ) ) )
		//if ((rand() % 500) == 0) return true;
		//else return false;
		;
	else {
		mqs[ mq2 ]++;
		//if ( mq2 == 43 ) cout << pos2 << endl;
	}
	
	//if ((rand() % 5) == 0) return true;
	//else return true;
	return true;

}

int main( int argc, char* argv[] ) {

	if (argc != 3) {
	  cerr << "USAGE: " << argv[0] << " <GOLD_SAM> <TEST_SAM> > <OUT_SAM>" << endl;
	  cerr << "       SAMs should be sorted by read names and do not have headers." << endl;
	  cerr << endl << argv[0] << " is a program to attach positions of GOLD_SAM in TEST_SAM as XC tags." << endl << endl;
	  return 1;
	}
	
	ifstream sam1, sam2;
	sam1.open( argv[1] );
	sam2.open( argv[2] );

	vector <unsigned int> mqs( 255, 0 );
	vector <unsigned int> maps( 255, 0 );
	//string record1Mate1, record1Mate2, record2Mate1, record2Mate2;
	string record1Mate1, record2Mate1;

	//unsigned int no1 = 0, no2 = 0;
	if ( !sam1.good() || !sam2.good() ) {
		cout << "ERROR: Cannot open input files." << endl;
		return 1;
	}
	
	getline ( sam1, record1Mate1 );
	//getline ( sam1, record1Mate2 );
	getline ( sam2, record2Mate1 );
	//getline ( sam2, record2Mate2 );

	//unsigned int unmappedCount = 0;
	unsigned int count = 0;
	while ( sam1.good() && sam2.good() ) {
		//istringstream buffer11(record1Mate1), buffer12(record1Mate2), buffer21(record2Mate1), buffer22(record2Mate2);
		istringstream buffer11(record1Mate1), buffer21(record2Mate1);
		string junk;

		string name1, name2;
		buffer11 >> name1;
		buffer21 >> name2;
		//buffer12 >> junk;
		//buffer22 >> junk;

		//unsigned short flag1, flag2, flag3;
		unsigned short flag1, flag2;
		buffer11 >> flag1;
		buffer21 >> flag2;
		//buffer12 >> junk;
		//buffer22 >> flag3;

		if ( name1 == name2 ) {
		count++;
			//===for wgsim===
			//istringstream buffer_temp1(buffer21.str());
			//istringstream buffer_temp2(buffer22.str());
			//string pos1, pos2;
			//buffer_temp1 >> pos1;
			//buffer_temp1 >> pos1;
			//buffer_temp1 >> pos1;
			//buffer_temp1 >> pos1;
			//buffer_temp2 >> pos2;
			//buffer_temp2 >> pos2;
			//buffer_temp2 >> pos2;
			//buffer_temp2 >> pos2;

			//int pos1_i = atoi(pos1.c_str());
			//int pos2_i = atoi(pos2.c_str());

			//bool flag1Mate1 = true;
			//bool flag2Mate1 = (pos2_i < pos1_i) ? false : true;
			//===end of wgsim===

			//===for PE===
			//bool flag1Mate1 = flag1 & 0x0040;
			//bool flag2Mate1 = flag2 & 0x0040;

			//bool unmappedMate1 = flag2 & 0x0004;
			//bool unmappedMate2 = flag3 & 0x0004;
			//===end of PE===

			unsigned int correctPos = 0;
			string chr;

			//===for SE===
			bool unmappedMate1 = flag2 & 0x0004;
			if (check( buffer11, buffer21, mqs, maps, unmappedMate1, correctPos, chr ))
				cout << buffer21.str() << " XC:Z:" << chr << ";" << correctPos << endl;
			//===end of SE===

			//===for PE===
			/*
			if ( flag1Mate1 && flag2Mate1 ) {
				if ( check( buffer11, buffer21, mqs, maps, unmappedMate1, correctPos ) )
					cout << buffer21.str() << " XC:Z:" << correctPos << endl;
				if ( check( buffer12, buffer22, mqs, maps, unmappedMate2, correctPos ) )
					cout << buffer22.str() << " XC:Z:" << correctPos << endl;
			} else if ( !flag1Mate1 && flag2Mate1 ) {
				if ( check( buffer12, buffer21, mqs, maps, unmappedMate1, correctPos ) )
					cout << buffer21.str() << " XC:Z:" << correctPos << endl;
				if ( check( buffer11, buffer22, mqs, maps, unmappedMate2, correctPos ) )
					cout << buffer22.str() << " XC:Z:" << correctPos << endl;
			} else if ( flag1Mate1 && !flag2Mate1 ) {
				if ( check( buffer11, buffer22, mqs, maps, unmappedMate2, correctPos ) )
					cout << buffer22.str() << " XC:Z:" << correctPos << endl;
				if ( check( buffer12, buffer21, mqs, maps, unmappedMate1, correctPos ) )
					cout << buffer21.str() << " XC:Z:" << correctPos << endl;
			} else {
				if ( check( buffer12, buffer22, mqs, maps, unmappedMate2, correctPos ) )
					cout << buffer22.str() << " XC:Z:" << correctPos << endl;
				if ( check( buffer11, buffer21, mqs, maps, unmappedMate1, correctPos ) )
					cout << buffer21.str() << " XC:Z:" << correctPos << endl;
			}
			*/
			//===end of PE===

			getline ( sam1, record1Mate1 );
			//getline ( sam1, record1Mate2 );
			getline ( sam2, record2Mate1 );
			//getline ( sam2, record2Mate2 );
			//no1 += 2;
			//no2 += 2;
		}
		else {
			if ( name1 < name2 ) {
				getline ( sam1, record1Mate1 );
				//getline ( sam1, record1Mate2 );
				//no1 += 2;
			}
			else {
				getline ( sam2, record2Mate1 );
				//getline ( sam2, record2Mate2 );
				//no2 += 2;
			}
		}
	}

	
	sam1.close();
	sam2.close();

/*
	unsigned int totalMismatch = 0;
	unsigned int totalMaps     = 0;
	for ( int i = mqs.size() - 1; i >= 0; --i ) {
		totalMismatch += mqs[ i ];
		totalMaps     += maps[ i ];
		//if ( maps[ i ] != 0 ) 
		//	cout << i << "\t" << totalMaps << "\t" << totalMismatch << endl;
		if ( maps[ i ] != 0 ) {
		  cout << i << "\t" 
		       << -10 * (float) log10(mqs[ i ]/(float)maps[ i ]) << "\t" 
		       << maps[ i ] << "\t" 
		       << maps[ i ] - mqs[ i ] << "\t" 
		       << mqs[ i ] << endl;
		}

	}
*/

	return 0;
}
