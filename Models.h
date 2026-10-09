// ============================================================
//  Models.h - Data models for the DSA Car Rental System
// ============================================================
#pragma once
#include <string>
using namespace std;

struct Car {
    int id;
    string make;
    string model;
    int year;
    string color;
    string plate;        // license plate (unique key for BST index)
    double rentPerDay;
    bool available;

    Car() : id(0), year(0), rentPerDay(0), available(true) {}
    Car(int i, string mk, string md, int y, string c, string p, double r)
        : id(i), make(mk), model(md), year(y), color(c), plate(p),
          rentPerDay(r), available(true) {}
};

struct Customer {
    int id;
    string name;
    string phone;        // unique key for HashMap
    string licenseNo;

    Customer() : id(0) {}
    Customer(int i, string n, string p, string l)
        : id(i), name(n), phone(p), licenseNo(l) {}
};

struct Rental {
    int id;
    int carId;
    int customerId;
    string customerName;
    string carPlate;
    int days;
    double totalCost;
    bool returned;

    Rental() : id(0), carId(0), customerId(0), days(0),
               totalCost(0), returned(false) {}
};

struct RentalRequest {
    string customerPhone;
    string carPlate;
    int days;

    RentalRequest() : days(0) {}
    RentalRequest(string ph, string pl, int d)
        : customerPhone(ph), carPlate(pl), days(d) {}
};
