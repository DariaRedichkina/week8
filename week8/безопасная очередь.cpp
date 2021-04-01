#include <mutex>
#include <queue>

using namespace std;

template <typename T>
class threadSafe_queue {

    std::queue<int> rawQueue; // структура, общая для всех потоков
    std::mutex m;

public:

    threadSafe_queue() { count = 0; }
    threadSafe_queue(const threadSafe_queue& _Q) {
        try {
            // попытка выделить память
            A = new T[_Q.count];
            P = new int[_Q.count];
        }
        catch (bad_alloc e)
        {
            // если память не выделилась то обработать ошибку
            cout << e.what() << endl;
            count = 0;
            return;
        }
    }

    int& retrieve_and_delete() {
        int front_value = 0; // если пустая, return будет 0
        std::lock_guard<std::mutex> lg(m);
        // Отныне текущий поток единственный, который имеет доступ к rawQueue
        if (count > 0)
        {
            delete[] A;
            delete[] P;
            count = 0;
        }
        return front_value;
    };  // теперь другие потоки могут захватить мьютекс

    // возвращает количество элементов в очереди
    int& Count() {
        std::lock_guard<std::mutex> lg(m);
        return count;
    }

    void push(T item, int priority) {
        std::lock_guard<std::mutex> lg(m);
        // отныне текущий поток единственный, который имеет доступ к rawQueue
        rawQueue.push(val);
    }; // теперь другие потоки могут захватить мьютекс

    // оператор копирования - чтобы избежать побитового копирования
    QueueP operator=(const QueueP& _Q);

    // функция, выводящая очередь
    void Print(const char* objName) {
        std::lock_guard<std::mutex> lg(m);
        rawQueue.push(val);
    }

private:
    T* A; // динамический массив элементов типа T
    int* P; // массив приоритетов
    int count; // количество элементов в очереди
};
