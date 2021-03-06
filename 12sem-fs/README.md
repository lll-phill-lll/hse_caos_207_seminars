# ****12 Семинар 15.02.2022 fork****

[fork_atexit.c](fork_atexit.c)

[fork_buf.c](fork_buf.c)

[fork_file.c](fork_file.c)

## Ожидание детей

- **Зачем родителю всегда ждать ребенка?**
    - Родитель всегда должен ждать ребенка, потому что когда завершается ребенок, он остается в таблице процессов для того, чтобы родитель мог его подождать и понять, как именно ребенок завершился (с ошибкой, убитый). Такой процесс называется **zombie**
    Он находится в таком состоянии, пока не завершится родитель и не сделает от него wait
- **Что будет с ребенком, если родитель его не подождет?**
    - Тогда родителем этого процесса станет процесс `init` (`pid=1`) и этот новый родитель дождется, когда процесс завершится и убьёт его. Таким образом процессы не теряются и не висят вечно в zombie, чтобы кто-то его дождался и завершил.

```c
pid_t pid;

    printf("initial parent pid = %d\n", getpid());
    if ((pid = fork()) == 0) {
		for(int i = 0; i != 10; ++i) {
            printf("parent pid = %d\n", getppid());
            sleep(1);
        }
    }

    sleep(5);
/*
Вывод будет: 
initial parent pid = 85832
parent pid = 85832
parent pid = 85832
parent pid = 85832
parent pid = 85832
parent pid = 85832
parent pid = 85832
parent pid = 1
parent pid = 1
parent pid = 1
.....
*/
```

[init_child.c](init_child.c)

`sleep()` - приостанавливает выполнение программы на время, задаваемое параметром time. Время задается в секундах.

`getpid()` - возвращает идентификатор ID текущего процесса. (Это часто используется функциями, которые генерируют уникальные имена временных файлов)

`getppid()` - возвращает идентификатор ID родительского процесса. 

- Но это не значит, что можно пложить процессы и надеется на то, что init все почистит (к примеру, если достичь лимита, то процессы новые создаться не смогут, а старые не смогут убиться).

**Итого:** То есть всегда ждем процесс и пытаемся его убить как можно раньше

## Неожиданные последствия работы форков

```c
if (fork() == 0) {
        exit(0);
    };

    sleep(1);
/*
Вывод будет: 
hellohello
*/

if (fork() == 0) {
        _exit(0);
    };

    sleep(1);
/*
Вывод будет: 
hello
*/
```

[fork_buf.c](fork_buf.c)

`exit(0)` выполнится только в ребенке. Родитель пропустит `if`. 

- Вывод будет такой, потому что буффер сбросился два раза и при exit, и при return, так как память в ребенка из родителя копируется.
- На самом деле нужно использовать `_exit(0)`, так как это означает использование системного вызова ⇒ буффер не будет освобождаться

### Пусть хотим, чтобы и ребенок, и родитель писали в файл

[fork_atexit.c](fork_atexit.c)

`atexit()` - устанавливает функцию, на которую указывает параметр `func`, в качестве вы­зываемой при нормальном завершении программы. Иными словами, указанная функция вызыва­ется в конце выполнения программы. Согласно стандарту ANSI С процесс установки такой фун­кции называется регистрацией.
(Функции выполняются в обратном порядке)

- **Про пример:**
    - Закрываем файл всегда после выхода
    - Вообще мы открываем файл, здороваемся с миром в этом файле, если не удалось, печатаем ошибку, иначе говорим, что всё хорошо и печатаем ID процесса, который ее вызвал. При каждом вызове функции будем открывать и закрывать файл.
    - Тогда напишем в файл из родительского процесса, сделаем fork() и запишем в файл из ребенка, а потом еще раз попробуем сделать это из родителя
    - По итогу хотим, чтобы в файл три раза написалось *hello*
    - Если там exit(0), то мы файл удалим, а потом будем открывать — ошибка
    
    ```c
    /*
    Вывод будет:
    Parent pid: 94792
    Pid: 94793 written successfully
    Child pid: 94793
    Pid: 94793 written successfully
    file deleted
    Pid: 94792: No such file or dir
    delete: No such file or dir
    */
    ```
    
    - Правильно делать `_exit(0)`
    
    ```c
    /*
    Вывод будет:
    Parent pid: 95712
    Pid: 95712 written successfully
    Child pid: 95713
    Pid: 95713 written successfully
    Pid: 95712 written successfully
    file deleted
    */
    ```
    
    `wait(NULL)` - ждет любого ребенка
    

## wait()

Вообще у ребенка есть много способов завершиться.

`waitpid(pid, &status, 0)` - в статус записывается то, как именно завершился ребенок

- Проверка того, завершился ли ребенок `WIFEXITED`
    - Проверка статуса равершения `WIESITSTATUS` (используется только если `WIFEXITED` вернул 1)
- Проверка был ли убит ребенок `WIFSIGNALED`
    - `WTERMSIG` - номер сигнала, при помощи которого убили  (используется только если `WIFSIGNALED` вернул 1)

[wait_example.c](wait_example.c)

- То есть по выходным статусам детей мы можем что-то особенное делать в родителе

[wait_example_2.c](wait_example_2.c)

- `ctrl+c` завершает только текущий процесс
- При закрытии терминала убиваются все процессы, которые мы там понасоздавали

Есть еще хитрый `wait4()`, в него передается структурка, в которую записываются интересные данные, к примеру, про то, сколько он длился и вообще как он работал

### options

- `WNOHANG` - “нет рукопожатия” — не ждать, если не завершился процесс

[wnohang_example.c](wnohang_example.c)

- Поскольку ребенок будет жить 10 секунд, то родитель тоже должен 10 раз по секунде засыпать

```c
/*
waiting 1 sec
child pid = 13399
waiting 1 sec
waiting 1 sec
waiting 1 sec
waiting 1 sec
waiting 1 sec
waiting 1 sec
waiting 1 sec
waiting 1 sec
waiting 1 sec
waiting 1 sec
child finished
*/
```

- То есть можно узнавать, как дела у ребенка и не останавливаться в выполнении