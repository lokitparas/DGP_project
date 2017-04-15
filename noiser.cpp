#include <iostream>
#include <math.h>
#include <vector>
#include <random>

using namespace std;

//override the standard input and output streams with the necessary files
//command to compile: g++ -std=c++11 noiser.cpp -o noiser
//command to run: ./noiser < "original point cloud" > "noised point cloud"
//example: ./noiser < data/cube.pts >data/noised_cube.pts
//change the sigma value as needed
//change the boolean normal to true if you want to add noise to the normals as well
int main(){
	float sigma = 0.01;
	float a,b,c,d,e,f;
	bool normal = false;
	default_random_engine generator;
	normal_distribution<double> distribution(0.0,sigma);
	while(cin>>a){
		cin>>b>>c>>d>>e>>f;
		if(normal) cout<<a+distribution(generator)<<" "<<b+distribution(generator)<<" "<<c+distribution(generator)<<" "<<d+distribution(generator)<<" "<<e+distribution(generator)<<" "<<f+distribution(generator)<<endl;
		else cout<<a+distribution(generator)<<" "<<b+distribution(generator)<<" "<<c+distribution(generator)<<" "<<d<<" "<<e<<" "<<f<<endl;
	}

}