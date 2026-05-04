#ifndef CEVALUATOR_H
#define CEVALUATOR_H

#include <string>
#include <vector>
#include "Constants.h"

struct SCoordinate 
{
    double d_x;
    double d_y;

    SCoordinate();
    SCoordinate(double dX, double dY);
};

class CEvaluator 
{
public:
    CEvaluator();
    ~CEvaluator();

    // Wczytuje instancje z pliku - zwraca true jesli sukces
    bool bLoadInstance(const std::string& sFilePath, int iNumGroups);

    // Oblicza fitness rozwiazania
    double dEvaluate(const std::vector<int>& vSolution) const;

    // Gettery
    int iGetNumCustomers() const;
    int iGetNumGroups() const;
    int iGetSolutionSize() const;
    int iGetLowerBound() const;
    int iGetUpperBound() const;
    int iGetCapacity() const;
    std::string sGetName() const;
    std::string sGetLastError() const;
    bool bIsLoaded() const;

private:
    std::string s_name;
    int i_dimension;
    int i_capacity;
    int i_depot;
    int i_num_groups;
    bool b_loaded;
    std::string s_last_error;

    std::vector<SCoordinate> v_coordinates;
    std::vector<int> v_demands;
    std::vector<int> v_permutation;

    double dCalculateDistance(int iFrom, int iTo) const;
    double dCalculateRouteCost(const std::vector<int>& vRoute) const;
    bool bValidateSolution(const std::vector<int>& vSolution) const;
    void vBuildRoutes(const std::vector<int>& vGrouping, 
                      std::vector<std::vector<int> >& vRoutes) const;

    bool bParseCoordinates(std::ifstream& cFile);
    bool bParseDemands(std::ifstream& cFile);
    bool bParseDepot(std::ifstream& cFile);
    std::string sTrim(const std::string& sInput);
};

#endif
