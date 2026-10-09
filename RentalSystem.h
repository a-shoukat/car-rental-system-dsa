// ============================================================
//  RentalSystem.h - Core system wiring all data structures:
//   LinkedList<Car>      car inventory
//   BST<string,int>      license-plate -> car id index
//   HashMap<Customer>    phone -> customer lookup
//   Stack<Rental>        undo last rental
//   Queue<RentalRequest> pending rental requests
// ============================================================
#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "DataStructures.h"
#include "Models.h"
#include "Algorithms.h"

class RentalSystem {
private:
    LinkedList<Car> cars;
    BST<string, int> plateIndex;          // plate -> car id
    HashMap<Customer> customers;          // phone -> customer
    vector<Customer> customerList;        // for display
    vector<Rental> rentals;
    Stack<Rental> undoStack;
    Queue<RentalRequest> requests;
    int nextCarId, nextCustId, nextRentId;

public:
    RentalSystem() : nextCarId(1), nextCustId(1), nextRentId(1) {
        seedDemoData();
    }

    // ---------------- CARS ----------------
    void addCar(const string& make, const string& model, int year,
                const string& color, const string& plate, double rent) {
        if (plateIndex.search(plate)) {
            cout << "A car with this plate already exists!\n";
            return;
        }
        Car c(nextCarId++, make, model, year, color, plate, rent);
        cars.pushBack(c);
        plateIndex.insert(plate, c.id);
        cout << "Car added: " << make << " " << model << " (" << plate << ")\n";
    }

    bool removeCar(const string& plate) {
        int* id = plateIndex.search(plate);
        if (!id) { cout << "Car not found.\n"; return false; }
        Car* c = findCarById(*id);
        if (c && !c->available) {
            cout << "Car is currently rented - cannot remove.\n";
            return false;
        }
        cars.removeIf([id](const Car& x) { return x.id == *id; });
        plateIndex.remove(plate);
        cout << "Car removed.\n";
        return true;
    }

    // O(log n) BST search by license plate
    void searchCar(const string& plate) {
        int* id = plateIndex.search(plate);
        if (!id) { cout << "No car found with plate " << plate << ".\n"; return; }
        printCar(findCarById(*id));
    }

    void showAllCars() {
        if (cars.empty()) { cout << "No cars in the system.\n"; return; }
        vector<Car> v;
        cars.toVector(v);
        printHeader();
        for (auto& c : v) printCar(&c);
    }

    void sortByRent() {
        vector<Car> v;
        cars.toVector(v);
        if (v.empty()) { cout << "No cars to sort.\n"; return; }
        quickSort(v, 0, (int)v.size() - 1,
                  [](const Car& a, const Car& b) { return a.rentPerDay < b.rentPerDay; });
        cout << "\n--- Cars sorted by rent/day (Quick Sort: O(n log n) avg) ---\n";
        printHeader();
        for (auto& c : v) printCar(&c);
    }

    void sortByYear() {
        vector<Car> v;
        cars.toVector(v);
        if (v.empty()) { cout << "No cars to sort.\n"; return; }
        mergeSort(v, 0, (int)v.size() - 1,
                  [](const Car& a, const Car& b) { return a.year < b.year; });
        cout << "\n--- Cars sorted by year (Merge Sort: O(n log n)) ---\n";
        printHeader();
        for (auto& c : v) printCar(&c);
    }

    // ---------------- CUSTOMERS ----------------
    void addCustomer(const string& name, const string& phone, const string& lic) {
        if (customers.get(phone)) {
            cout << "A customer with this phone already exists!\n";
            return;
        }
        Customer c(nextCustId++, name, phone, lic);
        customers.put(phone, c);
        customerList.push_back(c);
        cout << "Customer added: " << name << "\n";
    }

    // O(1) average HashMap lookup
    void findCustomer(const string& phone) {
        Customer* c = customers.get(phone);
        if (!c) { cout << "No customer found with phone " << phone << ".\n"; return; }
        cout << "ID: " << c->id << " | Name: " << c->name
             << " | Phone: " << c->phone << " | License: " << c->licenseNo << "\n";
    }

    // ---------------- RENTALS ----------------
    void rentCar(const string& phone, const string& plate, int days) {
        Customer* cust = customers.get(phone);
        if (!cust) { cout << "Customer not found. Add them first.\n"; return; }
        int* id = plateIndex.search(plate);
        if (!id) { cout << "Car not found.\n"; return; }
        Car* car = findCarById(*id);
        if (!car->available) { cout << "Car is already rented!\n"; return; }
        if (days < 1) days = 1;

        Rental r;
        r.id = nextRentId++;
        r.carId = car->id;
        r.customerId = cust->id;
        r.customerName = cust->name;
        r.carPlate = car->plate;
        r.days = days;
        r.totalCost = days * car->rentPerDay;
        r.returned = false;

        car->available = false;
        rentals.push_back(r);
        undoStack.push(r);   // enable UNDO
        cout << "Rented! " << car->make << " " << car->model
             << " to " << cust->name << " for " << days
             << " day(s). Total: Rs " << fixed << setprecision(0) << r.totalCost << "\n";
    }

    void returnCar(const string& plate) {
        int* id = plateIndex.search(plate);
        if (!id) { cout << "Car not found.\n"; return; }
        Car* car = findCarById(*id);
        if (car->available) { cout << "This car is not rented.\n"; return; }
        for (auto& r : rentals) {
            if (r.carId == car->id && !r.returned) {
                r.returned = true;
                break;
            }
        }
        car->available = true;
        cout << "Car " << plate << " returned. Thank you!\n";
    }

    // UNDO via stack - reverses the most recent rental
    void undoLastRental() {
        Rental r;
        if (!undoStack.pop(r)) {
            cout << "Nothing to undo.\n";
            return;
        }
        Car* car = findCarById(r.carId);
        if (car) car->available = true;
        for (auto& x : rentals) {
            if (x.id == r.id && !x.returned) { x.returned = true; break; }
        }
        cout << "Undone rental #" << r.id << " (" << r.carPlate << ").\n";
    }

    void showRentals() {
        if (rentals.empty()) { cout << "No rentals yet.\n"; return; }
        cout << "\n" << left << setw(5) << "ID" << setw(12) << "Plate"
             << setw(18) << "Customer" << setw(7) << "Days"
             << setw(12) << "Total(Rs)" << "Status\n";
        cout << string(62, '-') << "\n";
        for (auto& r : rentals) {
            cout << left << setw(5) << r.id << setw(12) << r.carPlate
                 << setw(18) << r.customerName << setw(7) << r.days
                 << setw(12) << fixed << setprecision(0) << r.totalCost
                 << (r.returned ? "Returned" : "Active") << "\n";
        }
    }

    // ---------------- REQUEST QUEUE ----------------
    void addRequest(const string& phone, const string& plate, int days) {
        requests.enqueue(RentalRequest(phone, plate, days));
        cout << "Request queued. Position: " << requests.size() << "\n";
    }

    void processRequest() {
        RentalRequest req;
        if (!requests.dequeue(req)) {
            cout << "No pending requests.\n";
            return;
        }
        cout << "Processing request: " << req.customerPhone
             << " wants " << req.carPlate << " for " << req.days << " day(s)...\n";
        rentCar(req.customerPhone, req.carPlate, req.days);
    }

    int pendingRequests() const { return requests.size(); }

private:
    Car* findCarById(int id) {
        return cars.findIf([id](const Car& c) { return c.id == id; });
    }

    void printHeader() {
        cout << "\n" << left << setw(5) << "ID" << setw(12) << "Plate"
             << setw(22) << "Car" << setw(6) << "Year"
             << setw(14) << "Rent/Day" << "Status\n";
        cout << string(68, '-') << "\n";
    }

    void printCar(Car* c) {
        if (!c) return;
        cout << left << setw(5) << c->id << setw(12) << c->plate
             << setw(22) << (c->make + " " + c->model) << setw(6) << c->year
             << setw(14) << ("Rs " + to_string((int)c->rentPerDay))
             << (c->available ? "Available" : "Rented") << "\n";
    }

    void seedDemoData() {
        addCar("Toyota", "Corolla", 2022, "White", "LEA-1234", 5000);
        addCar("Honda", "Civic", 2023, "Black", "LEB-5678", 7000);
        addCar("Suzuki", "Swift", 2021, "Silver", "LEC-9012", 4000);
        addCar("Kia", "Sportage", 2024, "Red", "LED-3456", 9000);
        addCustomer("Ahmed Khan", "03001234567", "DL-1001");
        addCustomer("Sara Ahmed", "03219876543", "DL-1002");
        cout << "\n(Demo data loaded: 4 cars, 2 customers)\n";
    }
};
