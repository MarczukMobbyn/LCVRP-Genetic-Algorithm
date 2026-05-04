#ifndef CGENETICALGORITHM_H
#define CGENETICALGORITHM_H

#include <vector>
#include "Constants.h"
#include "CMySmartPointer.h"
#include "CIndividual.h"

class CEvaluator;

class CGeneticAlgorithm 
{
public:
    CGeneticAlgorithm(int iPopSize, double dCrossProb, double dMutProb, 
                      CEvaluator* pcEvaluator);
    ~CGeneticAlgorithm();

    // Uruchamia algorytm - zwraca true jesli sukces
    bool bRun(int iIterations);

    CIndividual* pcGetBestIndividual();

    double dGetBestFitness();

    int iGetCurrentIteration() const;

    //pomocnicza do testu turbo mutacji
    void vApplyTurboMutation(CIndividual* pcIndividual);

private:
    int i_pop_size;
    double d_cross_prob;
    double d_mut_prob;
    
    std::vector<CMySmartPointer<CIndividual> > v_population;
    CIndividual* pc_best_individual;
    int i_current_iteration;
    
    CEvaluator* pc_evaluator;

    void vInitializePopulation();
    void vEvaluatePopulation();
    void vCreateNextGeneration();
    CIndividual* pcTournamentSelection();
    void vUpdateBestIndividual();
    void vClearPopulation();
    
    int iRandomInt(int iMax);

    // Zakaz kopiowania
    CGeneticAlgorithm(const CGeneticAlgorithm& cOther);
    CGeneticAlgorithm& operator=(const CGeneticAlgorithm& cOther);
};

#endif
