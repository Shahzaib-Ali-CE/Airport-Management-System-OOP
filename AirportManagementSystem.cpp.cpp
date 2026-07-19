#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Forward Declaration (Friend Function ke liye)
class AirportManagementSystem;

// INPUT VALIDATION HELPER FUNCTION (BINA LIMITS LIBRARY KE)
int getValidatedInteger(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        
        // Agar user ne integer ki jagah koi letter ya character daal diya
        if (cin.fail()) {
            cin.clear(); // Error flag ko clear karta hai
            cin.ignore(1000, '\n'); // Pehle se majood ghalat 1000 characters tak buffer saaf kar dega
            cout << "[Error]: Invalid Input! Please enter a valid number (Letters are not allowed).\n" << endl;
        } else {
            cin.ignore(1000, '\n'); // Sahi input ke baad trailing newline saaf karne ke liye
            return value;
        }
    }
}

// 1. COMPOSITION CLASS
class FlightSchedule {
private:
    string date;
    int flight_time;

public:
    FlightSchedule() {
        date = "Not Set";
        flight_time = 0;
    }
    ~FlightSchedule() {}

    void setSchedule(string d, int t) {
        date = d;
        flight_time = t;
    }
    string getDate() { return date; }
    int getFlightTime()  { return flight_time; }
};

// 2. BASE CLASS (For Single & Multilevel Inheritance)
class Person {
protected:
    string name;
    string cnic;

public:
    Person() {
        name = "";
        cnic = "";
    }
    virtual ~Person() {}

    // Runtime Polymorphism implementation
    void printPassengerType()  {
        cout << "[Passenger Type]: Standard Citizen Record" << endl;
    }
};

// 3. SINGLE INHERITANCE (Person -> Passenger)
class Passenger : public Person {
protected:
    string passport_number;

public:
    Passenger() {
        passport_number = "PK-A900";
    }
    ~Passenger() {}

    void setPassengerDetails(string n, string c) {
        name = n;
        cnic = c;
    }
};

// 4. MULTILEVEL INHERITANCE (Person -> Passenger -> VIPPassenger)
class VIPPassenger : public Passenger {
protected:
    int lounge_access_code;

public:
    VIPPassenger() {
        lounge_access_code = 101;
    }
    ~VIPPassenger() {}

    void printPassengerType()  {
        cout << "[Passenger Type]: VIP Commercial Passenger (Lounge Access Granted)" << endl;
    }
};

// 5. BASE CLASS 2 (For Multiple Inheritance)
class FlightDetails {
protected:
    string departure_city;
    string arrival_city;
    string flight_class;
    int fare;

public:
    FlightDetails() {
        departure_city = "";
        arrival_city = "";
        flight_class = "";
        fare = 0;
    }
    ~FlightDetails() {}
};

// 6. MULTIPLE INHERITANCE & COMPOSITION (CORE TICKET CLASS)
class Ticket : public VIPPassenger, public FlightDetails {
private:
    int ticket_id;
    
    // Composition: Ticket "Has-A" FlightSchedule inside
    FlightSchedule schedule; 

public:
    Ticket() {
        ticket_id = 0;
    }
    ~Ticket() {}

    void setTicketData(int id, string n, string c, string dep, string arr, string d, int t, string cls, int f) {
        ticket_id = id;
        setPassengerDetails(n, c);
        departure_city = dep;
        arrival_city = arr;
        flight_class = cls;
        fare = f;
        schedule.setSchedule(d, t);
    }

    int getTicketId() { return ticket_id; }
    
    // Friend Function ko access dene ke liye getter bana diya taake time check ho sake
    int getFlightTimeFromSchedule()  { return schedule.getFlightTime(); }
    
    void updateBooking(string new_date, int new_time, string new_city) {
        arrival_city = new_city;
        schedule.setSchedule(new_date, new_time);
    }

    // Compile-time Polymorphism (Function Overloading)
    void printReceipt()  {
        cout << "\n--- Flight Ticket Receipt ---" << endl;
        cout << "Ticket ID:\t\t" << ticket_id << endl;
        cout << "Passenger Name:\t" << name << endl;
        cout << "CNIC:\t\t\t" << cnic << endl;
        cout << "From:\t\t\t" << departure_city << endl;
        cout << "To:\t\t\t" << arrival_city << endl;
        cout << "Date:\t\t\t" << schedule.getDate() << endl;
        cout << "Flight Time:\t\t" << schedule.getFlightTime() << " Hours" << endl;
        cout << "Class:\t\t\t" << flight_class << endl;
        cout << "Fare:\t\t\tRs. " << fare << endl;
    }

    void printReceipt(string layout_type) {
        cout << "[" << layout_type << "] ID: " << ticket_id << " | Passenger: " << name
		 << " | Route: " << departure_city << " to " << arrival_city << " | Time: " << schedule.getFlightTime() << endl;
    }

    // File Handling Methods
    void writeToFile(ofstream &file)  {
        file << ticket_id << "\n" << name << "\n" << cnic << "\n"
             << departure_city << "\n" << arrival_city << "\n" << schedule.getDate() << "\n"
             << schedule.getFlightTime() << "\n" << flight_class << "\n" << fare << "\n";
    }

    void readFromFile(ifstream &file) {
        string d_input;
        int t_input;
        file >> ticket_id;
        file.ignore();
        getline(file, name);
        getline(file, cnic);
        getline(file, departure_city);
        getline(file, arrival_city);
        getline(file, d_input);
        file >> t_input;
        file.ignore();
        getline(file, flight_class);
        file >> fare;
        file.ignore();
        schedule.setSchedule(d_input, t_input);
    }
};

// 7. AIRPORT SYSTEM CONTROLLER CLASS
class AirportManagementSystem {
private:
    Ticket passenger_tickets[1000]; // Array Storage
    int total_bookings;

public:
    AirportManagementSystem() {
        total_bookings = 0;
    }
    ~AirportManagementSystem() {}

    // Friend Function Declaration
    friend void showSystemStatistics( AirportManagementSystem &sys);

    void loadData() {
        ifstream file("AirportData.txt");
        if (!file) return;
        file >> total_bookings;
        file.ignore();
        for (int i = 0; i < total_bookings; i++) {
            passenger_tickets[i].readFromFile(file);
        }
        file.close();
    }

    void saveData()  {
        ofstream file("AirportData.txt");
        if (!file) return;
        file << total_bookings << endl;
        for (int i = 0; i < total_bookings; i++) {
            passenger_tickets[i].writeToFile(file);
        }
        file.close();
    }

    void bookTicket() {
        if (total_bookings >= 1000) {
            cout << "[Notice]: Database storage capacity maxed out!" << endl;
            return;
        }

        int select, select1, select2, select3, select4;
        string dep_city, arr_city, date_input, p_name, cnic_input, class_input;
        int time_input = 0, fare_input = 0;

        do {
            cout << "Select Departure City:\n1. Islamabad\n2. Peshawar\n3. Lahore" << endl;
            select = getValidatedInteger("Choose valid option (1-3): ");
            if (select == 1) dep_city = "Islamabad";
            else if (select == 2) dep_city = "Peshawar";
            else if (select == 3) dep_city = "Lahore";
            else cout << "[Warning]: Choice out of bounds! Try again.\n" << endl;
        } while (select < 1 || select > 3);

        do {
            cout << "Select Arrival City:\n1. Islamabad\n2. Peshawar\n3. Lahore" << endl;
            select1 = getValidatedInteger("Choose valid option (1-3): ");
            if (select1 == 1) arr_city = "Islamabad";
            else if (select1 == 2) arr_city = "Peshawar";
            else if (select1 == 3) arr_city = "Lahore";
            else cout << "[Warning]: Choice out of bounds! Try again.\n" << endl;

            if (dep_city == arr_city && select1 >= 1 && select1 <= 3) {
                cout << "[Error]: Departure and Arrival cities cannot be the same! Choose another destination.\n" << endl;
            }
        } while (select1 < 1 || select1 > 3 || dep_city == arr_city);

        cout << "Enter Date (dd/mm/yyyy): ";
        cin >> date_input;

        do {
            cout << "Select Flight Time:\n1. 1200\n2. 1600" << endl;
            select2 = getValidatedInteger("Choose option: ");
            if (select2 == 1) time_input = 1200;
            else if (select2 == 2) time_input = 1600;
            else cout << "[Warning]: Option doesn't exist.\n" << endl;
        } while (select2 < 1 || select2 > 2);

        do {
            cout << "Select Class:\n1. First Class (50000)\n2. Business Class (35000)\n3. Economy Class (25000)" << endl;
            select3 = getValidatedInteger("Choose option: ");
            if (select3 == 1) { class_input = "First"; fare_input = 50000; }
            else if (select3 == 2) { class_input = "Business"; fare_input = 35000; }
            else if (select3 == 3) { class_input = "Economy"; fare_input = 25000; }
            else cout << "[Warning]: Selected dynamic configuration profile invalid.\n" << endl;
        } while (select3 < 1 || select3 > 3);

        cout << "Enter Passenger Name: ";
        getline(cin, p_name); 

        cout << "Enter CNIC: ";
        cin >> cnic_input;

        int generated_id = 1000 + total_bookings + select2;

        cout << "\nPress 1 to Confirm Ticket Booking: ";
        select4 = getValidatedInteger("Confirm code: ");

        if (select4 == 1) {
            passenger_tickets[total_bookings].setTicketData(generated_id, p_name, cnic_input, dep_city, arr_city, date_input, time_input, class_input, fare_input);
            passenger_tickets[total_bookings].printReceipt(); 
            total_bookings++;
            cout << "\n>>> Flight Ticket Successfully Manifested!" << endl;
        } else {
            cout << "\n>>> Booking Cancelled." << endl;
        }
    }

    void cancelTicket() {
        if (total_bookings == 0) {
            cout << "\n[Notice]: No bookings available to cancel." << endl;
            return;
        }
        int id_to_cancel = getValidatedInteger("Enter Ticket ID to Cancel: ");
        for (int i = 0; i < total_bookings; i++) {
            if (passenger_tickets[i].getTicketId() == id_to_cancel) {
                for (int j = i; j < total_bookings - 1; j++) {
                    passenger_tickets[j] = passenger_tickets[j + 1];
                }
                total_bookings--;
                cout << "Flight Ticket ID " << id_to_cancel << " Cancelled Successfully." << endl;
                return;
            }
        }
        cout << "Ticket ID not found in database records." << endl;
    }

    void viewAllTickets()  {
        if (total_bookings == 0) {
            cout << "\nNo active passenger manifests found." << endl;
            return;
        }
        cout << "\n========= CURRENT REGISTERED MANIFESTS =========" << endl;
        for (int i = 0; i < total_bookings; i++) {
            passenger_tickets[i].printReceipt("SHORT MANIFEST"); 
        }
        cout << "=================================================" << endl;
    }

    void modifyBooking() {
        if (total_bookings == 0) {
            cout << "\n[Notice]: Database empty. Nothing to modify." << endl;
            return;
        }
        int id_to_search = getValidatedInteger("Enter Ticket ID to Modify: ");
        for (int i = 0; i < total_bookings; i++) {
            if (passenger_tickets[i].getTicketId() == id_to_search) {
                string new_date, new_city;
                int select2, select1, new_time;
                
                cout << "Enter New Date (dd/mm/yyyy): "; 
                cin >> new_date;
                
                do {
                    cout << "Select Time:\n1. 1200\n2. 1600" << endl;
                    select2 = getValidatedInteger("Choose options (1-2): ");
                    new_time = (select2 == 1) ? 1200 : 1600;
                } while(select2 < 1 || select2 > 2);

                do {
                    cout << "Select New Arrival City:\n1. Islamabad\n2. Peshawar\n3. Lahore" << endl;
                    select1 = getValidatedInteger("Choose option (1-3): ");
                    new_city = (select1 == 1) ? "Islamabad" : (select1 == 2 ? "Peshawar" : "Lahore");
                } while(select1 < 1 || select1 > 3);

                passenger_tickets[i].updateBooking(new_date, new_time, new_city);
                cout << "\nFlight Booking Updated Successfully!" << endl;
                return;
            }
        }
        cout << "Invalid Ticket ID input entry." << endl;
    }
};

// 8. FRIEND FUNCTION IMPLEMENTATION (TIMING TRACKING ADDED!)
void showSystemStatistics( AirportManagementSystem &sys) {
    int count_1200 = 0;
    int count_1600 = 0;

    // Loop chala kar check kar rahe hain ke kaunsi ticket kis time par booked hai
    for (int i = 0; i < sys.total_bookings; i++) {
        if (sys.passenger_tickets[i].getFlightTimeFromSchedule() == 1200) {
            count_1200++;
        } else if (sys.passenger_tickets[i].getFlightTimeFromSchedule() == 1600) {
            count_1600++;
        }
    }

    cout << "\n=================== BOOKING REPORT ===================" << endl;
    cout << " Total Number of Booked Tickets : " << sys.total_bookings << " / 1000" << endl;
    cout << "------------------------------------------------------" << endl;
    cout << " Active Bookings for 12:00 PM Flight : " << count_1200 << " Tickets" << endl;
    cout << " Active Bookings for 04:00 PM Flight : " << count_1600 << " Tickets" << endl;
    cout << "======================================================" << endl;
}

// MAIN EXECUTIVE INTERFACE TERMINAL
int main() {
    AirportManagementSystem system;
    system.loadData(); 

    int option;
    char choice;

    cout << "===== Welcome to Airport Management System =====" << endl;
    do {
        cout << "\n===== AIRPORT INTERACTION MENU =====" << endl;
        cout << "1. Book Flight Ticket" << endl;
        cout << "2. Flight Ticket Cancellation" << endl;
        cout << "3. View Booked Tickets" << endl;
        cout << "4. Modify Flight Booking" << endl;
        cout << "5. Number of Booked Tickets" << endl; 
        cout << "6. Exit Terminal System" << endl;
        
        option = getValidatedInteger("Choose menu option: ");

        switch (option) {
            case 1: system.bookTicket(); break;
            case 2: system.cancelTicket(); break;
            case 3: system.viewAllTickets(); break;
            case 4: system.modifyBooking(); break;
            case 5: showSystemStatistics(system); break; // Live tracking report call
            case 6: break;
            default: cout << "[Warning]: Invalid entry! Please select options between 1 and 6." << endl;
        }
        if (option == 6) break;
        
        cout << "\nPress Y/y for Main Menu or any other key to close: ";
        cin >> choice;
    } while (choice == 'Y' || choice == 'y');

    system.saveData(); 
    cout << "\nTerminal Session Secured. Airport Database Saved." << endl;
    return 0;
}