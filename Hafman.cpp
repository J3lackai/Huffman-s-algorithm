using namespace std;
#include <iostream>
#include <locale.h>
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
int main()
{
    setlocale(LC_ALL, "Rus");
    return 0;
}