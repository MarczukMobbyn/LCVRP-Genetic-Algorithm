#include "CIndividual.h"
#include "CEvaluator.h"
#include <cstdlib>

// ============================================
// KONSTRUKTORY
// ============================================

CIndividual::CIndividual(int iGenotypeSize, int iNumGroups)
{
    pv_genotype = new std::vector<int>(iGenotypeSize);
    d_fitness = 0.0;
    b_evaluated = false;
    i_num_groups = iNumGroups;
    
    vRandomize();
}

CIndividual::CIndividual(const CIndividual& cOther)
{
    pv_genotype = new std::vector<int>(*(cOther.pv_genotype));
    d_fitness = cOther.d_fitness;
    b_evaluated = cOther.b_evaluated;
    i_num_groups = cOther.i_num_groups;
}

CIndividual::CIndividual(std::vector<int>* pvGenotype, int iNumGroups)
{
    pv_genotype = pvGenotype;
    d_fitness = 0.0;
    b_evaluated = false;
    i_num_groups = iNumGroups;
}

// ============================================
// DESTRUKTOR
// ============================================

CIndividual::~CIndividual()
{
    if (pv_genotype != NULL)
    {
        delete pv_genotype;
    }
}

// ============================================
// OPERATOR PRZYPISANIA
// ============================================

CIndividual& CIndividual::operator=(const CIndividual& cOther)
{
    if (this != &cOther)
    {
        if (pv_genotype != NULL)
        {
            delete pv_genotype;
        }
        
        pv_genotype = new std::vector<int>(*(cOther.pv_genotype));
        d_fitness = cOther.d_fitness;
        b_evaluated = cOther.b_evaluated;
        i_num_groups = cOther.i_num_groups;
    }
    
    return *this;
}

// ============================================
// GETTERY
// ============================================

std::vector<int>* CIndividual::pvGetGenotype() const
{
    return pv_genotype;
}

int CIndividual::iGetGenotypeSize() const
{
    return static_cast<int>(pv_genotype->size());
}

// ============================================
// FUNKCJE LOSOWE
// ============================================

int CIndividual::iRandomInt(int iMax)
{
    return rand() % (iMax + 1);
}

double CIndividual::dRandomDouble()
{
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

// ============================================
// GENEROWANIE LOSOWEGO GENOTYPU
// ============================================

void CIndividual::vRandomize()
{
    for (int i = 0; i < static_cast<int>(pv_genotype->size()); i++)
    {
        (*pv_genotype)[i] = iRandomInt(i_num_groups - 1);
    }
    
    b_evaluated = false;
}

// ============================================
// POBRANIE FITNESS
// ============================================

double CIndividual::dGetFitness(CEvaluator& cEvaluator)
{
    // Jesli fitness nie jest aktualny, oblicz go
    if (!b_evaluated)
    {
        double dResult = cEvaluator.dEvaluate(*pv_genotype);
        
        if (dResult < 0.0)
        {
            // Rozwiazanie niepoprawne
            d_fitness = 1e18;
        }
        else
        {
            d_fitness = dResult;
        }
        
        b_evaluated = true;
    }
    
    return d_fitness;
}

// ============================================
// MUTACJA
// ============================================

void CIndividual::vMutate(double dMutProb)
{
    for (int i = 0; i < static_cast<int>(pv_genotype->size()); i++)
    {
        double dRandom = dRandomDouble();
        
        if (dRandom < dMutProb)
        {
            (*pv_genotype)[i] = iRandomInt(i_num_groups - 1);
        }
    }
    
    b_evaluated = false;
}

// ============================================
// KRZYZOWANIE
// ============================================

bool CIndividual::bCrossover(const CIndividual& cOther, double dCrossProb,
                             CIndividual*& pcChild1, CIndividual*& pcChild2)
{
    bool bSuccess = true;
    double dRandom = dRandomDouble();
    int iGenotypeSize = static_cast<int>(pv_genotype->size());
    
    if (dRandom >= dCrossProb || iGenotypeSize <= 1)
    {
        // Krzyzowanie nie zachodzi, zwracamy kopie rodzicow
        pcChild1 = new CIndividual(*this);
        pcChild2 = new CIndividual(cOther);
    }
    else
    {
        int iCrossPoint = 1 + iRandomInt(iGenotypeSize - 2);
        
        std::vector<int>* pvChild1Genotype = new std::vector<int>(iGenotypeSize);
        std::vector<int>* pvChild2Genotype = new std::vector<int>(iGenotypeSize);
        
        // Czesc przed punktem krzyzowania
        for (int i = 0; i < iCrossPoint; i++)
        {
            (*pvChild1Genotype)[i] = (*pv_genotype)[i];
            (*pvChild2Genotype)[i] = (*(cOther.pv_genotype))[i];
        }
        
        // Czesc po punkcie krzyzowania
        for (int i = iCrossPoint; i < iGenotypeSize; i = i + 1)
        {
            (*pvChild1Genotype)[i] = (*(cOther.pv_genotype))[i];
            (*pvChild2Genotype)[i] = (*pv_genotype)[i];
        }
        
        pcChild1 = new CIndividual(pvChild1Genotype, i_num_groups);
        pcChild2 = new CIndividual(pvChild2Genotype, i_num_groups);
    }
    
    return bSuccess;
}

// ============================================
// POROWNYWANIE
// ============================================

bool CIndividual::bIsBetterThan(CIndividual& cOther, CEvaluator& cEvaluator)
{
    // Pobierz fitness obu osobnikow (oblicza jesli potrzeba)
    double dMyFitness = dGetFitness(cEvaluator);
    double dOtherFitness = cOther.dGetFitness(cEvaluator);
    
    // mniejszy fitness = lepszy
    return dMyFitness < dOtherFitness;
}

//=============================================
// TURBO MUTACJA (modyfikacja)
//=============================================

void CIndividual::vTurboMut(std::vector<CIndividual*>* pvOther)
{
    if (pvOther == NULL || pvOther->size() == 0)
    {
        return;
    }

    int iOtherCount = static_cast<int>(pvOther->size());

    for (int i = 0; i < static_cast<int>(pv_genotype->size()); i++)
    {
        int iRandomIndex = iRandomInt(iOtherCount - 1);

        CIndividual* pcOther = (*pvOther)[iRandomIndex];
        std::vector<int>* pvOtherGenotype = pcOther->pvGetGenotype();

        (*pv_genotype)[i] = (*pvOtherGenotype)[i];
    }

    b_evaluated = false;
}
