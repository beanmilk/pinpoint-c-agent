//
// Created by Bluse on 12/8/2019.
//

#ifndef COMMON_SHAREDOBJ_H
#define COMMON_SHAREDOBJ_H

#include "common.h"


bool init_shared_obj();
bool register_shared_obj_address();
void* fetch_shared_address();
bool checking_and_init();



#endif //COMMON_SHAREDOBJ_H
