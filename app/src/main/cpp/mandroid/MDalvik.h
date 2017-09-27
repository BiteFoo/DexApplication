//
// Created by John.Lu on 2017/9/27.
//

#ifndef DEXAPPLICATION_MDALVIK_H
#define DEXAPPLICATION_MDALVIK_H

void* get_module_base(const char* modulename ,void* startAdd,void* endAddr);
void changeCode(void* baseAddr);

void call_changeCode(const char* moduleName );

#endif //DEXAPPLICATION_MDALVIK_H
