using namespace std;
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