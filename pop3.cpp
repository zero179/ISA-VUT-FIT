//
// Created by simon on 07.11.21.
//

#include "pop3.h"

void help()
{
    std::cout << "Klient POP3 s podporou TLS" << '\n';
    std::cout << "Pouzitie:" << '\n';
    std::cout << "popcl --help ---> zobrazi tuto napovedu a ukonci sa" << '\n';
    std::cout << "popcl server  [-p <port>] [-T|-S [-c <certfile>]" << '\n';
    std::cout << "[-C <certaddr>]] [-d] [-n] -a <auth_file> -o <out_dir>" << '\n';
    std::cout << "server          - povinny parameter (IP adresa nebo domenove meno)" << '\n';
    std::cout << "-p <port>       - specifikuje cislo portu na serveru" << '\n';
    std::cout << "-T              - zapina sifrovani" << '\n';
    std::cout << "-S              - navaze nesifrovane spojeni se serverem a pomoci prikazu" << '\n';
    std::cout << "                  STLS prejde na sifrovanou verzi protokolu" << '\n';
    std::cout << "-c <certfile>   - definuje soubor s certifikaty" << '\n';
    std::cout << "-C <certaddr>   - definuje adresar ve kterem se maji vyhledavat certifikaty" << '\n';
    std::cout << "-d              - smaze zpravy" << '\n';
    std::cout << "-n              - stahnou se pouze nove zpravy" << '\n';
    std::cout << "-a <auth_file>  - povinny parametr, vynucuje autentizaci" << '\n';
    std::cout << "-o <out_dir>    - povinny parametr, specifikuje vystupni adresar" << '\n';
    exit(0);
}


bool ok(string message)
{
    //string message;
    if(message.find("+OK ") != -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

BIO* connect_to_server(string server, int port)
{
    string buffer;
    BIO *cbio;
    cbio = BIO_new_connect(string(server + ":" + to_string(port)).c_str());
    if (cbio == NULL)
    {
        cerr<< "Error connecting to server\n";
        exit(3);
    }
    if (BIO_do_connect(cbio) <= 0)
    {
        cerr<< "Error connecting to server\n";
        exit(3);
    }
    return cbio;
}

string send_to_server(BIO *cbio, char* msg, bool multi_line)
{
    int len;
    string message;
    char tmpbuf[1024];
    memset(tmpbuf, '\0', 1024);
    // // Function gets non empty string, it sends and waits for response
    if(strcmp(msg,""))
    {
        if(BIO_write(cbio, msg, strlen(msg))<=0)
        {
            if(!BIO_should_retry(cbio))
            {
                cerr<< "Error connecting to server\n";
                exit(3);
            }
        }
    }
    while(1)
    {
        len = BIO_read(cbio, tmpbuf, 1024);
        if (len == 0)
        {
            break;
        }
        else if (len < 0)
        {
            if (! BIO_should_retry(cbio))
            {
                cerr<< "Error connecting to server\n";
                exit(3);
            }
        }
        message += string(tmpbuf);
        // Checking if message is multiline type or single line type
        if(multi_line)
        {
            if(!strcmp("\r\n.\r\n", tmpbuf + strlen(tmpbuf) - 5)) break;
        }

        else
        {
            if(!strcmp("\r\n", tmpbuf + strlen(tmpbuf) - 2)) break;
        }
        memset(tmpbuf, '\0', 1024);
    }
    return message;
}

SSL_CTX* get_cert(string cert_dir, string cert_file)
{
    string buffer;
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    if (!ctx)
    {
        cerr<<"Error ctx\n";
        exit(5);
    }
    if (cert_file != "")
    {
        if (!SSL_CTX_load_verify_locations(ctx, cert_file.c_str(), NULL))
        {
            cerr<<"File with TLS certificates not found\n";
            exit(6);
        }
    }
    else if (cert_dir != "")
    {
        if (!SSL_CTX_load_verify_locations(ctx, NULL, cert_dir.c_str()))
        {
            cerr<<"Folder with TLS certificates not found\n";
            exit(6);
        }
    }
    else
    {
        SSL_CTX_set_default_verify_paths(ctx);
    }
    return ctx;
}

BIO *connect_stls_server(string server, int port, SSL_CTX* ctx)
{
    SSL *ssl;
    BIO *cbio;
    cbio = connect_to_server(server, port);
    send_to_server(cbio, ""); //overit
    string stls = send_to_server(cbio, "STLS\r\n");
    BIO *ret = NULL, *sslC = NULL;
    sslC = BIO_new_ssl(ctx,1);
    ret = BIO_push(sslC, cbio);
    BIO_get_ssl(ret, &ssl);

    if (BIO_do_connect(cbio) <= 0)
    {
        cerr<<"Can not connect to server\n";
        exit(3);
    }
    if(SSL_get_verify_result(ssl)!= X509_V_OK)
    {
        cerr<<"Invalid certificate\n";
        exit(3);
    }
    cbio=ret;
    return cbio;
}

BIO* connect_to_sec_server(string server, int port,SSL_CTX* ctx)
{
    SSL *ssl;
    BIO *cbio;
    cbio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(cbio, & ssl);
    SSL_set_mode(ssl,SSL_MODE_AUTO_RETRY);
    BIO_set_conn_hostname(cbio, string(server+":"+to_string(port)).c_str());
    if (BIO_do_connect(cbio) <= 0)
    {
        cerr<<"Can not connect to server\n";
        exit(3);
    }
    if(SSL_get_verify_result(ssl)!= X509_V_OK)
    {
        cerr<<"Invalid certificate\n";
        exit(3);
    }
    return cbio;
}

int stat(BIO *cbio)
{
    string status = send_to_server(cbio,"STAT\r\n");
    if(!ok(status)){
        cerr<<"Something went wrong with STAT\n";
        exit(5);
    }
    int pos = status.find(" ", 4);
    string number = status.substr(4, pos);
    return stoi(number);
}

string retr(BIO *cbio, int a)
{
    string retrieve = send_to_server(cbio, (char*)("RETR " + to_string(a) + "\r\n").c_str(), true);
    if (!ok(retrieve)){
        cerr<<"Something went wrong with RETR\n";
        exit(5);
    }
    return retrieve;
}

string download_all_messages(string out_dir, BIO *cbio)
{
    string path;
    string filename;
    char *tmp;
    tmp = new char[out_dir.length() + 1];
    strcpy(tmp, out_dir.c_str());
    DIR *dir;
    dir = opendir(tmp);
    if (dir)
    {
        closedir(dir);
    }
    else if(ENOENT == errno){
        system(string("mkdir -p " + out_dir).c_str());
    }
    if (out_dir[out_dir.length()-1] == "/"[0]){
        out_dir = out_dir.erase(out_dir.length()-1, 1);
    }

    int number = stat(cbio);
    for (int a = 1; a <= number; a++)
    {
        string message = retr(cbio, a);
        path = out_dir;
        //vytvor.nazo.subor. pripojim k path a potom otvorim a zapisujem
        filename = string("mail_").append(to_string(a)).append(".txt");
        path.append("/").append(filename);
        ofstream myFile(out_dir + "/" + filename);
        message = message.erase(0, message.find_first_of("\r\n") + 2);
        size_t pos = message.find("\r\n.\r\n");
        message = message.erase(pos, pos + 3);
        myFile << message;

    }
    cout << "SPRAV STIAHNUTYCH: " << number << '\n';
}

string delete_all_messages(BIO *cbio)
{
    int number = stat(cbio);
    for (int a = 1; a <= number; a++)
    {
        string deleting = send_to_server(cbio,(char*)("DELE "+ to_string(a)+"\r\n").c_str());
    }
    cout<<"ZMAZANYCH SPRAV: "<<number <<'\n';
}

bool quit(BIO *cbio)
{
    string quit = send_to_server(cbio,"QUIT\r\n");
}

bool login(string username, string password,BIO *cbio)
{
    string send_user = "USER ";
    send_user += username+"\r\n";
    string login_user = send_to_server(cbio,(char*)send_user.c_str());
    if (ok(login_user))
    {
        string send_password = "PASS ";
        send_password += password+"\r\n";
        string login_pass = send_to_server(cbio,(char*)send_password.c_str());
        if (ok(login_pass))
        {
            return true;
        }
        cerr<<"Wrong username or password\n";
        exit(4);
    }
    return false;
}

/*
 * Inspired from :" https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c ",
 * but improved by me
 */
vector<string> split (string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    while ((pos_end = s.find (delimiter,pos_start)) != string::npos)
    {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start, s.length() - pos_start));
    return res;
}

string delete_new_messages(BIO* cbio, string out_dir)
{

    DIR *dir;
    dir = opendir(out_dir.c_str());
    struct dirent *ent;
    int max = 0;
    // Reading whole directory
    while ((ent = readdir (dir)) != NULL)
    {
        // Find all files, whose name is in the format "mail_(number).txt"
        regex rx("mail_[1-9]*.txt");
        // When there is a match
        if(regex_search(ent->d_name,rx))
        {
            // Split by the character "_" and the result split by the "." and get to a number
            string mails = split(split(ent->d_name, "_")[1],".")[0];
            int numbers = stoi(mails);
            // Find max value of number of mails already downloaded
            if (numbers > max)
            {
                max = numbers;
            }
        }
    }
    // Find out a number od all mails
    int number = stat(cbio);
    // Count of new mails
    int new_mail = number - max;
    for (int i = max+1; i<= number; i++){
        string deleting = send_to_server(cbio,(char*)("DELE "+ to_string(i)+"\r\n").c_str());
    }
    cout<<"ZMAZANYCH NOVYCH SPRAV: "<<new_mail<<'\n';
}

string download_new_messages(string out_dir, BIO *cbio)
{
    string path;
    string filename;
    char *tmp;
    tmp = new char[out_dir.length()+1];
    strcpy(tmp,out_dir.c_str());
    DIR *dir;
    dir = opendir(out_dir.c_str());
    struct dirent *ent;
    int max = 0;
    // Reading whole directory
    while ((ent = readdir (dir)) != NULL)
    {
        // Find all files, whose name is in the format "mail_(number).txt"
        regex rx("mail_[1-9]*.txt");
        // When there is a match
        if(regex_search(ent->d_name,rx))
        {
            // Split by the character "_" and the result split by the "." and get to a number
            string mails = split(split(ent->d_name, "_")[1],".")[0];
            int numbers = stoi(mails);
            // Find max value of number of mails already downloaded
            if (numbers > max)
            {
                max = numbers;
            }
        }
    }
    // Find out a number od all mails
    int number = stat(cbio);
    // Count of new mails
    int new_mail = number - max;
    for (int i = max+1; i<= number; i++)
    {
        string message = retr(cbio, i);
        path = out_dir;
        filename = string("mail_").append(to_string(i)).append(".txt");
        path.append("/").append(filename);
        ofstream myFile(filename);
        message = message.erase(0, message.find_first_of("\r\n") + 2);
        size_t pos = message.find("\r\n.\r\n");
        message = message.erase(pos, pos + 3);
        myFile << message;
    }
    cout << "STIAHNUTYCH NOVYCH SPRAV: " << new_mail << '\n';

}

bool load_credentials(credentials* credentials, string auth)
{
    string buff;
    // Reading auth_file and load credentials
    ifstream auth_file(auth);
    if (auth_file.fail()){
        cerr<<"auth_file not found\n";
        exit(6);
    }
    while (getline (auth_file, buff))
    {
        // Find the character "=" and split
        vector<string> user_pass = split(buff, " = ");
        // Checks if credentials are ok after split
        if (user_pass.size() != 2){
            continue;
        }
        // If first part of division is username and password
        if (user_pass[0] == "username")
        {
            // Load credentials
            credentials->username = new string(user_pass[1]);
        }

        if (user_pass[0] == "password")
        {
            // Load credentials
            credentials->password = new string(user_pass[1]);
        }

    }
    // Checks if content of auth_file is correct
    if ((credentials->username == NULL) || (credentials->password == NULL)){
        cerr<<"Wrong content of auth_file";
        exit(2);
    }
    return true;
}