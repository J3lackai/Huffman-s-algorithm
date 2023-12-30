using namespace std;
#include <iostream>
#include <queue>
#include <unordered_map>

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
        cout << "Error: Text == ''";
        exit;
    }

    // Подсчет частоты встречаемости символов
    unordered_map<char, int> freqCount; // создали хэш-таблицу для подсчёта слов <ключ, значение>
    for (char c : text)                 // слева идентификатор для работы с элементом контейнера, справа контейнер.
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
    if (root == nullptr) // случай когда дошли до листа дерева
    {   
        return;
    }
    if (!root->left && !root->right)
    {
        codes[root->data] = path;
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

// Декодирование текста с помощью дерева Хаффмана
string decode(Node *root, string encodedText)
{
    string decodedText = "";
    Node *curr = root;

    for (char c : encodedText)
    {
        if (c == '0')
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }

        if (!curr->left && !curr->right)
        {
            decodedText += curr->data;
            curr = root;
        }
    }

    return decodedText;
}

int main()
{
    string text = "";
    cout << "Enter your text: ";
    cin >> text;
    Node *root = buildHuffmanTree(text); // root - корень
    unordered_map<char, string> codes;
    generateCodes(root, "", codes);

    string encodedText = encode(text, codes);
    cout << "Encoded text: " << encodedText << endl;

    string decodedText = decode(root, encodedText);
    cout << "Decoded text: " << decodedText << endl;

   
    // DeleteTree(root); ОСВОБОЖДЕНИЕ ПАМЯТИ
    return 0;
}