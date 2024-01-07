#include <iostream>
#include <windows.h>
#include <vector>


#pragma pack(push,1)
using namespace std;

// Cтруктура Volume Header
struct Volume_Header
{
    WORD header_crc;
    BYTE header_type;
    WORD header_flags;
    WORD header_size;
};

// Cтруктура File_head для извлечения имён файлов

struct File_head
{
    DWORD pack_size;
    DWORD unpsize;
    BYTE HostOS;
    DWORD fileCRC;
    DWORD FileTime;
    BYTE unpver;
    BYTE method;
    WORD namesize;
    DWORD fileattr;
};

#pragma pack(pop)
// Прототип функции вывода имён файлов

// Сигнатуры

const BYTE signatures[] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00};
void PrintHeaderContent(const vector <char> &, DWORD filesize);
void Print_vector(const vector <char> & printing_vector, int vectorsize);


int main() {
    setlocale(LC_ALL, "Russian");
    HANDLE file_to_open_Handler = CreateFile("C:\\Users\\Андрей\\Downloads\\HomeW\\Example (3).rar",
                                              GENERIC_READ,
                                              0,
                                              NULL,
                                              OPEN_EXISTING,
                                              FILE_ATTRIBUTE_NORMAL,
                                              NULL);

    if (file_to_open_Handler == INVALID_HANDLE_VALUE)
    {
        cout << "Ошибка открытия файла" << endl;
        return 1;
    }
    // Получаем размер файла
    DWORD fileSize = GetFileSize(file_to_open_Handler, NULL);

    vector<char> buffer(fileSize);

    DWORD bytesRead;

    ReadFile(file_to_open_Handler, &buffer[1], fileSize, &bytesRead, NULL);

    PrintHeaderContent(buffer, fileSize);

    CloseHandle(file_to_open_Handler);



    return 0;
}

void Print_vector(const vector <char> & printing_vector)
{
    for (char c : printing_vector)
    {
        wcout << c;
    }
}

void DebugVar(auto variable)
{
    cout << "\n----->" << variable << endl;
}
void PrintHeaderContent(const vector <char> & filebufer, DWORD filesize)
{

    // Создаём экземпляр структуры Volume Header
    cout << "\nFile size : " <<  filesize << endl ;
    Volume_Header header_main;
    // Создаём экземпляр структуры для header файла
    File_head file_header;
    int index_to_cut = sizeof(signatures); // 7 байт для среза(структура)
    int count_of_files = 0;
    while (index_to_cut < filesize)
    {
        memcpy(&header_main, &filebufer[index_to_cut], sizeof(header_main));
        if (header_main.header_type == 0x74)
        {
            int temporary = index_to_cut;
            count_of_files++ ;
            wcout << "\n--->0х74<-----" << endl;
            index_to_cut += sizeof(header_main) ;
            memcpy(&file_header, &filebufer[index_to_cut], sizeof(file_header));
            index_to_cut += sizeof(file_header);
            char *FileName = new char[file_header.namesize];
            for (int index_to_file = 0 ; index_to_file < file_header.namesize; index_to_file++)
            {
                FileName[index_to_file] = filebufer[index_to_cut];
                index_to_cut++;
                cout << FileName[index_to_file];
            }

            cout << "\n";
            index_to_cut = temporary;
            index_to_cut += file_header.pack_size + header_main.header_size;
            delete[] FileName;
        }
        else
        {
            index_to_cut += sizeof(header_main.header_size) ;
        }
    }
    wcout << "Количество файлов в архиве :" << count_of_files;


}
