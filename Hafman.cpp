using namespace std;
#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <memory.h>
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
        unsigned char *newVector = (unsigned char *)malloc(sizeof(unsigned char) * bytes);
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
    unsigned char bite1 = 0;
    unsigned char bite2 = 0;
    int bytes = ((bits - 1) / 8) + 1;
    int z = bits % 8;
    unsigned char m = (1 << k) - 1;

    for (int i = 0; i < bytes; i++)
    {
        bite1 = vec[i] & m;
        vec[i] = vec[i] >> k;
        vec[i] |= bite2 << (8 - k);
        bite2 = bite1;
    }
}
void ShiftLeft(unsigned char *vec, size_t bits, size_t k)
{
    unsigned char bite1 = 0;
    unsigned char bite2 = 0;
    int bytes = ((bits - 1) / 8) + 1;
    int z = bits % 8;
    unsigned char m = (1 << k) - 1;
    m = m << (8 - k);

    for (int i = bytes - 1; i >= 0; i--)
    {
        bite1 = vec[i] & m;
        vec[i] = vec[i] << k;
        vec[i] |= bite2 >> (8 - k);
        bite2 = bite1;
    }
}

// Структура узла дерева Хаффмана
struct Node
{
    char data;   // Символ
    int freq;    // Частота символа
    Node *left;  // левый потомокd
    Node *right; // правый потомок

    Node(char data, int freq)
    {
        this->data = data;
        this->freq = freq;
        left = right = nullptr;
    }
};
void DestroyTree(Node *node) // функция для освобождения памяти выделенной под дерево
{
    if (node)
    {
        if (node->left)
            DestroyTree(node->left);
        if (node->right)
            DestroyTree(node->right);
        delete node;
    }
}
// Сравнение двух узлов по частоте
struct compare
{
    bool operator()(Node *left, Node *right)
    {
        return left->freq > right->freq;
    }
};
// Построение дерева Хаффмана
Node *buildHuffmanTree(string text)
{
    if (text.length() == 0)
    {
        cout << "Error: text.txt is empty";
        exit(1);
    }
    unordered_map<char, int> freqCount;
    // Подсчет частоты встречаемости символов в тексте
    for (char c : text) // слева идентификатор для работы с элементом контейнера, справа контейнер.
    {
        freqCount[c]++;
    }
    priority_queue<Node *, vector<Node *>, compare> pq; //`vector` используется в качестве контейнера для хранения элементов приоритетной очереди.
    // он предоставляет динамический массив элементов, позволяя увеличивать и уменьшать его размер по мере необходимости.
    // compare позволяет определять приоритет при добавлении в очередь
    for (auto pair : freqCount)
    {
        pq.push(new Node(pair.first, pair.second)); // выделяем память для узла дерева и добавляем этот узел в очередь
    }

    // Строим дерево объединяя два узла с наименьшей частотой и помещая новый узел в очередь
    // 1) тот случай когда текст состоит из одного и того же символа (прим: aaaaaaaaaaaaaaaaaaaaa)
    if (pq.size() == 1)
    {
        Node *left = pq.top();
        Node *newNode = new Node('\0', left->freq);
        newNode->left = left;
        return newNode;
    }
    // 2) общий случай
    while (pq.size() != 1) // очередь уменьшается, дерево увеличивается
    {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        Node *newNode = new Node('\0', sum);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }
    return pq.top();
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
            unsigned char mask = 1 << 7;
            // Создаем маску, начиная с самого левого бита

            for (int i = 0; i < bytes; i++)
            {
                for (int j = 0; j < 8 && (k < str.length()); j++)
                {
                    if (str[k] != '0')
                        vec[i] = vec[i] | mask;

                    mask = mask >> 1; // Смещаем маску
                    k++;
                }
                mask = 1 << 7; // Создаем маску, начиная с самого левого бита; // сбрасываем маску перед переходом к следующему байту
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
    if (!encodedText)
        return nullptr;
    for (char c : text) // Упаковываем все биты закодированного текста в один булев вектор
    {
        ShiftLeft(encodedText, bytes * 8, bit_codes[c].len);
        for (size_t i = 0; i < ((bit_codes[c].len - 1) / 8 + 1); i++)
        {
            encodedText[bytes - i - 1] = encodedText[bytes - i - 1] | bit_codes[c].data[i];
        }
    }
    return encodedText;
}
// Декодирование текста с помощью дерева Хаффмана
string decode(unsigned char *encodedText, size_t bytes, unordered_map<char, boolVec> &bit_codes)
{
    string decodedText = ""; // end
    bool flag = true;
    if (encodedText)
    {
        for (size_t k = 0; k < bytes; k++)
        {
            for (const auto &pair : bit_codes) // просматриваем таблицу Хаффмана
            {
                size_t bytesPair = (pair.second.len - 1) / 8 + 1;
                unsigned char *mask = (unsigned char *)calloc(sizeof(unsigned char), bytesPair);
                if (!mask)
                {
                    decodedText = "ERROR MEMORY";
                    return decodedText;
                }
                size_t x = 0;
                for (; x < bytesPair - 1; x++)
                    mask[x] = 1;
                for (size_t y = 0; y <= (pair.second.len - x * 8 - 1); y++)
                {
                    mask[x] = mask[x] | 1 << y;
                    // cout << char (mask[x] + 48) << endl;
                }
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
                    decodedText += pair.first;
                    // cout << char(mask[j - 1] + 48) << endl; // wrong mask
                }
                free(mask);
            }
            if (!flag)
            {
                cout << "ERROR DECODE";
                return "";
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
        ofstream write("encodedtext.txt");
        string text;
        while (getline(in, line))
        {
            text += line;
        }
        root = buildHuffmanTree(text); // создаём на его основе дерево
        generateCodes(root, "", bit_codes);
        int bits = 0;
        size_t bytes = 0;
        for (char c : text)
        {
            bits += bit_codes[c].len;
        }
        bytes = (bits - 1) / 8 + 1;
        cout << "bytes - " << bytes;
        unsigned char *encodedText = encodeText(text, bit_codes, bytes);
        for (size_t i = 0; i < bytes; i++)
        {
            write << encodedText[i];
        }
        write << "\n\n"; // отступ между закодированным текстом и Деревом Хаффмана
        for (const auto &pair : bit_codes)
        {
            write << pair.first << " " << pair.second.len << " " << pair.second.data << endl; // для возможности раскодировать мы запоминаем пары вида "символ" -"код" в файл
        }
        cout << "Encoded text: " << encodedText << endl;
        DestroyTree(root); // освобождаем память потому что выделена она динамически
        DestroyVec(bit_codes);
        free(encodedText);
        write.close();
        in.close();
    }
    else
    {
        ifstream in("encodedtext.txt");
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
        char sym = ' ';
        unsigned char *path = (unsigned char *)calloc(sizeof(unsigned char), 1);
        size_t bytes = 0;
        string temp = "";
        string line = "";
        // считываем из файла построчно закодированный текст
        while (getline(in, line)) // ERROR
        {
            if (line.length() == 0)
                break;
            bytes += line.length();
            (unsigned char *)realloc(encodedText, bytes);
            for (size_t i = 0; i < line.length(); i++)
            {
                encodedText[j] = static_cast<unsigned char>(line[i]);
                j++;
            }
        }
        while (getline(in, line))
        {
            sym = line[0];
            int len = line[2] - 48;
            (unsigned char *)realloc(path, (len - 1) / 8 + 1);
            size_t i = 4;
            for (; line[i]; i++)
            {
                path[i - 4] = static_cast<unsigned char>(line[i]);
            }
            i++;
            bit_codes[sym] = boolVec(path, len);
        }
        free(path);
        cout << "EncodedText: " << encodedText;
        for (auto &pair : bit_codes)
        {
            cout << "\n sym: " << pair.first << " len: " << pair.second.len << " code: " << char(pair.second.data[0]) << endl;
        }
        string decodedText = decode(encodedText, bytes, bit_codes);
        write << decodedText;
        //   записали результат в файл text.txt
        cout << "Decoded text: " <<decodedText <<endl; // вывели результат
        in.close();
        write.close();
    }
    return 0;
}