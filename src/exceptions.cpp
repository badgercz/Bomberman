//
// Created by badger on 6/14/16.
//

#include "exceptions.h"

InitException::InitException(const char * desc):m_Desc (desc){}
const char* InitException::what() const noexcept {
    return m_Desc;
}

