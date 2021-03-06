# 11 Семинар 08.02.2022 mmap/fork

## Файлики с кодом:

* [Пример работы mmap](mmap_example.c)
* [Простой fork](simple_fork.c)
* [Считаем количество процессов](fork_proc_num.c)
* [Fork бомба](fork_bomb_1.c)
* [Случайная fork бомба](fork_bomb_2.c)
* [Проверяем, изменится ли переменная из ребенка](fork_var_change.c)
* [Делаем так, чтобы переменная менялась из ребенка](fork_var_change_mmap.c)
* [С помощью fork посчитать лимит на созданые процессы](fork_limit.c)
* [Узнать лимиты из программы](limit.c)



# mmap

```
#include <sys/mman.h>

void *mmap(
    void *addr,    /* рекомендуемый адрес отображения, можно передавать NULL, если адрес не важен */
    size_t length, /* размер отображения */
    int prot,      /* аттрибуты доступа */
    int flags,     /* флаги совместного отображения */
    int fd,        /* файловый дескриптор файла */
    off_t offset   /* смещение относительно начала файла */
  );

int munmap(void *addr, size_t length) /* освободить отображение */

```

При ошибке `mmap` возвращает специальное значение `MAP_FAILED`.
Чтобы детектировать ошибку, нужно проверять возвращаемое значение не на `NULL`,
а на `MAP_FAILED`

## Значения prot:

	 PROT_NONE   Pages may not be accessed.
     PROT_READ   Pages may be read.
     PROT_WRITE  Pages may be written.
     PROT_EXEC   Pages may be executed.

Использовать с битовым или: `PROT_READ | PROT_WRITE`

## Значения flags:

* `MAP_FIXED` - требует, чтобы память была выделена по указаному в первом аргументе адресу; без этого флага ядро может выбрать адрес, наиболее близкий к указанному.
* `MAP_ANONYMOUS` - выделить страницы в оперативной памяти, а не связать с файлом. В таком случае значение файлового дескриптора игнорируется.
* `MAP_SHARED` - выделить страницы, разделяемые с другими процессами; в случае с отображением на файл - синхронизировать изменения так, чтобы они были доступны другим процессам.
* `MAP_PRIVATE` - в противоположность `MAP_SHARED`, не делать отображение доступным другим процессам. В случае отображения на файл, он доступен для чтения, а созданные процессом изменения, в файл не сохраняются.

# Fork:

Создание нового процесса осуществляется с помощью системного вызова fork, который создаёт почти точную копию текущего процесса, причём оба процесса продолжают своё выполнение со следующей, после вызова fork, инструкции.

Вновь созданный (сыновний) процесс отличается только своим идентификатором процесса pid и идентификатором родительского процесса ppid. Кроме того, у нового процесса сбрасываются счётчики использования ресурсов, блокировки файлов и ожидающие сигналы.

Сыновний процесс продолжает выполнять тот же код, что и родительский процесс с точки программы, непосредственно после возврата из функции fork. Отличить новый процесс от родительского можно только по возвращаемому системным вызовом fork значению. В сыновний процесс возвращается 0, а в родительский процесс возвращается идентификатор сыновнего процесса. Кроме того, функция fork возвращает число -1, когда новый процесс не может быть создан из-за нехватки ресурсов, либо из-за превышения максимального разрешённого числа процессов для пользователя или всей системы.

Посмотрите лекцию прошлого года.

# ulimit:

Чтобы посмотреть различные лимиты в системе, можно использовать команду `ulimit`

    ulimit -a - посмотреть все лимиты
    ulimit -u - лимит на количество созданных процессов
    ulimit -s - размер стека
    ulimit -n - количество файловых дескрипторов

У лимита есть 2 значения soft и hard.
soft - может меняться любым пользователем в диапазоне от 0 до hard.
hard - может уменьшаться любым пользователем, увеличиваться только супер пользователем.
Лимит выставляется по soft.

    ulimit -S -u - посмотреть soft limit на процессы (в этот лимит упирается форк бомба)
    ulimit -H -u - посмотреть hard limit на процессы запущенные пользователем
    ulimit -S -u 1000 - установить soft limit на процессы запущенные пользователем
    ulimit -H -u 1000 - установить hard limit на процессы запущенные пользователем

Подробнее: `man limits.conf`

# Команды с семинара:

* `ps -aux` - посмотреть на все запущенные процессы
* `pstree` - посмотреть на дерево процессов
* `ulimit -a` - посмотреть различные лимиты



# References:

* [Лекция с физтеха (mmap)](https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/mmap)
* [Прошлый год (mmap)](https://github.com/blackav/hse-caos-2020/tree/master/14-mmap)
* [Лекция с физтеха (fork)](https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/fork)
* [Прошлый год (fork)](https://github.com/blackav/hse-caos-2020/tree/master/15-fork)
