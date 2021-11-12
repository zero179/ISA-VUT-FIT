//
// Created by simon on 07.11.21.
//

#ifndef ISA_PARSEPARAMS_H
#define ISA_PARSEPARAMS_H

#include <vector>
#include <iostream>
#include <dirent.h>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <regex>

typedef struct globals{
    char* data_port;
    char* server;
    char* authfile;
    char* certfile = "";
    char* certaddr = "";
    char* out_dir;
    bool port_b = false;
    bool T_sec = false;
    bool S_nsec = false;
    bool c_file = false;
    bool C_addr = false;
    bool del = false;
    bool isnew = false;
    bool auth = false;
    bool out = false;
    bool server_b = false;
    int data;
}globals;


globals* Parse_params(int argc, char *argv[]);

#endif //ISA_PARSEPARAMS_H
