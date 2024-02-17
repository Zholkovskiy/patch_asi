#include <iostream>
#include <fstream>
#include <vector>
#include <locale>

using namespace std;

bool patchASI(const string& asiPath, const string& patchValue)
{
    // Устанавливаем язык для вывода сообщений на русском языке
    setlocale(LC_ALL, "Russian");

    // Читаем файл ASI в память
    vector<char> asiData;
    {
        // Открываем файл ASI для чтения в бинарном режиме
        fstream asiFile(asiPath, ios::in | ios::binary);
        if (!asiFile.is_open())
        {
            cout << "Ошибка открытия файла ASI: " << asiPath << endl;
            return false;
        }

        // Получаем размер файла
        asiFile.seekg(0, ios::end);
        size_t fileSize = asiFile.tellg();
        asiFile.seekg(0, ios::beg);

        // Изменяем размер вектора до размера файла
        asiData.resize(fileSize);

        // Читаем данные файла в вектор
        asiFile.read(&asiData[0], fileSize);

        // Закрываем файл
        asiFile.close();
    }

    // Ищем целевую строку в данных ASI
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
        cout << "Целевая строка не найдена в файле ASI." << endl;
        return false;
    }

    // Преобразуем значение патча в вектор байтов
    vector<char> patchData;
    for (char c : patchValue)
    {
        patchData.push_back(c);
    }

    // Проверяем, совпадает ли длина значения патча с длиной целевой строки
    if (patchData.size() != 8)
    {
        cout << "Ошибка: длина значения патча должна быть 8 символов." << endl;
        return false;
    }

    // Применяем патч к данным ASI
    for (size_t i = 0; i < patchData.size(); i++)
    {
        asiData[targetPos + i] = patchData[i];
    }

    // Записываем пропатченные данные ASI обратно в файл
    {
        // Открываем файл ASI для записи в бинарном режиме
        fstream asiFile(asiPath, ios::out | ios::binary);
        if (!asiFile.is_open())
        {
            cout << "Ошибка открытия файла ASI для записи: " << asiPath << endl;
            return false;
        }

        // Записываем пропатченные данные в файл
        asiFile.write(&asiData[0], asiData.size());

        // Закрываем файл
        asiFile.close();
    }

    cout << "Файл ASI пропатчен успешно." << endl;
    return true;
}

int main()
{
    // Устанавливаем язык для вывода сообщений на русском языке
    setlocale(LC_ALL, "Russian");

    // Устанавливаем путь к файлу ASI
    string asiPath = "localip.asi";

    // Устанавливаем значение патча
    string patchValue = "141.95.190.144";

    // Применяем патч
    if (patchASI(asiPath, patchValue))
    {
        cout << "Файл ASI пропатчен успешно." << endl;
    }
    else
    {
        cout << "Ошибка пропатчивания файла ASI." << endl;
    }

    return 0;
}
