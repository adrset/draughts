#ifndef POTTS_H_
#define POTTS_H_

#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
namespace Potts {

typedef struct Color{
		char r;
		char g;
		char b;
		Color(int rc, int gc, int bc) : r(rc), g(gc), b(bc){};

	} Color;

class MainMatrix{

	//char ** matrix;	//pointer to the main square array of cells
	//char matrix[40][40];
	std::vector <char>* matrix;
	std::vector <Color> colors;
	int matrixSize;		//dimension of matrix

	float temperature;
	float coupling;
	int maxState;
	int minState;
	int states;

	void setSpin(int x,int y, char newSpin);
	int cronecker(int a, int b);
	int validRandom(); 	//returns random number from 0 to matrixSize;
	float random01();
	float calcHamilton(int x, int y, bool fromMatrix=true, char subSpin=0);	//calculates hamiltonian for point (x,y)
	char getRandomSpin(); //zwraca losową wartość spinu spomiędzy 0-masSpin

	public:

	Color getColor(int i){ return colors[i]; }
    MainMatrix(int newMatrixSize, float simTemperature, float couplingFactor, int maxSpin, int minSpin );
    ~MainMatrix();
	void MetropolisStep();	//does one step of calculation
	int getMatrixSize() const {return matrixSize;};
	char getSpin(int x, int y);
	void getSpinColor(int x, int y);
	void adjustTemperature(float dT);
	float getTemperature() const { return this-> temperature; }
	int getSpinsN() const { return states; };
};

class InfoPack{

    MainMatrix * matrix;

    float temperature;
    float interfaces;
    float orderFactor;
    int * stateHistogram;

    public:
   	float getInterfaces() const { return this->interfaces ;}
   	float getOrderFactor() const { return this->orderFactor ;}
    InfoPack(MainMatrix * currMatrix);
    void calcStateHistogram(bool doFurtherCalculations);
    void consolePrintData();

};


}
#endif
