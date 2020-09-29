#include <string>
#include <cstring>
#include <cmath>
#include "task1.h"

int comp1 (const void* a, const void* b) {
	return (*(int*)a) - (*(int*)b);
}

int comp2 (const void* a, const void* b) {
	if ( ((*(double*)a) - (*(double*)b)) < 0)
		return -1;
	else if ( ((*(double*)a) - (*(double*)b)) > 0 )
		return 1;
	return 0;
}

int comp3 (const void* a, const void* b) {
	return strcmp( *(const char**)a, *(const char**)b );
}

int comp4 (const void* a, const void* b) {

	return int(( std::string(*(const char**)a).length() - std::string(*(const char**)b).length()));
}

int comp5 (const void* a, const void* b) {
	std::string str1 = std::string(*(const char**)a);
	std::string str2 = std::string(*(const char**)b);
	int a1{0}, b1{0};


	for ( auto c : str1 ) {
		if ( c == ' ' )
			a1++;
	}

	for ( auto c : str2 ) {
		if ( c == ' ' )
			b1++;
	}

	return a1 - b1;
}

int comp6 (const void* a, const void* b) {
	Person par_a = (*(Person*)a);
	Person par_b = (*(Person*)b);

	return int(par_a.age - par_b.age);
}
