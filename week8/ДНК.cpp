#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

using namespace std;

int find(string str);

int main()
{
    string KOD = "AGTCGGAATTCCCACTCGCGTACGCGATATAT";
    find(KOD);
    return 0;
}

int find(string str) {
    std::lock_guard < std::mutex > lock(mutex);
    size_t pos = 0;
    vector<size_t> positions;
    positions.push_back(pos = str.find("AC"));

    while ((pos = str.find("AC", pos + 2)) != string::npos) {
        positions.push_back(pos);
    }

    for (auto elem : positions) {
        cout << elem << endl;
    }
    return 0;
}

