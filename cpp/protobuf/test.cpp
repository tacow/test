#include "Person.pb.h"
#include <stdio.h>

int g_size = 0;
char* g_buf = NULL;

void WriteTest() {
    Person person;
    person.set_name("aaa");
    person.set_age(25);
    person.set_gender(Male);
    person.set_married(true);
    person.add_friends("bbb");
    person.add_friends("ccc");

    g_size = person.ByteSize();
    g_buf = new char[g_size];
    if (!person.SerializeToArray(g_buf, g_size))
        printf("Serialize failed\n");
}

void ReadTest() {
    Person person;
    if (!person.ParseFromArray(g_buf, g_size)) {
        printf("Parse failed\n");
        return;
    }

    if (person.has_name())
        printf("Name: %s\n", person.name().c_str());
    if (person.has_age())
        printf("Age: %u\n", person.age());
    if (person.has_gender())
        printf("Gender: %s\n", person.gender() == Male ? "Male" : "Female");
    if (person.has_married())
        printf("Married: %s\n", person.married() ? "True" : "False");
    for(int i = 0; i < person.friends_size(); ++i)
        printf("Friend %d: %s\n", i, person.friends(i).c_str());
}

int main() {
    WriteTest();
    ReadTest();

    delete[] g_buf;
    return 0;
}

