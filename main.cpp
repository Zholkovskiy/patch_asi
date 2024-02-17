#include <iostream>
#include <fstream>
#include <vector>
#include <locale>

using namespace std;

bool patchASI(const string& asiPath, const string& patchValue)
{
    // ������������� ���� ��� ������ ��������� �� ������� �����
    setlocale(LC_ALL, "Russian");

    // ������ ���� ASI � ������
    vector<char> asiData;
    {
        // ��������� ���� ASI ��� ������ � �������� ������
        fstream asiFile(asiPath, ios::in | ios::binary);
        if (!asiFile.is_open())
        {
            cout << "������ �������� ����� ASI: " << asiPath << endl;
            return false;
        }

        // �������� ������ �����
        asiFile.seekg(0, ios::end);
        size_t fileSize = asiFile.tellg();
        asiFile.seekg(0, ios::beg);

        // �������� ������ ������� �� ������� �����
        asiData.resize(fileSize);

        // ������ ������ ����� � ������
        asiFile.read(&asiData[0], fileSize);

        // ��������� ����
        asiFile.close();
    }

    // ���� ������� ������ � ������ ASI
    size_t targetPos = asiData.size();
    for (size_t i = 0; i < asiData.size(); i++)
    {
        if (asiData[i] == '1' && asiData[i + 1] == '2' && asiData[i + 2] == '7' && asiData[i + 3] == '.' && asiData[i + 4] == '0' && asiData[i + 5] == '.' && asiData[i + 6] == '0' && asiData[i + 7] == '.')
        {
            targetPos = i;
            break;
        }
    }

    if (targetPos == asiData.size())
    {
        cout << "������� ������ �� ������� � ����� ASI." << endl;
        return false;
    }

    // ����������� �������� ����� � ������ ������
    vector<char> patchData;
    for (char c : patchValue)
    {
        patchData.push_back(c);
    }

    // ���������, ��������� �� ����� �������� ����� � ������ ������� ������
    if (patchData.size() != 8)
    {
        cout << "������: ����� �������� ����� ������ ���� 8 ��������." << endl;
        return false;
    }

    // ��������� ���� � ������ ASI
    for (size_t i = 0; i < patchData.size(); i++)
    {
        asiData[targetPos + i] = patchData[i];
    }

    // ���������� ������������ ������ ASI ������� � ����
    {
        // ��������� ���� ASI ��� ������ � �������� ������
        fstream asiFile(asiPath, ios::out | ios::binary);
        if (!asiFile.is_open())
        {
            cout << "������ �������� ����� ASI ��� ������: " << asiPath << endl;
            return false;
        }

        // ���������� ������������ ������ � ����
        asiFile.write(&asiData[0], asiData.size());

        // ��������� ����
        asiFile.close();
    }

    cout << "���� ASI ��������� �������." << endl;
    return true;
}

int main()
{
    // ������������� ���� ��� ������ ��������� �� ������� �����
    setlocale(LC_ALL, "Russian");

    // ������������� ���� � ����� ASI
    string asiPath = "localip.asi";

    // ������������� �������� �����
    string patchValue = "141.95.190.144";

    // ��������� ����
    if (patchASI(asiPath, patchValue))
    {
        cout << "���� ASI ��������� �������." << endl;
    }
    else
    {
        cout << "������ ������������� ����� ASI." << endl;
    }

    return 0;
}
