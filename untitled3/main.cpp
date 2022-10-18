#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

using namespace std;

struct file_data{
    char* file_name; //имя файла
    int size; //размер
    char* owner; //владелец
    string extension; //расширение
    char* permission;//права доступа
};

//метод для печати инфы о файле
void print_info(file_data statistica);

//проверка сущестует ли файл. если существует то тру возвращает иначе фалсе
bool file_exist(const std::string& name);

//владельца файла узнаем
char* get_owner(char* fileName);

//размер файла узнаем
int file_size(char* file_name);

//узнаем расширение файла
string get_file_extension(const string& file_name);

//получаем права доступа к файлу
char* get_permissions(char *file);

int main() {
    bool flag = true;
    while (flag) {
        char file_name[255];
        cout << "enter file name" << endl;
        cin >> file_name;
        if (strcmp(file_name,"exit") == 0){
            flag = false;
        } else if (file_exist(file_name)){
            file_data s;
            //запихиваем имя файла
            s.file_name = file_name;
            //запихиваем размер файла в структуру (в байтах)
            s.size = file_size(file_name);
            //запихиваем владельца файла в структуру
            s.owner = get_owner(file_name);
            //запихиваем расширение файла
            s.extension = get_file_extension(file_name);
            //узнаем права доустпа для админа
            s.permission = get_permissions(file_name);
            //печатаем всю эту хуйню
            print_info(s);
        }
        else{
            cout << "file not found\n";
        }
    }
}

void print_info(file_data statistica){
    cout << "file name - " << statistica.file_name << " file size - " << statistica.size << " byte file owner - "
        << statistica.owner << " file extension - " << statistica.extension << " permissions - "
        << statistica.permission << endl;
}

//использую стандарт посикс, так быстрее просто
bool file_exist(const std::string& name){
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

int file_size(char* file_name){
    fstream file_t(file_name);
    //делаем отсутп в 0 символов с начала
    file_t.seekg(0, std::ios::end);
    //возвращаем размер файла в структуру (в байтах)
    return (int) file_t.tellg();
}

char* get_owner(char* name){
    struct stat info;
    stat(name, &info);
    struct passwd *pw = getpwuid(info.st_uid);
    //возвращем имя владельца
    return pw->pw_name;
}

string get_file_extension(const string& file_name){
    size_t pos = file_name.find_last_of ("./");
    if (pos <=0 || file_name[pos] =='/') return "";
    return file_name.substr(pos, string::npos);
}

char* get_permissions(char *file){
    struct stat st;
    char *modeval = new char[3];
    if(stat(file, &st) == 0){
        mode_t perm = st.st_mode;
        //r - read, w - write
        modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
        modeval[2] = '\0';
        return modeval;
    }
    return reinterpret_cast<char *>('-');
}