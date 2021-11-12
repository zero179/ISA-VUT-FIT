//
// Created by simon on 07.11.21.
//
#include "ParseParams.h"
#include "pop3.h"

globals* Parse_params(int argc, char *argv[]){
    globals *globs = new globals;
    // Checks, if count of arguments is right
    if (argc < 2)
    {
        cerr<<"There were given too few arguments, use --help ";
        exit(1);
    }
    // It shows help manual
    if ((!strcmp(argv[1],"--help")) && argc == 2)
    {
        help();
        exit(1);
    }
    // Checks, if there are only two arguments and argument[1] is "--help"
    if ((!strcmp(argv[1], "--help")) && argc !=2)
    {
        cerr<<"Wrong arguments\n";
        exit(1);
    }
    // Cycle for parsing arguments
    for (int i=1;i<argc;i++)
    {
        // If argument is "-p"
        if (!strcmp(argv[i], "-p"))
        {
            // If port is set as false, it means, that the parameter appeared for the first time
            if (globs->port_b == false)
            {
                // Set port as true, so it does not appear a second time
                globs->port_b = true;
                // Parameter of -p is stored in data_port
                globs->data_port = argv[i+1];
                // Checks if "-p" has a parameter
                if(i+1 < argc)
                {
                    if ((!strcmp(argv[i+1], "110")) || (!strcmp(argv[i+1], "995")))
                    {
                        i++;
                    }
                    else
                    {
                        cerr<<"Wrong -p argument\n";
                        exit(1);
                    }
                }
                else
                {
                    cerr<<"Missing value of -p parameter\n";
                    exit(1);
                }
            }
            else
            {
                cerr<<"Parameter -p has already been used\n";
                exit(1);
            }
            continue;
        }
        else if (!strcmp(argv[i], "-T"))
        {
            // And if T_sec and S_nsec is false, so this is first occurance
            // of parameter -T and parameter -S was not used
            if (globs->T_sec == false && globs->S_nsec == false)
            {
                // Set T_sec on true, to avoid the second occurance
                globs->T_sec = true;
            }
            else
            {
                cerr<<"Parameter -T has already been used\n";
                exit(1);
            }
        }
            // If parameter is "-S"
        else if (!strcmp(argv[i], "-S"))
        {
            // And if S_nsec and T_sec is false, so this is the first occurance
            // of parameter -S and parameter -S was not used
            if (globs->S_nsec == false && globs->T_sec == false)
            {
                // Set S_nsec on true, to avoid the second occurance
                globs->S_nsec = true;
            }
            else
            {
                cerr<<"Parameter -S has already been used\n";
                exit(1);
            }
        }
            // If parameter is "-c"
        else if (!strcmp(argv[i], "-c"))
        {
            // And c_file is false, that means the parameter appeared for the first time
            if (globs->c_file == false)
            {
                // Set c_file on true, to avoid the second occurance
                globs->c_file = true;
                globs->certfile = argv[i+1];
                // Checks if "-c" has a parameter "cert_file"
                if(i+1 < argc){
                    i++;
                }
                else{
                    cerr<<"Missing value of -c parameter\n";
                    exit(0);
                }
            }
            else
            {
                cerr<<"Parameter -c has already been used\n";
                exit(1);
            }
            continue;
        }
            // If parameter is "-C"
        else if (!strcmp(argv[i], "-C"))
        {
            // And C_addr is false, so that means the parameter appeared for the first time
            if (globs->C_addr == false)
            {
                // set C_addr on true, to avoid the second occurance
                globs->C_addr = true;
                globs->certaddr = argv[i+1];
                // Checks if "-C" has parameter cert_addr
                if(i+1 < argc){
                    i++;
                }
                else{
                    cerr<<"Missing value of -C parameter\n";
                    exit(0);
                }
            }
            else
            {
                cerr<<"Parameter -C has already been used\n";
                exit(1);
            }
            continue;
        }
            // If parameter is "-d"
        else if (!strcmp(argv[i], "-d"))
        {
            // And del is false, so that means the parameter appeared for the first time
            if (globs->del == false)
            {
                // Set del on true, to avoid the second occurance
                globs->del = true;
            }
            else
            {
                cerr<<"Parameter -d has already been used\n";
                exit(1);
            }
        }
            // If parameter is "-n"
        else if (!strcmp(argv[i], "-n"))
        {
            // And isnew is false, so that means the parameter appeared for the first time
            if (globs->isnew == false)
            {
                // Set isnew on true, to avoid the second occurance
                globs->isnew = true;
            }
            else
            {
                cerr<<"Parameter -n has already been used\n";
                exit(1);
            }
        }
            // If parameter is "-a"
        else if (!strcmp(argv[i], "-a"))
        {
            // And auth is false, so that means the parameter appeared for the first time
            if (globs->auth == false)
            {
                // Set auth on true, to avoid the second occurance
                globs->auth = true;
                globs->authfile = argv[i+1];
                // Checks if "-a" has parameter auth_file
                if(i+1 < argc) {
                    i++;
                }
                else
                {
                    cerr<<"Missing value of -a parameter\n";
                    exit(1);
                }
            }
            else
            {
                cerr<<"Parameter -a has already been used\n";
                exit(1);
            }
            continue;
        }
            // If parameter is "-o"
        else if (!strcmp(argv[i], "-o"))
        {
            // And out is false, so that means the parameter appeared for the first time
            if (globs->out == false)
            {
                // Set out on true, to avoid the second occurance
                globs->out = true;
                globs->out_dir = argv[i+1];
                // Checks if "-o" has parameter out_dir
                if (i+1 < argc)
                {
                    i++;
                }
                else{
                    cerr<<"Missing value of -o parameter\n";
                    exit(1);
                }
            }
            else
            {
                cerr<<"Parameter -o has already been used\n";
                exit(1);
            }
            continue;
        }
            // Else parameter is taken as a server
        else
        {
            // And server is false, so it appeard for the first time
            if (globs->server_b == false)
            {
                // Set server on true, to avoid the second occurance
                globs->server_b = true;
                globs->server = argv[i];
            }
            else
            {
                cerr<<"Parameter server has already been used\n";
                exit(1);
            }
        }

        /*
         * Checking, that all mandatory parameters have been used
         */
    }
    if (globs->server_b != true)
    {
        cerr<<"Missing argument server\n";
        exit(1);
    }
    if (globs->auth != true)
    {
        cerr<<"Missing argument -a auth_file\n";
        exit(1);
    }
    if (globs->out != true)
    {
        cerr<<"Missing argument -o out_dir\n";
        exit(1);
    }
    // Checks if parameter -c is used with -T or -S
    if (globs->c_file == true && (globs->T_sec != true && globs->S_nsec != true))
    {
        cerr<<"Parameter -c is without -T or -S\n";
        exit(1);
    }
    // Checks if parameter -C is used with -T or -S
    if (globs->C_addr == true && (globs->T_sec != true && globs->S_nsec != true))
    {
        cerr<<"Parameter -C is without -T or -S\n";
        exit(1);
    }
    // Parameters -p, -S, -T are not given
    if ((globs->port_b == false) && (globs->S_nsec == false) && (globs->T_sec == false))
    {
        // In that case port is 110
        globs->data = 110;
    }
        // Parameters -p, -S are not given
    else if ((globs->port_b == false) && (globs->S_nsec == false) && (globs->T_sec == true))
    {
        // In that case port is 995
        globs->data = 995;
    }
        // Parameters -p, -T are not given
    else if ((globs->port_b == false) && (globs->S_nsec == true) && (globs->T_sec == false))
    {
        // In that case port is 110
        globs->data = 110;
    }
    else
    {
        // Convert char ptr data_port argument into integer
        globs->data = atoi(globs->data_port);
    }

    return globs;
}