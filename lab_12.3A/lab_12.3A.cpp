#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstring>
#include <windows.h>

using namespace std;

enum Specialty
{
    KN,
    IK,
    PZ,
    VP,
    SA
};

string strSpecialty[] = { "��", "��", "��", "��", "��" };

struct Student
{
    int student_id;             
    char surname[64];           
    Specialty specialty;        
    short unsigned course;      
    short unsigned grade_math;  
    short unsigned grade_physics; 
    short unsigned grade_profile; 
};

struct Index
{
    int position;               
    char surname[64];           
};

void Create(char* file_name);
void Print(char* file_name);
void SortBin(char* file_name);
void CreateIndex(char* file_name, char* index_file_name);
bool BinarySearch(char* index_file_name, const char* surname);

Specialty getSpecialty();

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char file_name[81], index_file_name[81];
    int menu;

    cout << "������ ����� �����: ";
    cin.getline(file_name, sizeof(file_name));
    cout << "������ ����� ���������� �����: ";
    cin.getline(index_file_name, sizeof(index_file_name));

    do
    {
        cout << "[1] - �������� ������ ��������" << endl;
        cout << "[2] - ������� ���������� ��� ��������" << endl;
        cout << "[3] - Գ����� ������������ ����" << endl;
        cout << "[4] - �������� ��������� ����" << endl;
        cout << "[5] - ��������� �������� ��������" << endl;
        cout << "[6] - ��������� ��������" << endl;

        cout << "����: ";
        cin >> menu;
        switch (menu)
        {
        case 1:
            Create(file_name);
            break;
        case 2:
            Print(file_name);
            break;
        case 3:
            SortBin(file_name);
            break;
        case 4:
            CreateIndex(file_name, index_file_name);
            break;
        case 5: {
            char surname[64];
            cout << "������ ������� ��� ������: ";
            cin >> surname;

            if (BinarySearch(index_file_name, surname))
                cout << "�������� ��������." << endl;
            else
                cout << "�������� �� ��������." << endl;
            break;
        }
        }
    } while (menu != 6);
}

void Create(char* file_name)
{
    ofstream f(file_name, ios::binary);
    if (!f)
    {
        cerr << "������� �������� ����� '" << file_name << "'" << endl;
        exit(1);
    }

    Student s;
    char ch;
    int i = 0;
    do
    {
        cout << endl << "������� � " << i + 1 << ":" << endl;
        s.student_id = i + 1;

        cin.sync();
        cout << "�������: ";
        cin >> s.surname;

        cout << "����: ";
        cin >> s.course;

        cout << "������������ (0 - ��, 1 - ��, 2 - ��, 3 - ��, 4 - ��): ";
        s.specialty = getSpecialty();

        cout << "������ � ����������: ";
        cin >> s.grade_math;

        cout << "������ � ������: ";
        cin >> s.grade_physics;

        cout << "������ � ����������� ��������: ";
        cin >> s.grade_profile;

        if (!f.write((char*)&s, sizeof(Student)))
        {
            cerr << "������� ������" << endl;
            exit(1);
        }

        cout << "������ �� ������ ��������? (Y/N): ";
        cin >> ch;
        i++;
    } while (ch == 'Y' || ch == 'y');
}

void Print(char* file_name)
{
    ifstream f(file_name, ios::binary);
    if (!f)
    {
        cerr << "������� �������� �����" << endl;
        exit(1);
    }

    Student s;
    int i = 0;
    cout << "=====================================================================================" << endl;
    cout << "| # |  ID  |   �������   | ���� |   ������������   | ���������� | Գ���� | ������� |" << endl;
    cout << "-------------------------------------------------------------------------------------" << endl;
    while (f.read((char*)&s, sizeof(Student)))
    {
        cout << "|" << setw(2) << right << i + 1 << " "
            << "|" << setw(5) << right << s.student_id << " "
            << "|" << setw(14) << left << s.surname
            << "|" << setw(4) << right << s.course << "  "
            << "|" << setw(15) << right << strSpecialty[s.specialty]
            << "    |" << setw(10) << right << s.grade_math
            << "  |" << setw(6) << right << s.grade_physics
            << "  |" << setw(7) << right << s.grade_profile << "  |" << endl;
        i++;
    }
    cout << "=====================================================================================" << endl;
}

void SortBin(char* file_name)
{
    fstream f(file_name, ios::binary | ios::in | ios::out);
    if (!f)
    {
        cerr << "������� �������� �����" << endl;
        exit(1);
    }

    f.seekg(0, ios::end);
    int size = f.tellg() / sizeof(Student);
    f.seekg(0, ios::beg);

    for (int i0 = 1; i0 < size; i0++)
        for (int i1 = 0; i1 < size - i0; i1++)
        {
            Student s1, s2;

            f.seekg(i1 * sizeof(Student));
            f.read((char*)&s1, sizeof(Student));

            f.seekg((i1 + 1) * sizeof(Student));
            f.read((char*)&s2, sizeof(Student));

            if (strcmp(s1.surname, s2.surname) > 0)
            {
                f.seekp(i1 * sizeof(Student));
                f.write((char*)&s2, sizeof(Student));

                f.seekp((i1 + 1) * sizeof(Student));
                f.write((char*)&s1, sizeof(Student));
            }
        }
}

void CreateIndex(char* file_name, char* index_file_name)
{
    ifstream f(file_name, ios::binary);
    if (!f)
    {
        cerr << "������� �������� �����" << endl;
        exit(1);
    }

    ofstream index_file(index_file_name, ios::binary);
    if (!index_file)
    {
        cerr << "������� �������� ���������� �����" << endl;
        exit(1);
    }

    Student s;
    int position = 0;
    while (f.read((char*)&s, sizeof(Student)))
    {
        Index index;
        index.position = position;
        strcpy(index.surname, s.surname);
        index_file.write((char*)&index, sizeof(Index));
        position++;
    }
    f.close();
    index_file.close();
}

bool BinarySearch(char* index_file_name, const char* surname)
{
    ifstream index_file(index_file_name, ios::binary);
    if (!index_file)
    {
        cerr << "������� �������� ���������� �����" << endl;
        return false;
    }

    index_file.seekg(0, ios::end);
    int size = index_file.tellg() / sizeof(Index);
    index_file.seekg(0, ios::beg);

    int left = 0, right = size - 1;
    while (left <= right)
    {
        int mid = (left + right) / 2;

        Index index;
        index_file.seekg(mid * sizeof(Index));
        index_file.read((char*)&index, sizeof(Index));

        int cmp = strcmp(index.surname, surname);
        if (cmp == 0)
            return true;
        else if (cmp < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return false;
}

Specialty getSpecialty()
{
    int specialty;
    cin >> specialty;
    while (specialty < 0 || specialty > 4)
    {
        cout << "����������� ��� ������������. ��������� �� ���: ";
        cin >> specialty;
    }
    return (Specialty)specialty;
}
