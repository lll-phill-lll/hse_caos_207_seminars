# 7 ****Семинар 14.12.2021 переполнение буфера, кэш при умножении матриц****

## Scanf

- Обязательно указываем, сколько символов хотим считать
    - Это делаем для того, чтобы не переполнился буффер
    
    ```c
    char buf[10];
    scanf("%s", buf);
    /*
    Маленький ввод работает нормально
    Большой ввод: stack smashing detected
    */
    ```
    
    - gcc и clang умеют детектить,что мы лезем по стеку туда, куда не надо
    - Проблема в том, что нам приходит указатель, scanf не знает длину, буффер переполняется ⇒ он начинает портить память дальше
- Посмотрим, где это может быть проблемой
    - Пусть у нас есть массивчик и пароль. Мы хотим проверить совпадение ввода с паролем и выдать/не выдать доспут
        
        ```c
        char input[9];
            char password[9] = "password";
        
            scanf("%s", input);
        
            if (strcmp(input, password) == 0) {
                printf("Access granted\n");
            } else {
                printf("Access denied\n");
            }
        /*
        Если ввести:
        "\x00aaaaaaaa\x00"
        то нам разрешат войти
        */
        ```
        
    - Проблема в `“%s”`, так как мы можем как-то хитро переполнить буффер
    - **ФАНФАКТ:** мы можем так хитро переполнить буффер, что при вводе совершенно любого пароля нам будет позволен вход
    - **Почему это работает?**
        - `strcmp` не знает размер строки, он просто берет и проверяет до нулевого символа
        ***Итого:*** В `input` пурвым номером записался размер строки, потом записалось что-то, потом переполнился буффер и в `password` первым номером тоже записался конец строки (они лежат друг за другом), значит при сравнении `input` и `password` будет равенство (и там, и там будет ноль)
    - Поэтому ставим ограничение на длину `“%8s”` ввода и тогда наши приколы больше не работают; `scanf` считал `8` символов, а остальное заигнорил

## Кеши

[Пример с перемножением матричек](matrix_mult.cpp)

- В примере мы генерируем две матрички n*m, потом умножаем эти матрички простым перемножением 3 фориками
*Но этот код можно улучшить*
- Вектор хранится по порядку. При работе с ним в кеш попадает не только запрошенный элемент, но и соседей. 
Но кеш ограничен, данные удаляются от туда. Процессор пытается делать так, чтобы мы обращались в кеш чаще, чем в ту память, которая лежит далеко.
    - Идея в том, чтобы не перепрыгивать в новый вектор при изменении `k`. То есть двигаться по одному и тому же вектору, а не прыгать между разными (первая функция в примере плохая)
    
    ```c
    /* На 1000 26sec/18sec */
    ```
    
- На таких языках, где есть GC, кеш процессора выбивается на сборке мусора и мы не так сильно можем на кеш него расчитывать.

### Бинарное дерево на си

- Тут лежат строчки и они умеют сравниваться
    - strcmp
    - int’овый компаратор
- Мы будем в динамической памяти выделять ноды нашего дерева, дальше будем считывать в ноды строчки (тоже в динамическую память, спасибо `scanf` с модификатором `m`. *На маке такого нет*).
- При выделении памяти при помощи calloc, malloc ее обязательно нужно чистить. Иначе память закончится и это убьет процесс.
- insert принимает указатель на указатель на root, так как просто по указателю приходит только копия. Раз мы хотим менять сам указатель, то приходится передавать его по указателю. (Иначе мы бы просто в функции что-то поменяли, а снаружи ничего бы не изменилось). 
*А вот если бы мы принимали вершину, тогда можно было бы не пользоваться двумя указателями*
- Дальше совершенно обычно спускаемся по дереву при помощи функции сравнения
- Печать рекурсивная (DFS справа налево)
- Чистка памяти тоже рекурсивная. Строку тоже чистим.
- Итого: наша программа умеет сортировать ввод
- **В чем проблема:** рекурсия может убить стек
- Также в примере можно заметить функцию, которая правильно удаляет ноды без рекурсии: Функция выпрямляет дерево в бамбук и потом идет по нему.

### Valgraid

- **Valgraid** - Помогает понять, были ли в программе утечки памяти.

`valgrind —leak-check=full ./a.out`