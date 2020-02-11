﻿#include <gtest/gtest.h>
#include "common.h"

PPAgentT global_agent_info = {
       "unix:./pinpoint.sock",
        1,10
};

int main(int argc, char **argv) {
    register_error_cb(NULL);

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}