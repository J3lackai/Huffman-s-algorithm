using namespace std;
#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <memory.h>
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
void PrintVec(unsigned char *vector, size_t bits)
{
    if (vector)
    {
        size_t bytes = ((bits - 1) / 8) + 1; // Количество байт
        for (size_t i = 0; i < bytes; i++)
        {
            unsigned char mask = 1 << 0; // Создаем маску, начиная с самого левого бита
            // Цикл для обработки каждого бита в байте
            for (size_t j = 0; j < 8 && i * 8 + j < bits; j++)
            {
                if ((vector[i] & mask) != 0)
                    printf("1"); // Выводим "1", если бит установлен
                else
                    printf("0");  // Выводим "0", если бит не установлен
                mask = mask << 1; // Сдвигаем маску вправо для проверки следующего бита
            }
            printf(" ");
        }
    }
    else
        printf("Can't print vector."); // Если указатель на вектор пуст, выводим сообщение об ошибке
    printf("\n");                      // Переводим строку после вывода битового вектора
}
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
int main()
{
    ofstream write("hehe.txt");
    string a = "\n\n\r\r\r\n";
    string line;
    write << a;
    write.close();
    ifstream in("hehe.txt");
    getline(in, line);
    for (char c : line)
        cout << line.length();
    return 0;
}