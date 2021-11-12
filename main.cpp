//
//  VUT FIT Klient POP3 s podporou TLS
//  main.cpp
//  isa
//  Created by Simon Fenko
//  xfenko01
//
#include "ParseParams.h"

#include <vector>
#include <iostream>

#include <dirent.h>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <regex>

/* OpenSSL headers*/

# include  "openssl/bio.h"
# include  "openssl/ssl.h"
# include  "openssl/err.h"

#include "pop3.h"

using namespace std;
/*
typedef struct opts
{
    char* port;
    char* auth_file;
    char* certfile;
    char* certaddr;
    char* out_dir;
} opts;
*/

int main(int argc, char *argv[])
{
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();

    globals* globs = Parse_params(argc, argv);
    // Parameter -T is given
    if (globs->T_sec == true)
    {
        auto ctx = get_cert(string(globs->certaddr), string(globs->certfile));
        auto cbio = connect_to_sec_server(globs->server, globs->data, ctx);
        send_to_server(cbio, "");
        credentials credentials;
        load_credentials(&credentials, globs->authfile);
        login(*credentials.username, *credentials.password,cbio);
        if ((globs->isnew) && (globs->del == false))
        {
            download_new_messages(globs->out_dir, cbio);
        }
        else if ((globs->isnew == false) && (globs->del == false))
        {
            download_all_messages(globs->out_dir, cbio);
        }
        else if ((globs->isnew == false) && (globs->del))
        {
            delete_all_messages(cbio);
        }
        else if ((globs->isnew) && (globs->del))
        {
            delete_new_messages(cbio,globs->out_dir);
        }
        quit(cbio);
        BIO_free_all(cbio);
    }
    else if (globs->T_sec == false)
    {
        // If parameter -S is given
        if(globs->S_nsec)
        {
            auto ctx = get_cert(string(globs->certaddr), string(globs->certfile));
            auto cbio = connect_stls_server(globs->server, globs->data, ctx);
            //send_to_server(cbio, "");
            credentials credentials;
            load_credentials(&credentials, globs->authfile);
            login(*credentials.username, *credentials.password,cbio);
            if ((globs->isnew) && (globs->del == false))
            {
                download_new_messages(globs->out_dir, cbio);
            }
            else if ((globs->isnew == false) && (globs->del == false))
            {
                download_all_messages(globs->out_dir, cbio);
            }
            else if ((globs->isnew == false) && (globs->del))
            {
                delete_all_messages(cbio);
            }
            else if ((globs->isnew) && (globs->del))
            {
                delete_new_messages(cbio,globs->out_dir);
            }
            quit(cbio);
            BIO_free_all(cbio);
        }
        // Parameter -S and -T are not given
        else
        {
            auto cbio = connect_to_server(globs->server, globs->data);
            send_to_server(cbio, "");
            credentials credentials;
            credentials.username = NULL;
            credentials.password = NULL;
            load_credentials(&credentials, globs->authfile);
            login(*credentials.username, *credentials.password,cbio);
            //download_messages("filename", cbio);
            if ((globs->isnew) && (globs->del == false))
            {
                download_new_messages(globs->out_dir, cbio);
            }
            else if ((globs->isnew == false) && (globs->del == false))
            {
                download_all_messages(globs->out_dir, cbio);
            }
            else if ((globs->isnew == false) && (globs->del))
            {
                delete_all_messages(cbio);
            }
            else if ((globs->isnew) && (globs->del))
            {
                delete_new_messages(cbio,globs->out_dir);
            }
            quit(cbio);
            BIO_free_all(cbio);
        }
    }
}
