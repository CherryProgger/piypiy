#include <iostream>
#include <fstream>
using namespace std;

bool is_empt(std::ifstream& pFile){
    return pFile.peek() == std::ifstream::traits_type::eof();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"

void error(int code){
    switch (code){
        case 0:
            cout << "incorrect information\n";
            break;
        case 1:
            cout << "username is already taken\n";
            break;
        case 2:
            cout << "username is not existing\n";
            break;
        case 3:
            cout << "incorrect password\n";
            break;
    }
}

#pragma clang diagnostic pop
bool check_log(const string& login){
    ifstream log_file("logins.txt");
    string log;
    char line_skip[1024];
    bool ret = true;
    if(!log_file){
        return ret;
    }
    while(!log_file.eof()){
        log_file >> log;
        log_file.getline(line_skip, 1024);
        if(log == login){
            ret = false;
            break;
        }
    }
    return ret;
}

int check_pswd(const string& login, const string& password){
    ifstream log_file("logins.txt");
    string log, pswd;
    int id, ret = 0;
    while(!log_file.eof()){
        log_file >> log;
        log_file >> pswd;
        log_file >> id;
        if(log == login && pswd == password){
            ret = id;
            break;
        }
    }
    log_file.close();
    return ret;
}

int get_reg_id(){
    ifstream log_file("logins.txt");
    char *str = new char [1024];
    int count=0;
    while (!log_file.eof()){
        log_file.getline(str, 1024, '\n');
        count++;
    }
    log_file.close();
    delete[] str;
    return count;
}

int get_usr_id(const string& usr){
    ifstream log_file("logins.txt");
    string log, pswd;
    int id, ret = 0;
    while(!log_file.eof()){
        log_file >> log;
        log_file >> pswd;
        log_file >> id;
        if(log == usr){
            ret = id;
            break;
        }
    }
    log_file.close();
    return ret;
}

void regis(){
    string log, pswd; int id;
    ofstream log_file("logins.txt", ios_base::app);
    cout << "write your login \npassword must contain only numbers and letters\n";
    cin >> log;
    if(check_log(log)){
        cout << "write your password\n";
        cin >> pswd;
        id = get_reg_id();
        log_file << log << " " << pswd << " " << id << "\n";
        log_file.close();
    } else {
        error(1);
    }
}

int login(){
    string log, pswd; int id;
    cout << "write your login\n";
    cin >> log;
    if(!check_log(log)){
        cout << "write your password\n";
        cin >> pswd;
        id = check_pswd(log, pswd);
        if(id > 0){
            return id;
        } else {
            error(3);
        }
    } else {
        error(2);
    }
    return -1;
}

void addfriend(int id){
    string file_path, friendname;
    file_path = to_string(id) + "_friends.txt";
    ofstream friendlist(file_path, ios_base::app);
    cout << "write your friend's login \n";
    cin >> friendname;
    if(!check_log(friendname)){
        cout << "Your friend is added\n";
        friendlist << friendname << "\n";
        friendlist.close();
    } else {
        error(2);
    }
}

void friends(int id){
    char user_input[500];
    [[maybe_unused]] char *str = new char [1024];
    string file_path;
    file_path = to_string(id) + "_friends.txt";
    ifstream friendlist(file_path);
    if(!friendlist or is_empt(friendlist)){
        cout << "You dont have any friends \n";
    } else {
        cout << "List of friends\n";
        cout << ifstream(file_path).rdbuf();
        friendlist.close();
    }
    cout << "What do you want to do?\n1 - add a friend\n2 - delete friend\n3 - exit\n";
    cin >> user_input;
    switch ((int)user_input[0]-48){
        case 1:
            addfriend(id);
            break;
    }
}

void userlist(){
    string temp;
    cout << "Users online:\n";
    ifstream usrlist("logins.txt", ios_base::in);
    char *str = new char [1024];
    usrlist >> temp;
    while (!usrlist.eof()){
        cout << temp << "\n";
        usrlist.getline(str, 1024, '\n');

        usrlist >> temp;
    }
    usrlist.close();
}

void send_message(const string& file_path, const string& file_path2){
    ofstream chatfile(file_path, ios_base::app);
    ofstream chatfile2(file_path2, ios_base::app);
    string mess;
    getline(cin, mess);
    chatfile << "I: " << mess << "\n\n";
    chatfile2 << "You: " << mess << "\n\n";
}

void messager(int id){
    string target, file_path, file_path2;
    int target_id;
    cout << "Enter a login\n ";
    cin >> target; cin.ignore();
    if(!check_log(target)){
        target_id = get_usr_id(target);
        file_path = to_string(id) + "_" + to_string(target_id) + "_chat.txt";
        file_path2 = to_string(target_id) + "_" + to_string(id) + "_chat.txt";
        ifstream chatfile(file_path);
        if(!chatfile or is_empt(chatfile)){
            cout << "Dialog is not exist!\n";
        } else {
            cout << "Your dialog:\n";
            cout << ifstream(file_path).rdbuf() << "\n";
        }
        cout << "Write a message and press enter.\n";
        chatfile.close();
        send_message(file_path, file_path2);
    } else {
        error(2);
    }
}

int main(){
    char user_input[500];
    int user_id = -1;
    while(user_id == -1){
        cout << "What do you want to do?\n1 - registration\n2 - enter the account\n3 - exit\n";
        cin >> user_input;
        switch ((int)user_input[0]-48){
            case 1:
                regis();
                break;
            case 2:
                user_id = login();
                break;
            case 3:
                return 0;
            default:
                error(0);
                break;
        }
    }
    while (true){
        cout << "What do you want to do?\n1 - friends\n2 - messages\n3 - users\n4 - exit\n";
        cin >> user_input;
        switch ((int)user_input[0]-48){
            case 1:
                friends(user_id);
                break;
            case 2:
                messager(user_id);
                break;
            case 3:
                userlist();
                break;
            case 4:
                return 0;
            default:
                error(0);
                break;
        }
    }
}
#include <iostream>
#include <fstream>
using namespace std;

bool is_empt(std::ifstream& pFile){
    return pFile.peek() == std::ifstream::traits_type::eof();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"

void error(int code){
    switch (code){
        case 0:
            cout << "incorrect information\n";
            break;
        case 1:
            cout << "username is already taken\n";
            break;
        case 2:
            cout << "username is not existing\n";
            break;
        case 3:
            cout << "incorrect password\n";
            break;
    }
}

#pragma clang diagnostic pop
bool check_log(const string& login){
    ifstream log_file("logins.txt");
    string log;
    char line_skip[1024];
    bool ret = true;
    if(!log_file){
        return ret;
    }
    while(!log_file.eof()){
        log_file >> log;
        log_file.getline(line_skip, 1024);
        if(log == login){
            ret = false;
            break;
        }
    }
    return ret;
}

int check_pswd(const string& login, const string& password){
    ifstream log_file("logins.txt");
    string log, pswd;
    int id, ret = 0;
    while(!log_file.eof()){
        log_file >> log;
        log_file >> pswd;
        log_file >> id;
        if(log == login && pswd == password){
            ret = id;
            break;
        }
    }
    log_file.close();
    return ret;
}

int get_reg_id(){
    ifstream log_file("logins.txt");
    char *str = new char [1024];
    int count=0;
    while (!log_file.eof()){
        log_file.getline(str, 1024, '\n');
        count++;
    }
    log_file.close();
    delete[] str;
    return count;
}

int get_usr_id(const string& usr){
    ifstream log_file("logins.txt");
    string log, pswd;
    int id, ret = 0;
    while(!log_file.eof()){
        log_file >> log;
        log_file >> pswd;
        log_file >> id;
        if(log == usr){
            ret = id;
            break;
        }
    }
    log_file.close();
    return ret;
}

void regis(){
    string log, pswd; int id;
    ofstream log_file("logins.txt", ios_base::app);
    cout << "write your login \npassword must contain only numbers and letters\n";
    cin >> log;
    if(check_log(log)){
        cout << "write your password\n";
        cin >> pswd;
        id = get_reg_id();
        log_file << log << " " << pswd << " " << id << "\n";
        log_file.close();
    } else {
        error(1);
    }
}

int login(){
    string log, pswd; int id;
    cout << "write your login\n";
    cin >> log;
    if(!check_log(log)){
        cout << "write your password\n";
        cin >> pswd;
        id = check_pswd(log, pswd);
        if(id > 0){
            return id;
        } else {
            error(3);
        }
    } else {
        error(2);
    }
    return -1;
}

void addfriend(int id){
    string file_path, friendname;
    file_path = to_string(id) + "_friends.txt";
    ofstream friendlist(file_path, ios_base::app);
    cout << "write your friend's login \n";
    cin >> friendname;
    if(!check_log(friendname)){
        cout << "Your friend is added\n";
        friendlist << friendname << "\n";
        friendlist.close();
    } else {
        error(2);
    }
}

void friends(int id){
    char user_input[500];
    [[maybe_unused]] char *str = new char [1024];
    string file_path;
    file_path = to_string(id) + "_friends.txt";
    ifstream friendlist(file_path);
    if(!friendlist or is_empt(friendlist)){
        cout << "You dont have any friends \n";
    } else {
        cout << "List of friends\n";
        cout << ifstream(file_path).rdbuf();
        friendlist.close();
    }
    cout << "What do you want to do?\n1 - add a friend\n2 - delete friend\n3 - exit\n";
    cin >> user_input;
    switch ((int)user_input[0]-48){
        case 1:
            addfriend(id);
            break;
    }
}

void userlist(){
    string temp;
    cout << "Users online:\n";
    ifstream usrlist("logins.txt", ios_base::in);
    char *str = new char [1024];
    usrlist >> temp;
    while (!usrlist.eof()){
        cout << temp << "\n";
        usrlist.getline(str, 1024, '\n');

        usrlist >> temp;
    }
    usrlist.close();
}

void send_message(const string& file_path, const string& file_path2){
    ofstream chatfile(file_path, ios_base::app);
    ofstream chatfile2(file_path2, ios_base::app);
    string mess;
    getline(cin, mess);
    chatfile << "I: " << mess << "\n\n";
    chatfile2 << "You: " << mess << "\n\n";
}

void messager(int id){
    string target, file_path, file_path2;
    int target_id;
    cout << "Enter a login\n ";
    cin >> target; cin.ignore();
    if(!check_log(target)){
        target_id = get_usr_id(target);
        file_path = to_string(id) + "_" + to_string(target_id) + "_chat.txt";
        file_path2 = to_string(target_id) + "_" + to_string(id) + "_chat.txt";
        ifstream chatfile(file_path);
        if(!chatfile or is_empt(chatfile)){
            cout << "Dialog is not exist!\n";
        } else {
            cout << "Your dialog:\n";
            cout << ifstream(file_path).rdbuf() << "\n";
        }
        cout << "Write a message and press enter.\n";
        chatfile.close();
        send_message(file_path, file_path2);
    } else {
        error(2);
    }
}

int main(){
    char user_input[500];
    int user_id = -1;
    while(user_id == -1){
        cout << "What do you want to do?\n1 - registration\n2 - enter the account\n3 - exit\n";
        cin >> user_input;
        switch ((int)user_input[0]-48){
            case 1:
                regis();
                break;
            case 2:
                user_id = login();
                break;
            case 3:
                return 0;
            default:
                error(0);
                break;
        }
    }
    while (true){
        cout << "What do you want to do?\n1 - friends\n2 - messages\n3 - users\n4 - exit\n";
        cin >> user_input;
        switch ((int)user_input[0]-48){
            case 1:
                friends(user_id);
                break;
            case 2:
                messager(user_id);
                break;
            case 3:
                userlist();
                break;
            case 4:
                return 0;
            default:
                error(0);
                break;
        }
    }
}