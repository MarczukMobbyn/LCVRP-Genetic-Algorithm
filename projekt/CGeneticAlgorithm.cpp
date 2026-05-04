#include "CGeneticAlgorithm.h"
#include "CIndividual.h"
#include "CEvaluator.h"
#include <cstdlib>
#include <ctime>

// ============================================
// KONSTRUKTOR
// ============================================

CGeneticAlgorithm::CGeneticAlgorithm(int iPopSize, double dCrossProb, 
                                     double dMutProb, CEvaluator* pcEvaluator)
{
    i_pop_size = iPopSize;
    d_cross_prob = dCrossProb;
    d_mut_prob = dMutProb;
    pc_best_individual = NULL;
    i_current_iteration = 0;
    pc_evaluator = pcEvaluator;
    
    srand(static_cast<unsigned int>(time(NULL)));
}

// ============================================
// DESTRUKTOR
// ============================================

CGeneticAlgorithm::~CGeneticAlgorithm()
{
    vClearPopulation();
    
    if (pc_best_individual != NULL)
    {
        delete pc_best_individual;
        pc_best_individual = NULL;
    }
}

// ============================================
// GETTERY
// ============================================

CIndividual* CGeneticAlgorithm::pcGetBestIndividual()
{
    return pc_best_individual;
}

double CGeneticAlgorithm::dGetBestFitness()
{
    double dResult = WRONG_FITNESS_VALUE;
    
    if (pc_best_individual != NULL)
    {
        dResult = pc_best_individual->dGetFitness(*pc_evaluator);
    }
    
    return dResult;
}

int CGeneticAlgorithm::iGetCurrentIteration() const
{
    return i_current_iteration;
}

// ============================================
// FUNKCJA LOSOWA
// ============================================

int CGeneticAlgorithm::iRandomInt(int iMax)
{
    return rand() % (iMax + 1);
}

// ============================================
// ZARZADZANIE POPULACJA
// ============================================

void CGeneticAlgorithm::vClearPopulation()
{
    // Smart pointery same zwolnia pamiec gdy wektor sie wyczyści
    v_population.clear();
}

void CGeneticAlgorithm::vInitializePopulation()
{
    vClearPopulation();
    
    int iGenotypeSize = pc_evaluator->iGetSolutionSize();
    int iNumGroups = pc_evaluator->iGetNumGroups();
    
    for (int i = 0; i < i_pop_size; i++)
    {
        CIndividual* pcNewIndividual = new CIndividual(iGenotypeSize, iNumGroups);
        CMySmartPointer<CIndividual> cSmartPointer(pcNewIndividual);
        v_population.push_back(cSmartPointer);
    }
}

// ============================================
// EWALUACJA POPULACJI
// ============================================

// void CGeneticAlgorithm::vEvaluatePopulation()
// {
//     for (int i = 0; i < static_cast<int>(v_population.size()); i++)
//     {
//         // dGetFitness oblicza fitness jesli jeszcze nie obliczony
//         v_population[i]->dGetFitness(*pc_evaluator);
//     }
// }

// ============================================
// AKTUALIZACJA NAJLEPSZEGO
// ============================================

void CGeneticAlgorithm::vUpdateBestIndividual()
{
    for (int i = 0; i < static_cast<int>(v_population.size()); i++)
    {
        CIndividual* pcCurrent = &(*v_population[i]);
        
        if (pc_best_individual == NULL)
        {
            pc_best_individual = new CIndividual(*pcCurrent);
        }
        else if (pcCurrent->bIsBetterThan(*pc_best_individual, *pc_evaluator))
        {
            delete pc_best_individual;
            pc_best_individual = new CIndividual(*pcCurrent);
        }
    }
}

// ============================================
// SELEKCJA TURNIEJOWA
// ============================================

CIndividual* CGeneticAlgorithm::pcTournamentSelection()
{
    CIndividual* pcWinner = NULL;
    
    int iPopulationSize = static_cast<int>(v_population.size());
    
    int iIdx1 = iRandomInt(iPopulationSize - 1);
    int iIdx2 = iRandomInt(iPopulationSize - 1);
    
    if (iPopulationSize > 1)
    {
        while (iIdx2 == iIdx1)
        {
            iIdx2 = iRandomInt(iPopulationSize - 1);
        }
    }
    
    CIndividual* pc1 = &(*v_population[iIdx1]);
    CIndividual* pc2 = &(*v_population[iIdx2]);
    
    if (pc1->bIsBetterThan(*pc2, *pc_evaluator))
    {
        pcWinner = pc1;
    }
    else
    {
        pcWinner = pc2;
    }
    
    return pcWinner;
}

// ============================================
// TWORZENIE NOWEGO POKOLENIA
// ============================================

void CGeneticAlgorithm::vCreateNextGeneration()
{
    std::vector<CMySmartPointer<CIndividual> > vNewPopulation;
    
    while (static_cast<int>(vNewPopulation.size()) < i_pop_size)
    {
        CIndividual* pcParent1 = pcTournamentSelection();
        CIndividual* pcParent2 = pcTournamentSelection();
        
        CIndividual* pcChild1 = NULL;
        CIndividual* pcChild2 = NULL;
        pcParent1->bCrossover(*pcParent2, d_cross_prob, pcChild1, pcChild2);
        
        pcChild1->vMutate(d_mut_prob);
        pcChild2->vMutate(d_mut_prob);

        vApplyTurboMutation(pcChild1);
        vApplyTurboMutation(pcChild2);
        
        // Dodajemy pierwsze dziecko
        if (static_cast<int>(vNewPopulation.size()) < i_pop_size)
        {
            CMySmartPointer<CIndividual> cSmartPointer1(pcChild1);
            vNewPopulation.push_back(cSmartPointer1);
        }
        else
        {
            delete pcChild1;
        }
        
        // Dodajemy drugie dziecko
        if (static_cast<int>(vNewPopulation.size()) < i_pop_size)
        {
            CMySmartPointer<CIndividual> cSmartPointer2(pcChild2);
            vNewPopulation.push_back(cSmartPointer2);
        }
        else
        {
            delete pcChild2;
        }
    }
    
    vClearPopulation();
    v_population = vNewPopulation;
}

// ============================================
// GLOWNA PETLA ALGORYTMU
// ============================================

bool CGeneticAlgorithm::bRun(int iIterations)
{
    bool bSuccess = true;
    
    if (pc_evaluator == NULL || !pc_evaluator->bIsLoaded())
    {
        bSuccess = false;
    }
    
    if (bSuccess)
    {
        vInitializePopulation();
        
        for (int iIter = 0; iIter < iIterations; iIter++)
        {
            i_current_iteration = iIter + 1;
            
            //vEvaluatePopulation();
            vUpdateBestIndividual();
            vCreateNextGeneration();
        }
        
        //vEvaluatePopulation();
        vUpdateBestIndividual();
    }
    
    return bSuccess;
}

// ============================================
// TURBO MUTACJA - TEST
// ============================================

void CGeneticAlgorithm::vApplyTurboMutation(CIndividual* pcIndividual)
{
    if (pcIndividual == NULL || v_population.size() < 2)
    {
        return;
    }

    int iMaxIndividuals = static_cast<int>(v_population.size()) - 1;
    int iNumDonors = 1 + iRandomInt(iMaxIndividuals - 1);

    //wektor losowo wybranych osobników
    std::vector<CIndividual*> vIndividuals;

    for (int i = 0; i < iNumDonors; i++)
    {
        int iRandomIdx = iRandomInt(static_cast<int>(v_population.size()) - 1);
        vIndividuals.push_back(&(*v_population[iRandomIdx]));
    }

    // Wywołaj turbo mutację
    pcIndividual->vTurboMut(&vIndividuals);
}
