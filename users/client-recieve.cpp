#include <iostream>
#include <sys/types.h>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <string>

using namespace std;
string delim = " ";

void reg(string user_id, string user_pas){
    ofstream fr;
    fr.open("user_list.txt", ofstream::app);
    fr << user_id + ":" + user_pas << "\n";
    fr.close();
    fr.open("users/" + user_id + ".txt");
    fr.close();
}

bool check_uzers(string id){
    string path = "user_list.txt";
    string str;
    size_t pos = 0;
    string us_n, token;
    ifstream cu;
    cu.open(path);
    if(cu.is_open()){
        while (!cu.eof())
        {
            getline(cu, str);
            pos = str.find(delim);
            us_n = str.substr(0, pos);
            if(us_n == id){
                return 1;
            }
        }
    }else{
        cout << "smthng wrong" << endl;
    }
    cu.close();
    return 0;
}

void add_friend(string user1, string user2){
    string str;
    string path = user1 + "_fr_list.txt";
    ofstream us2;
    us2.open("users/friends/" + path, ofstream::app);
    us2 << user2 << endl;
    us2.close();
}

void write( string user1, string user2, string mesg){
    string path = "users/messages/" + user1 + "To" + user2 + ".txt";
    ifstream wrmt;
    wrmt.open(path);
    if(wrmt.is_open()){
        wrmt.close();
        ofstream wrmf;
        wrmf.open(path, ofstream::app);
        wrmf << user1+": " + mesg << "\n";
        wrmf.close();
    }else{
        wrmt.close();
        path = "users/messages/" + user2 + "To" + user1 + ".txt";
        ofstream wrmf;
        wrmf.open(path, ofstream::app);
        wrmf << user1+": " + mesg << "\n";
        wrmf.close();
    }
}

// обновление буффера
void buf_date(char* buf){
    for(int i=0; i < 1024; i++){
        buf[i] = '#';
    }
}

void set_status(string user, string tt){
    ofstream pr;
    string path = "users/" + user + ".txt";
    pr.open(path);
    pr << "status: " + tt << endl;
    pr.close();
}


int main(){
    setlocale(LC_ALL, 0);
    struct sockaddr_in peer;
    string inform[5];
    int s;
    int rc;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    buf_date(buffer);

    peer.sin_family = AF_INET;
    peer.sin_port = htons(10050);
    peer.sin_addr.s_addr = inet_addr("172.20.10.7");


    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s<0){
        perror("ошибка вызова socket");
        exit(1);
    }
    cout << "\n=> Socket client has been created..." << endl;

    rc = connect(s, ( struct sockaddr * )&peer, sizeof(peer));

    if(rc){
        perror("ошибка вызова connect");
        exit(1);
    }

    do {
        recv(s, buffer, bufsize, 0);
        int run = buffer[0];

        int count = 0;
        for(int i = 0; i < bufsize; i++){
            if(buffer[i] == '#'){
                break;
            } else if(buffer[i] == ' ' and count < 3){
                count++;
            } else{
                inform[count] += buffer[i];
            }
        }

        cout << run << endl;
        switch (run){
            case 49:
                buf_date(buffer);
                reg(inform[1], inform[2]);
                break;
            case 50:
                buf_date(buffer);
                add_friend(inform[1], inform[2]);
                break;
            case 51:
                buf_date(buffer);
                write(inform[1], inform[2], inform[3]);

            case 52:
                buf_date(buffer);
                set_status(inform[1], inform[3]);
            default:
                break;
        }

        for(int i = 0; i < 5; i++){
            inform[i] = "";
        }

    }while(!isExit);
    return 0;
}