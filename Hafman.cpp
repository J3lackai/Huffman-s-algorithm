using namespace std;
#include <iostream>
#include <queue>
#include <unordered_map>

// Структура узла дерева Хаффмана
struct Node {
    char data;   // Символ
    int freq;    // Частота символа
    Node *left;  // левый потомок
    Node *right; // правый потомок
    
    Node(char data, int freq) {
        this->data = data;
        this->freq = freq;
        left = right = nullptr;
    }
};

// Сравнение двух узлов по частоте
struct compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

// Построение дерева Хаффмана
Node* buildHuffmanTree(string text) {
    // Подсчет частоты встречаемости символов
    unordered_map<char, int> freqCount;
    for (char c : text) {
        freqCount[c]++;
    }
    
    // Создание очереди с приоритетом, сортировка по возрастанию частоты
    priority_queue<Node*, vector<Node*>, compare> pq;
    for (auto pair : freqCount) {
        pq.push(new Node(pair.first, pair.second));
    }
    
    // Строим дерево объединяя два узла с наименьшей частотой и помещая новый узел в очередь
    while (pq.size() != 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        
        int sum = left->freq + right->freq;
        Node* newNode = new Node('\0', sum);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }
    
    return pq.top();
}

// Генерация кодов Хаффмана
void generateCodes(Node* root, string path, unordered_map<char, string>& codes) {
    if (root == nullptr) {
        return;
    }
    
    if (!root->left && !root->right) {
        codes[root->data] = path;
    }
    
    generateCodes(root->left, path + "0", codes);
    generateCodes(root->right, path + "1", codes);
}

// Кодирование текста с помощью дерева Хаффмана
string encode(string text, unordered_map<char, string> codes) {
    string encodedText = "";
    
    for (char c : text) {
        encodedText += codes[c];
    }
    
    return encodedText;
}

// Декодирование текста с помощью дерева Хаффмана
string decode(Node* root, string encodedText) {
    string decodedText = "";
    Node* curr = root;
    
    for (char c : encodedText) {
        if (c == '0') {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
        
        if (!curr->left && !curr->right) {
            decodedText += curr->data;
            curr = root;
        }
    }
    
    return decodedText;
}

int main() {
    string text = "abcdeffghhiijjkkllmmnnooppqrrssttuuvvwwxxxyyyzz";
    
    Node* root = buildHuffmanTree(text);
    unordered_map<char, string> codes;
    generateCodes(root, "", codes);
    
    string encodedText = encode(text, codes);
    cout << "Encoded text: " << encodedText << endl;
    
    string decodedText = decode(root, encodedText);
    cout << "Decoded text: " << decodedText << endl;
    
    return 0;
}