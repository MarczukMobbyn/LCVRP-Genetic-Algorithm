#ifndef CMYSMARTPOINTER_H
#define CMYSMARTPOINTER_H

#include <cstddef>
#include "CRefCounter.h"

template <typename T>
class CMySmartPointer
{
public:
    // Domyslny konstruktor - tworzy pusty smart pointer
    CMySmartPointer()
    {
        pc_pointer = NULL;
        pc_counter = new CRefCounter<T>();
        pc_counter->iAdd();
        pc_counter->vAddPointer(this);
    }

    // Konstruktor z wskaznikiem
    CMySmartPointer(T* pcPointer)
    {
        pc_pointer = pcPointer;
        pc_counter = new CRefCounter<T>();
        pc_counter->iAdd();
        pc_counter->vAddPointer(this);
    }

    // Konstruktor kopiujacy
    CMySmartPointer(const CMySmartPointer& cOther)
    {
        pc_pointer = cOther.pc_pointer;
        pc_counter = cOther.pc_counter;
        pc_counter->iAdd();
        pc_counter->vAddPointer(this);
    }

    // Destruktor
    ~CMySmartPointer()
    {
        pc_counter->vRemovePointer(this);

        if (pc_counter->iDec() == 0)
        {
            if (pc_pointer != NULL)
            {
                delete pc_pointer;
            }
            delete pc_counter;
        }
    }

    // Operator przypisania
    CMySmartPointer& operator=(const CMySmartPointer& cOther)
    {
        if (this != &cOther)
        {
            pc_counter->vRemovePointer(this);

            if (pc_counter->iDec() == 0)
            {
                if (pc_pointer != NULL)
                {
                    delete pc_pointer;
                }
                delete pc_counter;
            }

            pc_pointer = cOther.pc_pointer;
            pc_counter = cOther.pc_counter;

            pc_counter->vAddPointer(this);
            pc_counter->iAdd();
        }

        return *this;
    }

    int iGetCount() const 
    {
        return pc_counter->iGet();
    }

    T& operator*() 
    { 
        return (*pc_pointer); 
    }
    
    T* operator->() 
    { 
        return pc_pointer; 
    }

    const T& operator*() const 
    { 
        return (*pc_pointer); 
    }
    
    const T* operator->() const 
    { 
        return pc_pointer; 
    }

    bool bIsNull() const
    {
        return pc_pointer == NULL;
    }

private:
    CRefCounter<T>* pc_counter;
    T* pc_pointer;
};

#endif
