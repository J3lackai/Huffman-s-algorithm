using namespace std;
#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>

// Структура узла дерева Хаффмана
struct Node
{
    char data;   // Символ
    int freq;    // Частота символа
    Node *left;  // левый потомок
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
    return;
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
    // 1) тот случай когда в текст состоин из одного и того же символа (прим: aaaaaaaaaaaaaaaaaaaaa)
    if (freqCount[pq.top()->data] == text.length())
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

// Генерация кодов Хаффмана
void generateCodes(Node *root, string path, unordered_map<char, string> &codes) // codes - это хэш-таблица в которой ключ это символ, а значение его кодировка алгоритмом Хаффмана
{
    if (root == nullptr)
    {
        return;
    }
    if (!root->left && !root->right) // случай когда дошли до листа дерева
    {
        codes[root->data] = path; // записываем коды Хаффмана
    }

    generateCodes(root->left, path + "0", codes); // рекурсивно спускаемся по дереву от корня к листьям
    generateCodes(root->right, path + "1", codes);
}
// Кодирование текста с помощью таблицы Хаффмана
string encode(string text, unordered_map<char, string> codes)
{
    string encodedText = "";

    for (char c : text) // проходим по всей хэш-таблице(таблице Хаффмана), достаём все значения по ключам
    // и конкатенируем их с целью получить конечный закодированный текст
    {
        encodedText += codes[c];
    }

    return encodedText;
}

bool checkbool(string text)
{
    int i = 0;
    for (; i < text.length() && (text[i] == '0' || text[i] == '1'); i++)
        ;
    if (i == text.length())
        return true;
    return false;
}
// Декодирование текста с помощью дерева Хаффмана
string decode(string encodedText, unordered_map<char, string> codes)
{
    if (encodedText.length() == 0)
    {
        cout << "Error: encodedtext.txt is empty";
        exit(1);
    }
    if (!checkbool(encodedText))
    {
        cout << "ERROR: the encodedText must contain only '1' or '0'";
        exit(1);
    }
    string decodedText = "";
    int i = 0;
    int j = 0;
    bool flag = true;
    for (i = 0; i < encodedText.length() && flag;)
    {
        flag = false;
        for (const auto &pair : codes)
        {
            for (j = 0; j < pair.second.length() && encodedText[i + j] == pair.second[j]; j++)
                ;
            if (j == pair.second.length())
            {
                decodedText += pair.first;
                i += j;
                flag = true; // отслеживаем флаг чтобы избежать зацикливания при неверно введённых данных
                break;
            }
        }
    }
    if (!flag)
        cout << "ERROR: incorrect input data in HuffmanTree\n";
    return decodedText;
}
int main()
{   

    unordered_map<char, string> codes; // таблица Хаффмана
    Node *root = 0;
    string line ="";
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
        ofstream HufTree("HuffmanTree.txt");
        ofstream write("encodedtext.txt");
        string text;
        while (getline(in, line))
        {
            text += line;
        } 
        root = buildHuffmanTree(text); // создаём на его основе дерево
        generateCodes(root, "", codes);

        for (const auto &pair : codes)
        {
            HufTree << pair.first << " " << pair.second << endl; // для возможности раскодировать мы запоминаем пары вида "символ" -"код" в файл
        }
        string encodedText = encode(text, codes);
        write << encodedText;
        cout << "Encoded text: " << encodedText << endl;
        DestroyTree(root); // освобождаем память потому что выделена она динамически
        write.close();
        in.close();
        HufTree.close();
    }
    else
    {
        ifstream in("encodedtext.txt");
        ifstream HufTree("HuffmanTree.txt");
        ofstream write("text.txt");
        if (!in.is_open())
        {
            cout << "ERROR: file 'encodedtext.txt' not found create it \n";
            return 1;
        }
        if (!HufTree.is_open())
        {
            cout << "ERROR: file 'HuffmanTree.txt' not found create it \n";
            return 1;
        }
        char sym = ' ';
        string path = "";
        string encodedText;
        while (getline(in, line))
        {
            encodedText += line;
        }
        line = "";
        while (getline(HufTree, line))// извлекаем из файла символы и соответсвующие им коды
        {
            sym = line[0];
            path = "";
            for (size_t i = 2; line[i]; i++)
            {
                path += line[i];
            }
            codes[sym] = path;
        }

        string decodedText = decode(encodedText, codes);
        write << decodedText;
        cout << "Decoded text: " << decodedText << endl;
        in.close();
        HufTree.close();
        write.close();
    }
    return 0;
}