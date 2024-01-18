using namespace std;
#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include "HufTree.cpp" // дерево Хаффмана
#include "boolVec.cpp" //для работы с булевыми векторами
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
    unsigned char *encodedText = (unsigned char *)calloc(sizeof(unsigned char), bytes);
    if (!encodedText)
        return nullptr;
    for (char c : text) // Упаковываем все биты закодированного текста в один булев вектор
    {
        ShiftLeft(encodedText, bytes * 8, bit_codes[c].len);
        size_t bytesPair = (bit_codes[c].len - 1) / 8 + 1;
        for (size_t i = 0; i < bytesPair; i++)
            encodedText[bytes - i - 1] = encodedText[bytes - i - 1] | bit_codes[c].data[bytesPair - i - 1];
    }
    cout << "EncodedText: " << VecToStr(encodedText, bytes * 8) << endl;
    return encodedText;
}
// Декодирование текста с помощью дерева Хаффмана
string decode(unsigned char *encodedText, size_t bytes, unordered_map<char, boolVec> &bit_codes, size_t count)
{
    string decodedText = ""; // end
    if (encodedText)
    {
        bool flag = true;
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
                for (size_t y = 0; y <= (pair.second.len - 1) % 8; y++)
                    mask[x] = mask[x] | (1 << y);
                x++;
                for (; x < bytesPair; x++)
                    mask[x] = 255;
                for (size_t i = 0; i < bytesPair; i++)
                    mask[bytesPair - 1 - i] = encodedText[bytes - 1 - i] & mask[bytesPair - 1 - i];
                size_t j = 0;
                for (; j < bytesPair && mask[j] == pair.second.data[j]; j++)
                    ;
                free(mask);
                if (j == bytesPair)
                {
                    flag = true;
                    ShiftRight(encodedText, bytes * 8, pair.second.len);
                    decodedText = pair.first + decodedText;
                    break;
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
    cout << "count: " << count << endl;
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
        ofstream write("encodedtext.bin", ios::binary | ios::out); // Работаем с бинарным файлом работа с которым быстрее, помимо этого
        // если используем файл .txt мы не можем передать все байты unsigned char, некоторые байты невидимы!
        string text;
        while (getline(in, line))
            text += line;
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
        cout << "bytes - " << bytes << endl;
        unsigned char *encodedText = encodeText(text, bit_codes, bytes);
        cout << "count: " << count << endl;
        write << count << "\n";
        for (size_t i = 0; i < bytes; i++)
            write << encodedText[i];
        write << "\n\n";            // отступ между закодированным текстом и Деревом Хаффмана
        for (auto pair : bit_codes) // запись в файл дерева Хаффмана
        {
            write << pair.first << pair.second.len << ' ';
            for (size_t p = 0; p < (pair.second.len - 1) / 8 + 1; p++)
                write << pair.second.data[p]; // побайтово записали каждый код для каждого символа в файл
            write << endl;
            // для возможности раскодировать мы запоминаем пары вида "символ" -"код" в файл
            cout << pair.first << " " << pair.second.len << " " << VecToStr(pair.second.data, 8 * ((pair.second.len - 1) / 8 + 1)) << "\n";
        }
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
        // считываем из файла закодированный текст
        string line;
        size_t count;
        in >> count; // общее число символов, необходимо для декодера
        getline(in, line);
        while (getline(in, line))
        {
            if (line.length() == 0) // отступ между деревом и кодом
                break;
            if (bytes != 0) // передали байт == \n
            {
                bytes += 1;
                encodedText = (unsigned char *)realloc(encodedText, bytes);
                encodedText[j] = 10;
                j++;
            }
            bytes += line.length();
            encodedText = (unsigned char *)realloc(encodedText, bytes);
            for (size_t i = 0; i < line.length(); i++)
            {
                encodedText[j] = line[i];
                j++;
            }
        }
        int len;
        unsigned char *path;
        char sym;
        while (getline(in, line))
        {
            if (line.length() < 2) // обработка \n char(10)
            { 
                if (len > 8)
                {
                    if (line.length() == 1)
                    {
                        path[0] = 10; //     \nsym
                        path[1] = line[0];
                    }
                    else
                        path[1] = 10; //    sym\n
                }
                else
                    path[0] = 10; //     \n
                bit_codes[sym] = boolVec(path, len);
                continue;
            }
            sym = line[0];//считываем символ из файла
            len = 0;
            size_t k = 1;
            for (; line[k] != ' ' && line[k]; k++)
            {
                len *= 10;
                len += line[k] - 48;
            }//считываем длину кода из файла
            unsigned char *path = (unsigned char *)calloc(sizeof(unsigned char), (len - 1) / 8 + 1);
            k++;
            size_t i = k;
            for (; (i - k) < ((len - 1) / 8 + 1); i++)
                path[i - k] = line[i];
            bit_codes[sym] = boolVec(path, len);//создаём Хеш-таблицу пар символ - код
        }
        string decodedText = decode(encodedText, bytes, bit_codes, count);
        write << decodedText;
        //   записали результат в файл text.txt
        cout << "Decoded text: " << decodedText << endl; // вывели результат
        DestroyVec(bit_codes);
        in.close();
        write.close();
    }
    return 0;
}
