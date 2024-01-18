using namespace std;
#include <unordered_map>
struct boolVec//структура представляющая булев вектор
{
    unsigned char *data; // Закодированный Символ
    size_t len;          // его длина

    boolVec(unsigned char *data = 0, size_t len = 1)
    {
        this->data = data;
        this->len = len;
    }
};
void DestroyVec(unordered_map<char, boolVec> &bit_codes) // функция для освобождения памяти выделенной под булев вектор
{
    for (auto &pair : bit_codes)
    {
        free(pair.second.data);
    }
}

void ShiftRight(unsigned char *vec, size_t bits, size_t k)//сдвиг булева вектора вправо
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
void ShiftLeft(unsigned char *vec, size_t bits, size_t k)//сдвиг булева вектора влево
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

unsigned char *VecToStr(unsigned char *vec, size_t bits)//преобразование вектора в строку вида 011010101
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
unsigned char *StrToVec(string &str)//преобразование строки в булев вектор
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