//
// Created by badger on 6/14/16.
//
#include <string>

#ifndef BOMBERMAN_SEMESTRALKA_EXCEPTIONS_H
#define BOMBERMAN_SEMESTRALKA_EXCEPTIONS_H

/**
* InitException serves as exception for initial problems.
*/
class InitException{
public:
  /**
  * Contructor
  * @param[in] string that represents problem with the understandable description.
  */
  InitException (const char * );
  /**
  * Method that tells to user where problem is.
  * @return Text with the problem description.
  */
  const char *what (void) const noexcept;
private:
    const char * m_Desc;
};



#endif //BOMBERMAN_SEMESTRALKA_EXCEPTIONS_H
