#include <mutex>
#include <queue>

using namespace std;

template <typename T>
class threadSafe_queue {

    std::queue<int> rawQueue; // ���������, ����� ��� ���� �������
    std::mutex m;

public:

    threadSafe_queue() { count = 0; }
    threadSafe_queue(const threadSafe_queue& _Q) {
        try {
            // ������� �������� ������
            A = new T[_Q.count];
            P = new int[_Q.count];
        }
        catch (bad_alloc e)
        {
            // ���� ������ �� ���������� �� ���������� ������
            cout << e.what() << endl;
            count = 0;
            return;
        }
    }

    int& retrieve_and_delete() {
        int front_value = 0; // ���� ������, return ����� 0
        std::lock_guard<std::mutex> lg(m);
        // ������ ������� ����� ������������, ������� ����� ������ � rawQueue
        if (count > 0)
        {
            delete[] A;
            delete[] P;
            count = 0;
        }
        return front_value;
    };  // ������ ������ ������ ����� ��������� �������

    // ���������� ���������� ��������� � �������
    int& Count() {
        std::lock_guard<std::mutex> lg(m);
        return count;
    }

    void push(T item, int priority) {
        std::lock_guard<std::mutex> lg(m);
        // ������ ������� ����� ������������, ������� ����� ������ � rawQueue
        rawQueue.push(val);
    }; // ������ ������ ������ ����� ��������� �������

    // �������� ����������� - ����� �������� ���������� �����������
    QueueP operator=(const QueueP& _Q);

    // �������, ��������� �������
    void Print(const char* objName) {
        std::lock_guard<std::mutex> lg(m);
        rawQueue.push(val);
    }

private:
    T* A; // ������������ ������ ��������� ���� T
    int* P; // ������ �����������
    int count; // ���������� ��������� � �������
};
