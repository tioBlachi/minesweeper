//
// Created by Blas Antunez on 8/9/24.
//

#ifndef RANDOM_H
#define RANDOM_H



#include <random>

class Random
{
	// One instance of the x variable, and all instances SHARE this variable
	// static variables/functions exist for the lifetime of your project
	static std::mt19937 random;
public:
	static int x;
	// One and only one of this function
	static int Int(int min, int max);
	static float Float(float min, float max);
};



#endif //RANDOM_H
