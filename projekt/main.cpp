#include <iostream>
#include <string>

#include "Constants.h"
#include "CEvaluator.h"
#include "CIndividual.h"
#include "CGeneticAlgorithm.h"
#include "CMySmartPointer.h"

void vPrintSolution(const std::vector<int>& vSolution)
{
    std::cout << "Genotyp: [";
    
    for (int i = 0; i < static_cast<int>(vSolution.size()); i = i + 1)
    {
        std::cout << vSolution[i];
        
        if (i < static_cast<int>(vSolution.size()) - 1)
        {
            std::cout << ", ";
        }
    }
    
    std::cout << "]" << std::endl;
}

void vDemonstrateSmartPointer()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  Demonstracja CMySmartPointer" << std::endl;
    std::cout << "========================================" << std::endl;
    
    CMySmartPointer<int> cSp1(new int(42));
    std::cout << "sp1 utworzony, wartosc: " << *cSp1 << ", licznik: " << cSp1.iGetCount() << std::endl;
    
    {
        CMySmartPointer<int> cSp2(cSp1);
        std::cout << "sp2 skopiowany, wartosc: " << *cSp2 << ", licznik: " << cSp2.iGetCount() << std::endl;
        
        CMySmartPointer<int> cSp3(cSp1);
        std::cout << "sp3 skopiowany, wartosc: " << *cSp3 << ", licznik: " << cSp3.iGetCount() << std::endl;
    }
    
    std::cout << "sp2 i sp3 zniszczone, sp1 licznik: " << cSp1.iGetCount() << std::endl;
    std::cout << std::endl;
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  Algorytm Genetyczny dla LcVRP" << std::endl;
    std::cout << "  TEP - Mini Projekt" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::string sFilePath = "A-n32-k5.lcvrp";
    int iNumGroups = DEFAULT_NUM_GROUPS;
    int iPopSize = DEFAULT_POP_SIZE;
    double dCrossProb = DEFAULT_CROSS_PROB;
    double dMutProb = DEFAULT_MUT_PROB;
    int iIterations = DEFAULT_ITERATIONS;


    std::cout << "Plik instancji: " << sFilePath << std::endl;
    std::cout << "Liczba grup: " << iNumGroups << std::endl;
    std::cout << std::endl;

    // ========================================
    // 1. Wczytanie instancji problemu
    // ========================================
    std::cout << "Wczytywanie instancji..." << std::endl;
    
    CEvaluator cEvaluator;
    
    if (!cEvaluator.bLoadInstance(sFilePath, iNumGroups))
    {
        std::cout << "BLAD: " << cEvaluator.sGetLastError() << std::endl;
        return 1;
    }
    
    std::cout << "Nazwa instancji: " << cEvaluator.sGetName() << std::endl;
    std::cout << "Liczba klientow: " << cEvaluator.iGetNumCustomers() << std::endl;
    std::cout << "Pojemnosc pojazdu: " << cEvaluator.iGetCapacity() << std::endl;
    std::cout << std::endl;

    // ========================================
    // 2. Konfiguracja i uruchomienie AG
    // ========================================
    std::cout << "Parametry AG:" << std::endl;
    std::cout << "  PopSize = " << iPopSize << std::endl;
    std::cout << "  CrossProb = " << dCrossProb << std::endl;
    std::cout << "  MutProb = " << dMutProb << std::endl;
    std::cout << "  Iteracje = " << iIterations << std::endl;
    std::cout << std::endl;

    std::cout << "Uruchamianie algorytmu genetycznego..." << std::endl;
    std::cout << std::endl;

    CGeneticAlgorithm cGA(iPopSize, dCrossProb, dMutProb, &cEvaluator);
    
    if (!cGA.bRun(iIterations))
    {
        std::cout << "BLAD: Nie udalo sie uruchomic algorytmu" << std::endl;
        return 1;
    }

    // ========================================
    // 3. Wyswietlenie wynikow
    // ========================================
    std::cout << "========================================" << std::endl;
    std::cout << "  WYNIKI" << std::endl;
    std::cout << "========================================" << std::endl;
    
    CIndividual* pcBest = cGA.pcGetBestIndividual();
    
    if (pcBest != NULL)
    {
        std::cout << "Najlepszy fitness (koszt): " << cGA.dGetBestFitness() << std::endl;
        vPrintSolution(*(pcBest->pvGetGenotype()));
    }
    else
    {
        std::cout << "Nie znaleziono rozwiazania!" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "Liczba wykonanych iteracji: " << cGA.iGetCurrentIteration() << std::endl;
    std::cout << std::endl;

    // ========================================
    // 4. Demonstracja smart pointera
    // ========================================
    vDemonstrateSmartPointer();

    // ========================================
    // 5. Test turbo mutacji
    // ========================================

    std::cout << "=== TEST TURBO MUTACJI ===" << std::endl;

    CIndividual cInd1(5, 3);
    CIndividual cInd2(5, 3);
    CIndividual cInd3(5, 3);
    CIndividual cInd4(5,3);

    std::cout << "Ind1 przed: ";
    for (int i = 0; i < 5; i++) std::cout << (*cInd1.pvGetGenotype())[i] << " ";
    std::cout << std::endl;

    std::cout << "Ind2: ";
    for (int i = 0; i < 5; i++) std::cout << (*cInd2.pvGetGenotype())[i] << " ";
    std::cout << std::endl;

    std::cout << "Ind3: ";
    for (int i = 0; i < 5; i++) std::cout << (*cInd3.pvGetGenotype())[i] << " ";
    std::cout << std::endl;

    std::cout << "Ind4: ";
    for (int i = 0; i < 5; i++) std::cout << (*cInd4.pvGetGenotype())[i] << " ";
    std::cout << std::endl;

    std::vector<CIndividual*> vOthers;
    vOthers.push_back(&cInd2);
    vOthers.push_back(&cInd3);
    vOthers.push_back(&cInd4);

    cInd1.vTurboMut(&vOthers);

    std::cout << "Ind1 po turbo: ";
    for (int i = 0; i < 5; i++) std::cout << (*cInd1.pvGetGenotype())[i] << " ";
    std::cout << std::endl;


    std::cout << "Program zakonczony pomyslnie." << std::endl;
    
    return 0;
}
