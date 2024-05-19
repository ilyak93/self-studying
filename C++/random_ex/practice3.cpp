#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Person {
public:
    Person() : name(""), age(-1){}
    virtual void getdata(){
        cin >> name >> age;
    }
    virtual void putdata(){
        cout << name << " " << age;
    }

private:
    string name;
    int age;
};


class Professor : public Person {
public:
    Professor() : cur_id(id++), publications(-1){}
    void getdata() override{
        Person::getdata();
        cin >> publications;
    }

    void putdata() override{
        Person::putdata();
        cout << " " << publications  << " " << cur_id << endl;
    }

private:
    static int id;
    int publications;
    int cur_id;
};

int Professor::id = 1;


class Student : public Person {
public:
    Student() : cur_id(id++){}
    void getdata() override{
        Person::getdata();
        for(int i = 0; i < marks_num; ++i)
            cin >> marks[i];
    }

    void putdata() override{
        Person::putdata();
        int sum = 0;
        for(int i = 0; i < marks_num; ++i)
            sum += marks[i];
        cout << " " << sum << " " << cur_id << endl;
    }

private:
    static int id;
    const static int marks_num = 6;
    int marks[marks_num];
    int cur_id;
};

int Student::id = 1;

int main(){

    int n, val;
    cin>>n; //The number of objects that is going to be created.
    Person *per[n];

    for(int i = 0;i < n;i++){

        cin>>val;
        if(val == 1){
            // If val is 1 current object is of type Professor
            per[i] = new Professor;

        }
        else per[i] = new Student; // Else the current object is of type Student

        per[i]->getdata(); // Get the data from the user.

    }

    for(int i=0;i<n;i++)
        per[i]->putdata(); // Print the required output for each object.

    return 0;

}
