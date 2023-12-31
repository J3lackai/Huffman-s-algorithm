using namespace std;
#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>

// ��������� ���� ������ ��������
struct Node
{
    char data;   // ������
    int freq;    // ������� �������
    Node *left;  // ����� �������
    Node *right; // ������ �������

    Node(char data, int freq)
    {
        this->data = data;
        this->freq = freq;
        left = right = nullptr;
    }
};
void DestroyTree(Node *node) // ������� ��� ������������ ������ ���������� ��� ������
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
// ��������� ���� ����� �� �������
struct compare
{
    bool operator()(Node *left, Node *right)
    {
        return left->freq > right->freq;
    }
};
// ���������� ������ ��������
Node *buildHuffmanTree(string text)
{
    if (text.length() == 0)
    {
        cout << "Error: text.txt is empty";
        exit(1);
    }
    unordered_map<char, int> freqCount;
    // ������� ������� ������������� �������� � ������
    for (char c : text) // ����� ������������� ��� ������ � ��������� ����������, ������ ���������.
    {
        freqCount[c]++;
    }
    priority_queue<Node *, vector<Node *>, compare> pq; //`vector` ������������ � �������� ���������� ��� �������� ��������� ������������ �������.
    // �� ������������� ������������ ������ ���������, �������� ����������� � ��������� ��� ������ �� ���� �������������.
    // compare ��������� ���������� ��������� ��� ���������� � �������
    for (auto pair : freqCount)
    {
        pq.push(new Node(pair.first, pair.second)); // �������� ������ ��� ���� ������ � ��������� ���� ���� � �������
    }

    // ������ ������ ��������� ��� ���� � ���������� �������� � ������� ����� ���� � �������
    // 1) ��� ������ ����� � ����� ������� �� ������ � ���� �� ������� (����: aaaaaaaaaaaaaaaaaaaaa)
    if (freqCount[pq.top()->data] == text.length())
    {
        Node *left = pq.top();
        Node *newNode = new Node('\0', left->freq);
        newNode->left = left;
        return newNode;
    }
    // 2) ����� ������
    while (pq.size() != 1) // ������� �����������, ������ �������������
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

// ��������� ����� ��������
void generateCodes(Node *root, string path, unordered_map<char, string> &codes) // codes - ��� ���-������� � ������� ���� ��� ������, � �������� ��� ��������� ���������� ��������
{
    if (root == nullptr)
    {
        return;
    }
    if (!root->left && !root->right) // ������ ����� ����� �� ����� ������
    {
        codes[root->data] = path; // ���������� ���� ��������
    }

    generateCodes(root->left, path + "0", codes); // ���������� ���������� �� ������ �� ����� � �������
    generateCodes(root->right, path + "1", codes);
}
// ����������� ������ � ������� ������� ��������
string encode(string text, unordered_map<char, string> codes)
{
    string encodedText = "";

    for (char c : text) // �������� �� ���� ���-�������(������� ��������), ������ ��� �������� �� ������
    // � ������������� �� � ����� �������� �������� �������������� �����
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
// ������������� ������ � ������� ������ ��������
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
                flag = true; // ����������� ���� ����� �������� ������������ ��� ������� �������� ������
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
    unordered_map<char, string> codes; // ������� ��������
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
        root = buildHuffmanTree(text); // ������ �� ��� ������ ������
        generateCodes(root, "", codes);

        for (const auto &pair : codes)
        {
            HufTree << pair.first << " " << pair.second << endl; // ��� ����������� ������������� �� ���������� ���� ���� "������" -"���" � ����
        }
        string encodedText = encode(text, codes);
        write << encodedText;
        cout << "Encoded text: " << encodedText << endl;
        DestroyTree(root); // ����������� ������ ������ ��� �������� ��� �����������
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
        while (getline(HufTree, line))// ��������� �� ����� ������� � �������������� �� ����
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