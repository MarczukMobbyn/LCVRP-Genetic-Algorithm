#include "CEvaluator.h"
#include <fstream>
#include <sstream>
#include <cmath>

// ============================================
// SCOORDINATE
// ============================================

SCoordinate::SCoordinate()
{
    d_x = 0.0;
    d_y = 0.0;
}

SCoordinate::SCoordinate(double dX, double dY)
{
    d_x = dX;
    d_y = dY;
}

// ============================================
// CEVALUATOR - KONSTRUKTOR I DESTRUKTOR
// ============================================

CEvaluator::CEvaluator()
{
    s_name = "";
    i_dimension = 0;
    i_capacity = 0;
    i_depot = DEFAULT_DEPOT_INDEX;
    i_num_groups = 0;
    b_loaded = false;
    s_last_error = "";
}

CEvaluator::~CEvaluator()
{
}

// ============================================
// GETTERY
// ============================================

int CEvaluator::iGetNumCustomers() const
{
    return i_dimension - 1;
}

int CEvaluator::iGetNumGroups() const
{
    return i_num_groups;
}

int CEvaluator::iGetSolutionSize() const
{
    return i_dimension - 1;
}

int CEvaluator::iGetLowerBound() const
{
    return 0;
}

int CEvaluator::iGetUpperBound() const
{
    return i_num_groups - 1;
}

int CEvaluator::iGetCapacity() const
{
    return i_capacity;
}

std::string CEvaluator::sGetName() const
{
    return s_name;
}

std::string CEvaluator::sGetLastError() const
{
    return s_last_error;
}

bool CEvaluator::bIsLoaded() const
{
    return b_loaded;
}

// ============================================
// FUNKCJE POMOCNICZE
// ============================================

std::string CEvaluator::sTrim(const std::string& sInput)
{
    std::string sResult = "";
    
    int iStart = 0;
    int iEnd = static_cast<int>(sInput.length()) - 1;
    
    while (iStart <= iEnd && (sInput[iStart] == ' ' || sInput[iStart] == '\t' || 
           sInput[iStart] == '\r' || sInput[iStart] == '\n'))
    {
        iStart++;
    }
    
    while (iEnd >= iStart && (sInput[iEnd] == ' ' || sInput[iEnd] == '\t' || 
           sInput[iEnd] == '\r' || sInput[iEnd] == '\n'))
    {
        iEnd = iEnd - 1;
    }
    
    if (iStart <= iEnd)
    {
        sResult = sInput.substr(iStart, iEnd - iStart + 1);
    }
    
    return sResult;
}

// ============================================
// WCZYTYWANIE PLIKU
// ============================================

bool CEvaluator::bLoadInstance(const std::string& sFilePath, int iNumGroups)
{
    std::ifstream cFile(sFilePath.c_str());
    
    if (!cFile.is_open())
    {
        s_last_error = ERROR_MSG_FILE_NOT_FOUND;
        return false;
    }

    i_num_groups = iNumGroups;
    b_loaded = false;

    bool bHasCoords = false;
    bool bHasDemands = false;
    bool bHasDepot = false;
    bool bFinished = false;
    bool bError = false;

    std::string sLine;
    
    while (std::getline(cFile, sLine) && !bFinished && !bError)
    {
        if (sLine.length() > 0)
        {
            if (sLine.find(KEYWORD_NAME) != std::string::npos)
            {
                size_t iPos = sLine.find(FILE_SEPARATOR);
                if (iPos != std::string::npos)
                {
                    s_name = sTrim(sLine.substr(iPos + 1));
                }
            }
            else if (sLine.find(KEYWORD_DIMENSION) != std::string::npos)
            {
                size_t iPos = sLine.find(FILE_SEPARATOR);
                if (iPos != std::string::npos)
                {
                    i_dimension = std::stoi(sTrim(sLine.substr(iPos + 1)));
                    
                    if (i_dimension < 2)
                    {
                        s_last_error = ERROR_MSG_INVALID_FORMAT;
                        bError = true;
                    }
                    else
                    {
                        v_coordinates.resize(i_dimension);
                        v_demands.resize(i_dimension);
                    }
                }
            }
            else if (sLine.find(KEYWORD_CAPACITY) != std::string::npos)
            {
                size_t iPos = sLine.find(FILE_SEPARATOR);
                if (iPos != std::string::npos)
                {
                    i_capacity = std::stoi(sTrim(sLine.substr(iPos + 1)));
                    
                    if (i_capacity < 1)
                    {
                        s_last_error = ERROR_MSG_INVALID_FORMAT;
                        bError = true;
                    }
                }
            }
            else if (sLine.find(KEYWORD_PERMUTATION) != std::string::npos)
            {
                size_t iPos = sLine.find(FILE_SEPARATOR);
                if (iPos != std::string::npos)
                {
                    std::istringstream cStream(sLine.substr(iPos + 1));
                    int iCustomerId;
                    v_permutation.clear();
                    
                    while (cStream >> iCustomerId)
                    {
                        v_permutation.push_back(iCustomerId);
                    }
                }
            }
            else if (sLine.find(KEYWORD_NODE_COORD) != std::string::npos)
            {
                if (!bParseCoordinates(cFile))
                {
                    s_last_error = ERROR_MSG_INVALID_FORMAT;
                    bError = true;
                }
                else
                {
                    bHasCoords = true;
                }
            }
            else if (sLine.find(KEYWORD_DEMAND) != std::string::npos)
            {
                if (!bParseDemands(cFile))
                {
                    s_last_error = ERROR_MSG_INVALID_FORMAT;
                    bError = true;
                }
                else
                {
                    bHasDemands = true;
                }
            }
            else if (sLine.find(KEYWORD_DEPOT) != std::string::npos)
            {
                if (!bParseDepot(cFile))
                {
                    s_last_error = ERROR_MSG_INVALID_FORMAT;
                    bError = true;
                }
                else
                {
                    bHasDepot = true;
                }
            }
            else if (sLine.find(KEYWORD_EOF) != std::string::npos)
            {
                bFinished = true;
            }
        }
    }

    cFile.close();

    if (!bError)
    {
        if (!bHasCoords || !bHasDemands || !bHasDepot || i_dimension < 2)
        {
            s_last_error = ERROR_MSG_INVALID_FORMAT;
            bError = true;
        }
    }

    if (!bError)
    {
        if (v_permutation.empty())
        {
            for (int i = 2; i <= i_dimension; i = i + 1)
            {
                v_permutation.push_back(i);
            }
        }
        b_loaded = true;
    }
    
    return !bError;
}

bool CEvaluator::bParseCoordinates(std::ifstream& cFile)
{
    bool bSuccess = true;
    int i = 0;
    
    while (i < i_dimension && bSuccess)
    {
        int iNodeId;
        double dX;
        double dY;
        
        if (cFile >> iNodeId >> dX >> dY)
        {
            if (iNodeId >= 1 && iNodeId <= i_dimension)
            {
                v_coordinates[iNodeId - 1] = SCoordinate(dX, dY);
            }
            else
            {
                bSuccess = false;
            }
        }
        else
        {
            bSuccess = false;
        }
        
        i++;
    }
    
    return bSuccess;
}

bool CEvaluator::bParseDemands(std::ifstream& cFile)
{
    bool bSuccess = true;
    int i = 0;
    
    while (i < i_dimension && bSuccess)
    {
        int iNodeId;
        int iDemand;
        
        if (cFile >> iNodeId >> iDemand)
        {
            if (iNodeId >= 1 && iNodeId <= i_dimension)
            {
                v_demands[iNodeId - 1] = iDemand;
            }
            else
            {
                bSuccess = false;
            }
        }
        else
        {
            bSuccess = false;
        }
        
        i++;
    }
    
    return bSuccess;
}

bool CEvaluator::bParseDepot(std::ifstream& cFile)
{
    bool bSuccess = true;
    
    if (cFile >> i_depot)
    {
        int iTerminator;
        cFile >> iTerminator;
    }
    else
    {
        bSuccess = false;
    }
    
    return bSuccess;
}

// ============================================
// OBLICZANIE ODLEGLOSCI
// ============================================

double CEvaluator::dCalculateDistance(int iFrom, int iTo) const
{
    double dResult = WRONG_FITNESS_VALUE;
    
    if (iFrom >= 0 && iFrom < i_dimension && iTo >= 0 && iTo < i_dimension)
    {
        double dDx = v_coordinates[iFrom].d_x - v_coordinates[iTo].d_x;
        double dDy = v_coordinates[iFrom].d_y - v_coordinates[iTo].d_y;
        dResult = std::sqrt(dDx * dDx + dDy * dDy);
    }
    
    return dResult;
}

// ============================================
// WALIDACJA ROZWIAZANIA
// ============================================

bool CEvaluator::bValidateSolution(const std::vector<int>& vSolution) const
{
    bool bValid = true;
    
    if (static_cast<int>(vSolution.size()) != iGetSolutionSize())
    {
        bValid = false;
    }
    
    int i = 0;
    while (bValid && i < static_cast<int>(vSolution.size()))
    {
        if (vSolution[i] < iGetLowerBound() || vSolution[i] > iGetUpperBound())
        {
            bValid = false;
        }
        i++;
    }
    
    return bValid;
}

// ============================================
// BUDOWANIE TRAS
// ============================================

void CEvaluator::vBuildRoutes(const std::vector<int>& vGrouping, 
                              std::vector<std::vector<int> >& vRoutes) const
{
    vRoutes.clear();
    vRoutes.resize(i_num_groups);

    for (int i = 0; i < static_cast<int>(v_permutation.size()); i = i + 1)
    {
        int iCustomerId = v_permutation[i];
        int iCustomerIndex = iCustomerId - 2;

        if (iCustomerIndex >= 0 && iCustomerIndex < static_cast<int>(vGrouping.size()))
        {
            int iGroup = vGrouping[iCustomerIndex];
            vRoutes[iGroup].push_back(iCustomerId);
        }
    }
}

// ============================================
// OBLICZANIE KOSZTU TRASY
// ============================================

double CEvaluator::dCalculateRouteCost(const std::vector<int>& vRoute) const
{
    double dTotalCost = 0.0;
    bool bError = false;
    
    if (vRoute.size() > 0)
    {
        int iDepotIndex = i_depot - 1;
        int iCurrentLoad = 0;
        double dCurrentDistance = 0.0;
        int iLastPositionIndex = iDepotIndex;
        
        int i = 0;
        while (i < static_cast<int>(vRoute.size()) && !bError)
        {
            int iCustomerId = vRoute[i];
            int iCustomerIndex = iCustomerId - 1;
            int iCustomerDemand = v_demands[iCustomerIndex];

            if (iCurrentLoad + iCustomerDemand > i_capacity)
            {
                double dDistToDepot = dCalculateDistance(iLastPositionIndex, iDepotIndex);
                
                if (dDistToDepot < 0.0)
                {
                    bError = true;
                }
                else
                {
                    dCurrentDistance = dCurrentDistance + dDistToDepot;
                    dTotalCost = dTotalCost + dCurrentDistance;

                    iCurrentLoad = 0;
                    dCurrentDistance = 0.0;
                    iLastPositionIndex = iDepotIndex;
                }
            }

            if (!bError)
            {
                double dDistToCustomer = dCalculateDistance(iLastPositionIndex, iCustomerIndex);
                
                if (dDistToCustomer < 0.0)
                {
                    bError = true;
                }
                else
                {
                    dCurrentDistance = dCurrentDistance + dDistToCustomer;
                    iCurrentLoad = iCurrentLoad + iCustomerDemand;
                    iLastPositionIndex = iCustomerIndex;
                }
            }
            
            i++;
        }

        if (!bError)
        {
            double dDistToDepot = dCalculateDistance(iLastPositionIndex, iDepotIndex);
            
            if (dDistToDepot < 0.0)
            {
                bError = true;
            }
            else
            {
                dCurrentDistance = dCurrentDistance + dDistToDepot;
                dTotalCost = dTotalCost + dCurrentDistance;
            }
        }
    }

    if (bError)
    {
        dTotalCost = WRONG_FITNESS_VALUE;
    }
    
    return dTotalCost;
}

// ============================================
// EWALUACJA
// ============================================

double CEvaluator::dEvaluate(const std::vector<int>& vSolution) const
{
    double dResult = WRONG_FITNESS_VALUE;
    
    if (b_loaded && bValidateSolution(vSolution))
    {
        std::vector<std::vector<int> > vRoutes;
        vBuildRoutes(vSolution, vRoutes);

        double dTotalCost = 0.0;
        bool bValid = true;
        
        int i = 0;
        while (bValid && i < static_cast<int>(vRoutes.size()))
        {
            double dRouteCost = dCalculateRouteCost(vRoutes[i]);
            
            if (dRouteCost < 0.0)
            {
                bValid = false;
            }
            else
            {
                dTotalCost = dTotalCost + dRouteCost;
            }
            
            i++;
        }

        if (bValid)
        {
            dResult = dTotalCost;
        }
    }

    return dResult;
}
