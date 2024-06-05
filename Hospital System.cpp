#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Maximum limits for specialization and queue
const int MAX_SPECIALIZATION = 20;
const int MAX_QUEUE = 5;

// Patient class to represent individual patients
class Patient {
public:
    string name;
    int status; // 0 for regular, 1 for urgent

    // Constructor to initialize a patient with a name and status
    Patient(const string& name, int status) : name(name), status(status) {}
};

// Hospital class to manage patients and operations
class Hospital {
private:
    // Array of vectors to hold the queues for each specialization
    vector<Patient> queues[MAX_SPECIALIZATION + 1];

    // Function to shift patients to the left in the queue (removing the first patient)
    void shift_left(int spec) {
        if (queues[spec].empty()) return;
        queues[spec].erase(queues[spec].begin());
    }

    // Function to shift patients to the right in the queue (making space for a new urgent patient)
    void shift_right(int spec) {
        if (queues[spec].size() < MAX_QUEUE) {
            queues[spec].insert(queues[spec].begin(), Patient("", 0));
        }
    }

    // Function to display the menu and get the user's choice
    int menu() {
        int choice = -1;
        while (choice == -1) {
            cout << "\nEnter your choice:\n";
            cout << "1) Add new patient\n";
            cout << "2) Print all patients\n";
            cout << "3) Get next patient\n";
            cout << "4) Exit\n";
            cin >> choice;
            if (!(1 <= choice && choice <= 4)) {
                cout << "Invalid choice. Try again\n";
                choice = -1; // Loop keeps working
            }
        }
        return choice;
    }

public:
    // Function to add a new patient to the hospital system
    bool add_patient() {
        int spec;
        string name;
        int st;

        cout << "Enter specialization, name, status: ";
        cin >> spec >> name >> st;

        if (spec < 0 || spec >= MAX_SPECIALIZATION) {
            cout << "Invalid specialization.\n";
            return false;
        }

        if (queues[spec].size() >= MAX_QUEUE) {
            cout << "Sorry we can't add more patients for this specialization\n";
            return false;
        }

        if (st == 0) { // Regular, add to end
            queues[spec].emplace_back(name, st);
        } else { // Urgent, add to begin
            shift_right(spec);
            queues[spec][0] = Patient(name, st);
        }

        return true;
    }

    // Function to print all patients for all specializations
    void print_patients() {
        cout << "****************************\n";
        for (int spec = 0; spec < MAX_SPECIALIZATION; ++spec) {
            if (!queues[spec].empty()) {
                cout << "There are " << queues[spec].size() << " patients in specialization " << spec << "\n";
                for (const auto& patient : queues[spec]) {
                    cout << patient.name << " ";
                    cout << (patient.status == 1 ? "urgent\n" : "regular\n");
                }
                cout << "\n";
            }
        }
    }

    // Function to retrieve the next patient for a given specialization
    void get_next_patient() {
        int spec;
        cout << "Enter specialization: ";
        cin >> spec;

        if (spec < 0 || spec >= MAX_SPECIALIZATION) {
            cout << "Invalid specialization.\n";
            return;
        }

        if (queues[spec].empty()) {
            cout << "No patients at the moment. Have a rest, Dr\n";
            return;
        }

        // Let patient go to Dr
        cout << queues[spec][0].name << " please go with the Dr\n";

        // Delete the patient in position 0
        shift_left(spec);
    }

    // Main loop to run the hospital system
    void run() {
        while (true) {
            int choice = menu();
            if (choice == 1)
                add_patient();
            else if (choice == 2)
                print_patients();
            else if (choice == 3)
                get_next_patient();
            else
                break;
        }
    }
};

int main() {
    Hospital hospital;
    hospital.run();
    return 0;
}
