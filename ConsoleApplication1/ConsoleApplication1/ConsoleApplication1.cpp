#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;
// Maksimalus meniu įrašų kiekis masyve
const int MAX_MENU_SIZE = 20;
// Struktūra skirta saugoti patiekalo informaciją
struct menuItemType {
    string menuItem;   // Pavadinimas
    double menuPrice;  // Kaina
};
// Funkcijų prototipai
void getData(menuItemType menuList[], int& count);
void showMenu(menuItemType menuList[], int count);
void printCheck(menuItemType menuList[], int orders[], int count);

int main() {
    menuItemType menuList[MAX_MENU_SIZE]; // Masyvas meniu saugojimui
    int orders[MAX_MENU_SIZE] = { 0 };    // Masyvas užsakytų porcijų kiekiui (pradinė reikšmė 0)
    int itemCount = 0;                    // Kintamasis, sekantis kiek patiekalų nuskaityta
    // Nuskaitome duomenis iš failo
    getData(menuList, itemCount);
    // Jei nepavyko nuskaityti nei vieno įrašo, baigiame programą
    if (itemCount == 0) {
        cout << "Klaida: Nepavyko uzkrauti meniu failo!" << endl;
        return 1;
    }

    // Parodome vartotojui meniu
    showMenu(menuList, itemCount);

    int choice;
    int quantity;

    // Vartotojo įvesties ciklas
    cout << "\nIveskite patiekalo numeri (arba 0, jei baigete): ";
    while (cin >> choice && choice != 0) {
        // Patikriname, ar pasirinktas skaičius yra meniu ribose
        if (choice > 0 && choice <= itemCount) {
            cout << "Kiek porciju? ";
            cin >> quantity;
            if (quantity > 0) {
                // Pridedame kiekį į atitinkamą užsakymų masyvo vietą
                orders[choice - 1] += quantity;
            }
        }
        else {
            cout << "Tokio patiekalo nera. Bandykite dar karta." << endl;
        }
        cout << "Kitas pasirinkimas (0 - baigti): ";
    }

    // Apskaičiuojame sumas, išvedame į ekraną ir įrašome į failą
    printCheck(menuList, orders, itemCount);

    return 0;
}

// Funkcija, kuri nuskaito meniu duomenis iš tekstinio failo
void getData(menuItemType menuList[], int& count) {
    ifstream inputFile("menu.txt"); // Atidarome failą skaitymui
    if (inputFile.is_open()) {
        count = 0;
        // Skaitome tol, kol nepasiekėme masyvo ribos ir kol yra duomenų faile
        while (count < MAX_MENU_SIZE && inputFile >> menuList[count].menuItem >> menuList[count].menuPrice) {
            count++;
        }
        inputFile.close(); // Uždarome failą
    }
}

// Funkcija, kuri išveda visą meniu sąrašą į ekraną
void showMenu(menuItemType menuList[], int count) {
    cout << "Restorano meniu" << endl;
    cout << "---------------------------------------" << endl;
    for (int i = 0; i < count; i++) {
        // Naudojame setw(35) lygiuoti pavadinimą, o fixed ir setprecision kaina rodyti su 2 skaitmenimis
        cout << i + 1 << ". " << left << setw(35) << menuList[i].menuItem
            << fixed << setprecision(2) << menuList[i].menuPrice << " EUR" << endl;
    }
    cout << "---------------------------------------" << endl;
    cout << "Pasirinkite patiekalus ivedami ju numeri." << endl;
}

// Funkcija, kuri apskaičiuoja galutinę sąskaitą ir išsaugo ją faile
void printCheck(menuItemType menuList[], int orders[], int count) {
    ofstream outFile("receipt.txt"); // Atidarome failą rašymui
    double subtotal = 0;
    const double TAX_RATE = 0.21; // PVM mokestis 21%

    cout << "\nSASKAITA:\n" << endl;
    outFile << "SASKAITA:\n" << endl;

    for (int i = 0; i < count; i++) {
        // Jei tam tikro patiekalo kiekis didesnis už 0, įtraukiame į sąskaitą
        if (orders[i] > 0) {
            double itemTotal = menuList[i].menuPrice * orders[i];
            subtotal += itemTotal;

            // Išvedimas į ekraną
            cout << orders[i] << "  " << left << setw(35) << menuList[i].menuItem
                << fixed << setprecision(2) << itemTotal << " EUR" << endl;

            // Išvedimas į failą
            outFile << orders[i] << "  " << left << setw(35) << menuList[i].menuItem
                << fixed << setprecision(2) << itemTotal << " EUR" << endl;
        }
    }

    // Skaičiuojame mokesčius ir galutinę sumą
    double tax = subtotal * TAX_RATE;
    double total = subtotal + tax;

    // Galutinės sumos formatavimas ir išvedimas
    cout << "\n---------------------------------------" << endl;
    cout << left << setw(38) << "Mokesciai (21%):" << tax << " EUR" << endl;
    cout << left << setw(38) << "Galutine suma:" << total << " EUR" << endl;

    outFile << "\n---------------------------------------" << endl;
    outFile << left << setw(38) << "Mokesciai (21%):" << tax << " EUR" << endl;
    outFile << left << setw(38) << "Galutine suma:" << total << " EUR" << endl;

    outFile.close(); // Uždarome failą
    cout << "\nSaskaita issaugota faile receipt.txt" << endl;
}