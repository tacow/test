#include <iostream>
#include <limits>

using namespace std;

int main() {
    cout << "Max INT: " << numeric_limits<int>::max() << endl;
    cout << "Min INT: " << numeric_limits<int>::min() << endl;
    cout << "Max Double: " << numeric_limits<double>::max() << endl;
    cout << "Min Double: " << numeric_limits<double>::min() << endl;
    return 0;
}

