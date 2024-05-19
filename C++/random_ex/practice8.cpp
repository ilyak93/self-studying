#include<bits/stdc++.h>

using namespace std;

//Define the structs Workshops and Available_Workshops.
//Implement the functions initialize and CalculateMaxWorkshops
struct Workshops {
    int start_time;
    int duration;
    int end_time;
};

struct Available_Workshops {
    int n;
    vector<Workshops> workshops;
};

Available_Workshops* initialize(int start_time[], int duration[], int n) {
    Available_Workshops* available_workshops = new Available_Workshops;
    available_workshops->n = n;

    for (int i = 0; i < n; i++) {
        Workshops workshop;
        workshop.start_time = start_time[i];
        workshop.duration = duration[i];
        workshop.end_time = start_time[i] + duration[i];
        available_workshops->workshops.push_back(workshop);
    }

    return available_workshops;
}

int CalculateMaxWorkshops(Available_Workshops* ptr) {
    int n = ptr->n;
    vector<Workshops> workshops = ptr->workshops;

    // Sort workshops based on end_time
    sort(workshops.begin(), workshops.end(), [](const Workshops& a, const Workshops& b) {
        return a.end_time < b.end_time;
    });

    int max_workshops = 0;
    int current_end_time = 0;

    for (int i = 0; i < n; i++) {
        if (workshops[i].start_time >= current_end_time) {
            max_workshops++;
            current_end_time = workshops[i].end_time;
        }
    }

    return max_workshops;
}

int main(int argc, char *argv[]) {
    int n; // number of workshops
    cin >> n;
    // create arrays of unknown size n
    int* start_time = new int[n];
    int* duration = new int[n];

    for(int i=0; i < n; i++){
        cin >> start_time[i];
    }
    for(int i = 0; i < n; i++){
        cin >> duration[i];
    }

    Available_Workshops * ptr;
    ptr = initialize(start_time,duration, n);
    cout << CalculateMaxWorkshops(ptr) << endl;
    return 0;
}
