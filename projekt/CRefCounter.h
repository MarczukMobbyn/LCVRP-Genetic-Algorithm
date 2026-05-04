#ifndef CREFCOUNTER_H
#define CREFCOUNTER_H

#include <vector>

template <typename T>
class CMySmartPointer;

template <typename T>
class CRefCounter
{
private:
    int i_count;
    std::vector<CMySmartPointer<T>*> v_all_pointers;

public:
    CRefCounter() 
    { 
        i_count = 0; 
    }

    int iAdd() 
    { 
        i_count = i_count + 1;
        return i_count; 
    }
    
    int iDec() 
    { 
        i_count = i_count - 1;
        return i_count; 
    }
    
    int iGet() const 
    { 
        return i_count; 
    }

    void vAddPointer(CMySmartPointer<T>* pcSmartPointer) 
    {
        v_all_pointers.push_back(pcSmartPointer);
    }

    void vRemovePointer(CMySmartPointer<T>* pcSmartPointer) 
    {
        int i = 0;
        bool bFound = false;
        
        while (i < static_cast<int>(v_all_pointers.size()) && !bFound)
        {
            if (v_all_pointers[i] == pcSmartPointer) 
            {
                v_all_pointers.erase(v_all_pointers.begin() + i);
                bFound = true;
            }
            i = i + 1;
        }
    }

    std::vector<CMySmartPointer<T>*>& vGetAllPointers() 
    {
        return v_all_pointers;
    }
};

#endif
