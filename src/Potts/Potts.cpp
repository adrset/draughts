#include "Potts.h"

namespace Potts {

    /*
    *   METODY MainMatrix
    *
    */

    MainMatrix::MainMatrix(int newMatrixSize, float simTemperature, float couplingFactor, int maxSpin, int minSpin ){
        matrixSize = newMatrixSize;
        temperature = simTemperature;
        coupling = couplingFactor;
        maxState = maxSpin;
        minState = minSpin;
        states = maxState-minState;

        for(int i=0;i<maxSpin;i++){
          colors.push_back(Color(255/maxSpin * (i+1), 255/maxSpin * (maxSpin -i), 255/maxSpin * ((maxSpin+2*i) % maxSpin)));
        }

		matrix =  new std::vector <char> (matrixSize*matrixSize);

        for(int x=0 ; x<matrixSize ; x++)
            for(int y=0 ; y<matrixSize ; y++)
                setSpin(x,y, getRandomSpin() );



	}
    MainMatrix::~MainMatrix(){
    	delete matrix;
    }
    float MainMatrix::random01(){
        return (float)(rand()%100) / 100;
    }
	char MainMatrix::getSpin(int x, int y){
		return (*matrix)[x*matrixSize+y];
	}
	void MainMatrix::setSpin(int x,int y, char newSpin){
		(*matrix)[x*matrixSize+y] = newSpin;
	}

	void MainMatrix::adjustTemperature(float dT){
		if (this->temperature + dT > 0)
			this->temperature += dT;

	}
	int MainMatrix::validRandom(){
		return 	rand()%matrixSize;
	}
    char MainMatrix::getRandomSpin(){
        return rand()%states+minState;
    }
	int MainMatrix::cronecker(int a, int b){
		if(a==b) return 1;
		else return 0;
	}
	float MainMatrix::calcHamilton(int x, int y, bool fromMatrix, char subSpin){
		float H=0;
		char currentSpin;
		if(fromMatrix)
			currentSpin = getSpin(x,y);
		else
			currentSpin = subSpin;

		H -= cronecker( currentSpin, getSpin(x+1,y) );
		H -= cronecker( currentSpin, getSpin(x-1,y) );
		H -= cronecker( currentSpin, getSpin(x,y+1) );
		H -= cronecker( currentSpin, getSpin(x,y-1) );

		H *= coupling;
		return H;
	}

	void MainMatrix::MetropolisStep(){
		int x=validRandom();	//random cell picking
		int y=validRandom();	//

		float currentHamilton = calcHamilton(x,y);			//calculating next state
		char newSpin = getRandomSpin();					    //newsSPin value is by default set to the random one, thus less code to overwrite the value (may be changed)
		float probableHamilton = calcHamilton(x,y,false,newSpin);

		if( currentHamilton > probableHamilton )            //if new state is better than current, overwrite the newSpin as current spin
            setSpin(x,y,newSpin);
        else{
            if( random01() < exp(-(probableHamilton-currentHamilton)/temperature) )                                     //if we have luck them set spin to newSpin
                setSpin(x,y,newSpin);
        }
        //if we do not have luck the spin stays the same
	}
	void MainMatrix::getSpinColor(int x, int y){

    }


    /*
    *   METODY InfoCluster
    *
    */

    InfoPack::InfoPack(MainMatrix * currMatrix){
        this->matrix = currMatrix;
        stateHistogram = new int[ this->matrix->getSpinsN() ];
    }

    void InfoPack::calcStateHistogram(bool doFurtherCalculations){

        int sizeOfMatrix = this->matrix->getMatrixSize();

        for(int a=0;a<this->matrix->getSpinsN();a++){
              stateHistogram[a]=0;
        }
        interfaces=0;
        for(int x=0;x< sizeOfMatrix;x++ ){
            for(int y=0;y< sizeOfMatrix;y++ ){
                stateHistogram[ (int)this->matrix->getSpin(x,y) ]++;

                if( ((x+1) != sizeOfMatrix))
                    if( matrix->getSpin(x,y) != matrix->getSpin(x+1,y) )
                        interfaces++;

                if( ((y+1) != sizeOfMatrix))
                    if( matrix->getSpin(x,y) != matrix->getSpin(x,y+1) )
                        interfaces++;

            }
        }
        orderFactor = 0;
        float N_over_Q = pow(matrix->getMatrixSize(),2) / matrix->getSpinsN();
        for(int a=0;a<this->matrix->getMatrixSize();a++){
              orderFactor += abs( (float)stateHistogram[a] - N_over_Q );
        }



    }

    void InfoPack::consolePrintData(){
        /*for(int i=0; i<this->matrix->getSpinsN();i++){
            printf("#%d=%-4d ",i,this->stateHistogram[i]);
        }
        printf("| INT=%f-4.0",interfaces);
        printf("MAG=%f-4.1f\n",orderFactor);*/

    }
}
