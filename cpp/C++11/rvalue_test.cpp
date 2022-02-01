#include <iostream>
#include <cstring>
#include <utility>

using namespace std;

class MyString {
public:
    MyString(const char *str = nullptr) {
        cout << "MyString(const char*)" << endl;
        if (str != nullptr) {
            ptr_ = new char[strlen(str) + 1];
            strcpy(ptr_, str);
        } else {
            ptr_ = new char[1];
            *ptr_ = '\0';
        }
    }

    MyString(const MyString& str) {
        cout << "MyString(const MyString&)" << endl;
        ptr_ = new char[strlen(str.ptr_) + 1];
        strcpy(ptr_, str.ptr_);
    }

    MyString(MyString&& str) {
        cout << "MyString(MyString&&)" << endl;
        ptr_ = str.ptr_;
        str.ptr_ = new char[1];
        *str.ptr_ = '\0';
    }

    MyString& operator=(const MyString& str) {
        cout << "operator=(const MyString&)" << endl;
        if (this == &str) {
            return  *this;
        }
        delete[] ptr_;
        ptr_ = new char[strlen(str.ptr_) + 1];
        strcpy(ptr_, str.ptr_);
        return *this;
    }

    MyString& operator=(MyString&& str) {
        cout << "operator=(const MyString&)" << endl;
        ptr_ = str.ptr_;
        str.ptr_ = nullptr;
        return *this;
    }

    ~MyString() {
        cout << "~MyString()" << endl;
        delete[] ptr_;
        ptr_ = nullptr;
    }

    const char* c_str() const {
        return ptr_;
    }

private:
    char *ptr_;
};

int main() {
    MyString s1 = "abc";
    MyString s2 = s1;
    MyString s3 = std::move(s1);
    printf("[%s] [%s] [%s]\n", s1.c_str(), s2.c_str(), s3.c_str());
    return 0;
}
