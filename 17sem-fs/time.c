#include <time.h>
#include <stdio.h>

int main() {
    time_t t = time(NULL); // Возвращает значение времени (количество секунд от начала Эпохи – 
                           // 00:00:00 1 января 1970 года по UTC) в случае успеха
                           // –1 в случае ошибки.
    printf("%lu\n", t);

    printf("year: %ld\n", t / 3600 / 24 / 365 + 1970); // Какой сейчас год

    printf("hour: %ld\n", t / 3600 % 24); // Какой час 
}
