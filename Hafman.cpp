using namespace std;
#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <memory.h>
#include "HufTree.cpp"
// Написать булев вектор

struct boolVec
{
    unsigned char *data; // Закодированный Символ
    size_t len;          // его длина

    boolVec(unsigned char *data = 0, size_t len = 1)
    {
        this->data = data;
        this->len = len;
    }
};
void PrintVec(unsigned char *vector, size_t bits)
{
    if (vector)
    {
        size_t bytes = ((bits - 1) / 8) + 1; // Количество байт
        for (size_t i = 0; i < bytes; i++)
        {
            unsigned char mask = 1 << 7; // Создаем маску, начиная с самого левого бита
            // Цикл для обработки каждого бита в байте
            for (size_t j = 0; j < 8 && i * 8 + j < bits; j++)
            {
                if ((vector[i] & mask) != 0)
                    printf("1"); // Выводим "1", если бит установлен
                else
                    printf("0");  // Выводим "0", если бит не установлен
                mask = mask >> 1; // Сдвигаем маску вправо для проверки следующего бита
            }
            printf(" ");
        }
    }
    else
        printf("Can't print vector."); // Если указатель на вектор пуст, выводим сообщение об ошибке
    printf("\n");                      // Переводим строку после вывода битового вектора
}
unsigned char *CopyVector(unsigned char *vector, size_t bits)
{
    if (vector)
    {
        size_t bytes = ((bits - 1) / 8) + 1;
        unsigned char *newVector = (unsigned char *)calloc(sizeof(unsigned char), bytes);
        if (newVector)
        {
            for (size_t i = 0; i < bytes; i++)
                newVector[i] = vector[i];
            return newVector;
        }
    }
    return NULL;
}
void DestroyVec(unordered_map<char, boolVec> &bit_codes) // функция для освобождения памяти выделенной под дерево
{
    for (auto &pair : bit_codes)
    {
        free(pair.second.data);
    }
}

void ShiftRight(unsigned char *vec, size_t bits, size_t k)
{
    if (vec && k)
    {
        int bytes_to_shift = k / 8;
        int bytes = (bits + 7) / 8;

        // Сдвиг на целые байты
        if (bytes_to_shift)
        {
            for (int i = bytes - 1; i >= 0; --i)
            {
                if (i - bytes_to_shift >= 0)
                {
                    vec[i] = vec[i - bytes_to_shift];
                }
                else
                {
                    vec[i] = 0; // Обнуляем начальные байты
                }
            }
        }
        k %= 8;
        unsigned char bite1 = 0;
        unsigned char bite2 = 0;
        // unsigned char m = (1 << k) - 1;

        for (int i = 0; i < bytes; i++)
        {
            bite1 = vec[i];
            vec[i] = vec[i] >> k;
            vec[i] |= bite2 << (8 - k);
            bite2 = bite1;
        }
    }
}
void ShiftLeft(unsigned char *vec, size_t bits, size_t k)
{
    if (vec && k)
    {
        int bytes_to_shift = k / 8;
        int bytes = (bits + 7) / 8;

        // Сдвиг на целые байты
        if (bytes_to_shift)
        {
            for (int i = 0; i < bytes; ++i)
            {
                if (i + bytes_to_shift < bytes)
                {
                    vec[i] = vec[i + bytes_to_shift];
                }
                else
                {
                    vec[i] = 0; // Обнуляем конечные байты
                }
            }
        }
        k %= 8;
        unsigned char bite1 = 0;
        unsigned char bite2 = 0;
        // unsigned char m = 255;//(1 << k) - 1
        for (int i = bytes - 1; i >= 0; i--)
        {
            bite1 = vec[i];
            vec[i] = vec[i] << k; //
            vec[i] |= bite2 >> (8 - k);
            bite2 = bite1;
        }
    }
}

unsigned char *VecToStr(unsigned char *vec, size_t bits)
{
    if (vec)
    {
        size_t bytes = ((bits - 1) / 8) + 1;                                             // Количество байт
        unsigned char *str = (unsigned char *)calloc(sizeof(unsigned char), (bits + 1)); // Выделение памяти для строки
        if (str)
        {                        // Если указатель не нулевой
            size_t strIndex = 0; // Индекс для строки
            for (size_t i = 0; i < bytes; i++)
            {                                // Проход по ячейкам
                unsigned char mask = 1 << 7; // Создаем маску, начиная с самого левого бита
                for (int j = 0; j < 8 && i * 8 + j < bits; j++)
                {
                    if ((vec[i] & mask) != 0)
                        str[strIndex] = '1';
                    else
                        str[strIndex] = '0';
                    mask = mask >> 1;
                    strIndex++;
                }
                mask = 1 << 7;
            }
            str[bits] = '\0'; // Добавляем завершающий символ нуля
            return str;
        }
    }
    return NULL;
}
unsigned char *StrToVec(string &str)
{
    if (str.length() > 0)
    {
        size_t bytes = ((str.length() - 1) / 8) + 1;                                  // вычисление количества ячеек булева вектора
        unsigned char *vec = (unsigned char *)calloc(sizeof(unsigned char), (bytes)); // выделение памяти под вектор
        if (vec)
        {
            int k = 0;
            unsigned char mask = 1 << 0;
            // Создаем маску, начиная с самого левого бита

            for (int i = 0; i < bytes; i++)
            {
                for (int j = 0; j < 8 && (k < str.length()); j++)
                {
                    if (str[k] != '0')
                        vec[bytes - i - 1] = vec[bytes - i - 1] | mask;

                    mask = mask << 1; // Смещаем маску
                    k++;
                }
                mask = 1 << 0; // Создаем маску, начиная с самого левого бита; // сбрасываем маску перед переходом к следующему байту
            }
            return vec;
        }
    }
    return NULL;
}
// Генерация кодов Хаффмана
void generateCodes(Node *root, string path, unordered_map<char, boolVec> &bit_codes) // codes - это хэш-таблица в которой ключ это символ, а значение его кодировка алгоритмом Хаффмана
{
    if (root == nullptr)
        return;
    if (!root->left && !root->right)                                    // случай когда дошли до листа дерева
        bit_codes[root->data] = boolVec(StrToVec(path), path.length()); // записываем коды Хаффмана

    generateCodes(root->left, path + "0", bit_codes); // рекурсивно спускаемся по дереву от корня к листьям
    generateCodes(root->right, path + "1", bit_codes);
}

// Кодирование текста с помощью таблицы Хаффмана
unsigned char *encodeText(string &text, unordered_map<char, boolVec> &bit_codes, size_t bytes)
{
    unsigned char *encodedText;

    encodedText = (unsigned char *)calloc(sizeof(unsigned char), bytes);
    char temp;
    if (!encodedText)
        return nullptr;
    for (char c : text) // Упаковываем все биты закодированного текста в один булев вектор
    {
        ShiftLeft(encodedText, bytes * 8, bit_codes[c].len);
        temp = c;
        for (size_t i = 0; i < ((bit_codes[c].len - 1) / 8 + 1); i++)
        {
            encodedText[bytes - i - 1] = encodedText[bytes - i - 1] | bit_codes[c].data[i];
            cout << VecToStr(encodedText, bytes * 8) << endl;
        }
        // cout << "EncodedText: " << VecToStr(encodedText, bytes * 8) << endl;
    }
    cout << "EncodedText: " << VecToStr(encodedText, bytes * 8) << endl;
    return encodedText;
}
// Декодирование текста с помощью дерева Хаффмана
string decode(unsigned char *encodedText, size_t bytes, unordered_map<char, boolVec> &bit_codes, size_t count)
{
    string decodedText = ""; // end
    bool flag = true;
    if (encodedText)
    {
        for (size_t k = 0; k < count; k++)
        {
            for (const auto &pair : bit_codes) // просматриваем таблицу Хаффмана
            {
                size_t bytesPair = (pair.second.len - 1) / 8 + 1;
                unsigned char *mask = (unsigned char *)calloc(sizeof(unsigned char), bytesPair);
                if (!mask)
                {
                    cout << "ERROR MEMORY\n";
                    return "";
                }
                size_t x = 0;
                for (; x < bytesPair - 1; x++)
                    mask[x] = 1;
                for (size_t y = 0; y < (pair.second.len - x * 8); y++)
                {
                    mask[x] = mask[x] | (1 << y);
                    // cout << char (mask[x] + 48) << endl;
                }
                // cout << z << " mask - " << VecToStr(mask, bytesPair * 8) << endl;
                for (size_t i = 0; i < bytesPair; i++)
                {
                    mask[i] = encodedText[bytes - 1 - i] & mask[i];
                }
                size_t j = 0;
                for (; j < bytesPair && mask[j] == pair.second.data[j]; j++)
                    ;
                if (j == bytesPair)
                {   
                    flag = true;
                    ShiftRight(encodedText, bytes * 8, pair.second.len);
                    cout << endl
                         << VecToStr(encodedText, bytes * 8) << endl;
                    decodedText = pair.first + decodedText;
                    free(mask);
                    break;
                    // cout << char(mask[j - 1] + 48) << endl; // wrong mask
                }
            }
            if (!flag)
            {
                cout << "ERROR DECODE\n";
                return decodedText;
            }
            flag = false;
        }
    }
    return decodedText;
}

int main()
{
    unordered_map<char, boolVec> bit_codes; // таблица Хаффмана
    Node *root = 0;
    string line = "";
    bool flag = 0;
    cout << "Choose Decode or Encode (Enter 0 or 1): ";
    cin >> flag;
    if (flag)
    {
        ifstream in("text.txt");
        if (!in.is_open())
        {
            cout << "ERROR: file 'text.txt' not found create it \n";
            return 1;
        }
        ofstream write("encodedtext.bin", ios::binary | ios::out);
        string text;
        while (getline(in, line))
        {
            text += line;
        }
        root = buildHuffmanTree(text); // создаём на его основе дерево
        generateCodes(root, "", bit_codes);
        int bits = 0;
        size_t bytes = 0;
        size_t count = 0;
        for (char c : text)
        {      
            count++;
            bits += bit_codes[c].len;
        }
        bytes = (bits - 1) / 8 + 1;
        cout << "bytes - " << bytes;
        unsigned char *encodedText = encodeText(text, bit_codes, bytes);
        write << count << "\n";
        for (size_t i = 0; i < bytes; i++)
        {
            write << encodedText[i];
        }
        write << "\n\n"; // отступ между закодированным текстом и Деревом Хаффмана
        for (auto pair : bit_codes)
        {
            write << pair.first << pair.second.len << pair.second.data << "\n"; // для возможности раскодировать мы запоминаем пары вида "символ" -"код" в файл
            cout << pair.first << " " << pair.second.len << " " << VecToStr(pair.second.data, 8) << "\n";
        }
        // cout << "Encoded text: " << encodedText << endl;
        DestroyTree(root); // освобождаем память потому что выделена она динамически
        DestroyVec(bit_codes);
        free(encodedText);
        write.close();
        in.close();
    }
    else
    {
        ifstream in("encodedtext.bin", ios::binary | ios::in);
        ofstream write("text.txt");
        if (!in.is_open())
        {
            cout << "ERROR: file 'encodedtext.txt' not found create it \n";
            return 1;
        }
        unsigned char *encodedText = (unsigned char *)calloc(sizeof(unsigned char), 105);
        if (!encodedText)
        {
            cout << "ERROR MEMORY";
            return 1;
        }
        unordered_map<char, boolVec> bit_codes;
        size_t j = 0;
        size_t bytes = 0;
        string temp = "";
        // считываем из файла построчно закодированный текст
        string line;
        getline(in, line);
        size_t count = line[0] - 48;
        while (getline(in, line)) // ERROR
        {
            if (line.length() == 0)
                break;
            bytes += line.length();
            encodedText = (unsigned char *)realloc(encodedText, bytes);
            if (!encodeText)
            {
                cout << "ERROR MEMORY";
                return 1;
            }
            for (size_t i = 0; i < line.length(); i++)
            {
                encodedText[j] = line[i];
                j++;
            }
        }
        while (getline(in, line))
        {
            char sym = line[0];
            int len = line[1] - 48;
            unsigned char *path = (unsigned char *)calloc(sizeof(unsigned char), (len - 1) / 8 + 1);
            size_t i = 2;
            for (; line[i]; i++)
            {
                path[i - 2] = line[i];
            }
            bit_codes[sym] = boolVec(path, len);
            //cout << sym << " " << bit_codes[sym].len << " " << VecToStr(bit_codes[sym].data, 8) << "\n";
        }
        unsigned char *vec = VecToStr(encodedText, bytes * 8);
        cout << "EncodedText: " << vec;
        string decodedText = decode(encodedText, bytes, bit_codes, count);
        write << decodedText;
        //   записали результат в файл text.txt
        cout << "Decoded text: " << decodedText << endl; // вывели результат
        in.close();
        write.close();
    }
    return 0;
}