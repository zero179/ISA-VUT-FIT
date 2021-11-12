//
// Created by simon on 07.11.21.
//

#ifndef ISA_POP3_H
#define ISA_POP3_H

#include <fstream>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <regex>

# include  "openssl/bio.h"
# include  "openssl/ssl.h"
# include  "openssl/err.h"

using namespace std;

typedef struct credential
{
    string* username;
    string* password;
} credentials;

void help();

/**
 * @brief Checks if message response is OK
 * @param message message
 * @return true if message is ok, otherwise false
 */
bool ok(string message);

/**
 * @brief Establishes unsecuried connection to the server
 * @param server server hostname
 * @param port port
 * @return new bio object
 */
BIO* connect_to_server(string server, int port);

/**
 * @brief Sends message to server
 * @param cbio bio object
 * @param msg message string, if its an empty string just reads response from server
 * @return message response
 */
string send_to_server(BIO *cbio, char* msg, bool multi_line=false);

/**
 * @brief Checks and load certicikates
 * @param server server hostname
 * @param port port
 * @param cert_dir directory with certificate files
 * @param cert_file file with certificates
 * @return context object
 */
SSL_CTX* get_cert(string cert_dir, string cert_file);

/**
 * @brief established stls connection to server
 * @param server server hostname
 * @param port port
 * @param ctx context object
 * @return new bio object
 */
BIO *connect_stls_server(string server, int port, SSL_CTX* ctx);

/**
 * @brief Established secured connection to server
 * @param server server hostname
 * @param port port
 * @param ctx context object
 * @return new bio object
 */
BIO* connect_to_sec_server(string server, int port,SSL_CTX* ctx);

/**
 * @brief Gets count of unread messages
 * @param cbio bio object
 * @return count of unread messages
 */
int stat(BIO *cbio);

/**
 * @brief Gets the message with given ID
 * @param cbio bio object
 * @param a given ID
 * @return message with ID
 */
string retr(BIO *cbio, int a);

/**
 * @brief Downloads and stores unread messages into out_dir
 * @param out_dir Output directory where is messages stored
 * @param cbio bio object
 */
string download_all_messages(string out_dir, BIO *cbio);

/**
 * @brief Deletes all messages from mailbox
 * @param cbio bio object
 */
string delete_all_messages(BIO *cbio);

/**
 * @brief Commits changes
 * @param cbio bio object
 */
bool quit(BIO *cbio);

/**
 * @brief Sends loaded username and password to server
 * @param username username
 * @param password password
 * @param cbio bio object
 * @return true if ok, otherwise false
 */
bool login(string username, string password,BIO *cbio);

/**
 * @brief Splits strings bz given delimiter
 * @param s string
 * @param delimiter is character which splits a string
 * @return
 */
vector<string> split (string s, string delimiter);

/**
 *
 * @param cbio
 * @return
 */
string delete_new_messages(BIO* cbio,string out_dir);

/**
 * @brief Downloades only new messages from mailbox
 * @param out_dir output directory
 * @param cbio bio object
 */
string download_new_messages(string out_dir, BIO *cbio);

/**
 * @brief Loads credentials from auth_file
 * @param credentials username and password
 * @param auth auth_file
 * @return true if the content of the file is correct
 */
bool load_credentials(credentials* credentials, string auth);

#endif //ISA_POP3_H

