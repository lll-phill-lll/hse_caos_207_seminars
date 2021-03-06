/*
 * Код, который обрабатывает построчно что-то, используя fork
 *
 */
int main() {
    // Читаем построчно
    while (stringHasMoreLines()) {
        // Каждую строку обрабатываем отдельно
        processNextLine();
    }
}

void processNextLine() {
    // делаем что-то в ребенке
    if (fork() == 0) {
        // Если здесь не сделать _exit(0), то получится форк бомба. Ребенок не завершится,
        // а продолжит выполнение кода родителя, вернется из функции processNextLine и сам
        // создаст еще одного ребенка. И так количество детей будет расти экспоненциально
        // Поэтому важно следить, чтобы дети вовремя завершались
        // _exit(0);
    }
    // делаем что-то в родителе
}
