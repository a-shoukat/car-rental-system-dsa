// ============================================================
//  main.cpp - Car Rental System (DSA Project)
//  Author: Ayesha Shoukat
//  Demonstrates: Linked List, BST, HashMap, Stack, Queue,
//                Quick Sort, Merge Sort - all implemented
//                from scratch (see DataStructures.h).
// ============================================================
#include <iostream>
#include <string>
#include "RentalSystem.h"
using namespace std;

void showMenu() {
    cout << "\n========== CAR RENTAL SYSTEM (DSA) ==========\n";
    cout << "--- Cars (Linked List + BST index) ---\n";
    cout << " 1. Add Car\n";
    cout << " 2. Remove Car\n";
    cout << " 3. Search Car by Plate (BST)\n";
    cout << " 4. Show All Cars\n";
    cout << " 5. Sort Cars by Rent (Quick Sort)\n";
    cout << " 6. Sort Cars by Year (Merge Sort)\n";
    cout << "--- Customers (Hash Map) ---\n";
    cout << " 7. Add Customer\n";
    cout << " 8. Find Customer by Phone (HashMap)\n";
    cout << "--- Rentals (Stack for Undo) ---\n";
    cout << " 9. Rent a Car\n";
    cout << "10. Return a Car\n";
    cout << "11. Undo Last Rental (Stack)\n";
    cout << "12. Show All Rentals\n";
    cout << "--- Requests (Queue) ---\n";
    cout << "13. Add Rental Request\n";
    cout << "14. Process Next Request\n";
    cout << " 0. Exit\n";
    cout << "============================================\n";
    cout << "Choice: ";
}

int main() {
    RentalSystem sys;
    int choice;

    do {
        showMenu();
        if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); choice = -1; }
        cin.ignore();

        string s1, s2, s3;
        int n1;

        switch (choice) {
            case 1:
                cout << "Make: "; getline(cin, s1);
                cout << "Model: "; getline(cin, s2);
                cout << "Year: "; cin >> n1; cin.ignore();
                cout << "Color: "; getline(cin, s3);
                cout << "License Plate: "; { string p; getline(cin, p);
                cout << "Rent per day (Rs): "; double r; cin >> r; cin.ignore();
                sys.addCar(s1, s2, n1, s3, p, r); }
                break;
            case 2:
                cout << "License Plate: "; getline(cin, s1);
                sys.removeCar(s1);
                break;
            case 3:
                cout << "License Plate: "; getline(cin, s1);
                sys.searchCar(s1);
                break;
            case 4: sys.showAllCars(); break;
            case 5: sys.sortByRent(); break;
            case 6: sys.sortByYear(); break;
            case 7:
                cout << "Name: "; getline(cin, s1);
                cout << "Phone: "; getline(cin, s2);
                cout << "Driving License No: "; getline(cin, s3);
                sys.addCustomer(s1, s2, s3);
                break;
            case 8:
                cout << "Phone: "; getline(cin, s1);
                sys.findCustomer(s1);
                break;
            case 9:
                cout << "Customer Phone: "; getline(cin, s1);
                cout << "Car Plate: "; getline(cin, s2);
                cout << "Days: "; cin >> n1; cin.ignore();
                sys.rentCar(s1, s2, n1);
                break;
            case 10:
                cout << "Car Plate: "; getline(cin, s1);
                sys.returnCar(s1);
                break;
            case 11: sys.undoLastRental(); break;
            case 12: sys.showRentals(); break;
            case 13:
                cout << "Customer Phone: "; getline(cin, s1);
                cout << "Car Plate: "; getline(cin, s2);
                cout << "Days: "; cin >> n1; cin.ignore();
                sys.addRequest(s1, s2, n1);
                break;
            case 14: sys.processRequest(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice, try again.\n";
        }
    } while (choice != 0);

    return 0;
}
