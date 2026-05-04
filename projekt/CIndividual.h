#ifndef CINDIVIDUAL_H
#define CINDIVIDUAL_H

#include <vector>
#include "Constants.h"

class CEvaluator;

class CIndividual 
{
public:
    CIndividual(int iGenotypeSize, int iNumGroups);
    CIndividual(const CIndividual& cOther);
    CIndividual(std::vector<int>* pvGenotype, int iNumGroups);
    ~CIndividual();

    CIndividual& operator=(const CIndividual& cOther);

    // Zwraca fitness - jesli nie obliczony, oblicza go
    double dGetFitness(CEvaluator& cEvaluator);
    
    // Mutacja osobnika
    void vMutate(double dMutProb);
    
    // Krzyzowanie - zwraca true jesli sukces
    bool bCrossover(const CIndividual& cOther, double dCrossProb,
                    CIndividual*& pcChild1, CIndividual*& pcChild2);

    // Gettery
    std::vector<int>* pvGetGenotype() const;
    int iGetGenotypeSize() const;
    
    // Porownanie - wymaga ewaluatora bo moze potrzebowac obliczyc fitness
    bool bIsBetterThan(CIndividual& cOther, CEvaluator& cEvaluator);

    //modyfikacja - turbo mutacja
    void vTurboMut(std::vector<CIndividual*>* pvOther);

private:
    std::vector<int>* pv_genotype;
    double d_fitness;
    bool b_evaluated;
    int i_num_groups;

    void vRandomize();
    int iRandomInt(int iMax);
    double dRandomDouble();
};

#endif
