#include "Person.pb.h"
#include "Team.pb.h"
#include <stdio.h>

int g_size = 0;
char* g_buf = NULL;

void WriteTest() {
    Team team;
    team.set_name("ax");

    Person* person1 = team.add_persons();
    person1->set_name("aaa");
    person1->set_age(25);
    person1->set_gender(Male);
    person1->set_married(true);
    person1->add_friends("bbb");
    person1->add_friends("ccc");

    Person* person2 = team.add_persons();
    person2->set_name("xxx");
    person2->set_age(20);
    person2->set_gender(Female);
    person2->set_married(false);
    person2->add_friends("yyy");
    person2->add_friends("zzz");

    g_size = team.ByteSize();
    g_buf = new char[g_size];
    if (!team.SerializeToArray(g_buf, g_size))
        printf("Serialize failed\n");
}

void ReadTest() {
    Team team;
    if (!team.ParseFromArray(g_buf, g_size)) {
        printf("Parse failed\n");
        return;
    }

    if (team.has_name())
        printf("Team name: %s\n", team.name().c_str());

    for(int i = 0; i < team.persons_size(); ++i) {
        const Person& person = team.persons(i);
        printf("====================\n");
        if (person.has_name())
            printf("Name: %s\n", person.name().c_str());
        if (person.has_age())
            printf("Age: %u\n", person.age());
        if (person.has_gender())
            printf("Gender: %s\n", person.gender() == Male ? "Male" : "Female");
        if (person.has_married())
            printf("Married: %s\n", person.married() ? "True" : "False");
        for(int j = 0; j < person.friends_size(); ++j)
            printf("Friend %d: %s\n", j, person.friends(j).c_str());
    }
}

int main() {
    WriteTest();
    ReadTest();

    delete[] g_buf;
    return 0;
}

