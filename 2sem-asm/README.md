# 2 Семинар 09.11.2021 ASM 2

Файлики с кодом:

* [Использование переменной](variable.S)
* [Изменение значения переменной](modify_variable.S)
* [Массив](array.S)
* [Структура person](persons.S)
* [Call/ret](double_eax.S)
* [Работа со стеком](read_and_add.S) (Объяснено, что происходит со стеком)
* Работа с C/C++: [asm](sum_numbers.S), [c](sum_numbers.c), [c++](sum_numbers.cpp)

# Работа со стеком (TBD)


Стек уже известен нам из АИСД, глобальная идея такая же, как и у std::stack
Стек растет в сторону уменьшения адресов! Чем выше запись, тем меньше у нее адрес
То есть, чтобы сдвинуться выше по стеку, нужно из текущего адрера ВЫЧИТАТЬ

     Содержимое стека  Адреса в памяти

     .                .
     .                .
     .                .
     +----------------+ 0x0000F040
     |                |
     +----------------+ 0x0000F044 <-- вершина стека (на неё указывает %esp)
     |     данные     |
     +----------------+ 0x0000F048
     |     данные     |
     +----------------+ 0x0000F04C
     .                .
     .                .
     .                .
     +----------------+ 0x0000FFF8
     |     данные     |
     +----------------+ 0x0000FFFC
     |     данные     |
     +----------------+ 0x00010000 <-- дно стека

Подробнее можно посмотреть [тут](https://ru.wikibooks.org/wiki/%D0%90%D1%81%D1%81%D0%B5%D0%BC%D0%B1%D0%BB%D0%B5%D1%80_%D0%B2_Linux_%D0%B4%D0%BB%D1%8F_%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%81%D1%82%D0%BE%D0%B2_C#%D0%A1%D1%82%D0%B5%D0%BA)

# Работа с C и C++ кодом:

## Компиляция и запуск:

Для C:

```
gcc -m32 file.S file.c -o file && ./file
```

Для C++:

```
g++ -m32 file.S file.cpp -o file && ./file
```

Нужно писать объявление функции в C/C++ файле:

Пример для C:

```
int foo(int);
```

Пример для C++:

```
extern "C" int foo(int);
```

в отличие от C, здесь нужно указать extern "C".
Это нужно из-за того, что в C++ в отличие от C
функции с разными аргументами могут иметь одно то же имя: int plus(bool, bool);
Поэтому при линковке будет искаться не символ plus а другой (с информацией о типах аргументов).
Поэтому нужно явно сказать компилятору, чтобы он работал с этой функцией, как бы он это делал в C


# Все шаги компиляции (TBD)