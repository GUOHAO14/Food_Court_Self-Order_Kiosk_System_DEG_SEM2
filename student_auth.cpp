#include "student_auth.h"
#include <iostream>
#include <fstream>

using namespace std;

string toUpperString(string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - ('a' - 'A');
        }
    }
    return str;
}

// Validate starts with 'TP' followed by 6 digits 
bool isValidStudentIdFormat(const string& input) {
    if (input.length() != 8) return false;

    if ((input[0] != 'T' && input[0] != 't') || (input[1] != 'P' && input[1] != 'p')) {
        return false;
    }

    for (size_t i = 2; i < input.length(); i++) {
        if (input[i] < '0' || input[i] > '9') {
            return false;
        }
    }
    return true;
}

int extractNumericId(const string& tpStr) {
    int num = 0;
    for (size_t i = 2; i < tpStr.length(); i++) {
        num = num * 10 + (tpStr[i] - '0');
    }
    return num;
}

bool loadStudentFromCsv(const string& targetId, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false; 
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        size_t commaPos = line.find(',');
        string existingId = (commaPos != string::npos) ? line.substr(0, commaPos) : line;

        if (toUpperString(existingId) == "STUDENT_ID") {
            continue;
        }

        if (existingId == targetId) {
            file.close();
            return true; // Match found in CSV
        }
    }

    file.close();
    return false; 
}

bool saveStudentToCsv(const string& studentId, const string& filename) {

    ifstream checkFile(filename, ios::ate | ios::binary);
    bool isFileEmpty = true;
    bool needsLeadingNewline = false;

    if (checkFile.is_open()) {
        streampos fileSize = checkFile.tellg();
        if (fileSize > 0) {
            isFileEmpty = false;

            checkFile.seekg(-1, ios::end);
            char lastChar;
            checkFile.get(lastChar);

            if (lastChar != '\n' && lastChar != '\r') {
                needsLeadingNewline = true;
            }
        }
        checkFile.close();
    }

    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "[Error] Failed to open " << filename << " for writing." << endl;
        return false;
    }


    if (isFileEmpty) {
        file << "student_id\n";
    }
    else if (needsLeadingNewline) {
        file << "\n";
    }

    file << studentId << "\n";
    file.close();
    return true;
}

//Prompts
void registerStudentPrompt() {
    string inputId;

    cout << endl << "=============== Student Registration ===============" << endl;

    while (true) {
        cout << "Enter new Student ID (Format: TP000000, e.g., TP074123 or 0 to cancel): ";
        cin >> inputId;

        if (inputId == "0") {
            cout << "Registration cancelled." << endl;
            return;
        }

        inputId = toUpperString(inputId);

        if (!isValidStudentIdFormat(inputId)) {
            cout << "[Error] Invalid format! Student ID must be strictly 'TP' followed by 6 digits (e.g., TP074123)." << endl;
            continue;
        }

        if (loadStudentFromCsv(inputId)) {
            cout << "[Error] Duplicate ID! Student ID " << inputId << " is already registered." << endl;
            continue;
        }

        if (saveStudentToCsv(inputId)) {
            cout << "\n[Success] Student ID " << inputId << " successfully registered!" << endl;
        }
        break;
    }
}

int loginPrompt() {
    string inputId;

    while (true) {
        cout << endl << "=============== Student Login ===============" << endl;
        cout << "Enter your Student ID (Format: TP000000) (0 to cancel): ";
        cin >> inputId;

        if (inputId == "0") {
            return -1;
        }

        inputId = toUpperString(inputId);

        if (!isValidStudentIdFormat(inputId)) {
            cout << "[Error] Invalid format! Must be TP followed by 6 digits (e.g., TP074123)." << endl;
            continue;
        }

        if (!loadStudentFromCsv(inputId)) {
            cout << "[Error] Student ID " << inputId << " not found! Please register first." << endl;
            return -1;
        }

        cout << "\n[Login Successful] Welcome, " << inputId << "!" << endl;
        return extractNumericId(inputId);
    }
}

int studentAuthMenu() {
    int choice;
    while (true) {
        cout << endl << "=============== Student Portal ===============" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register Student ID" << endl;
        cout << "3. Back to Main Menu" << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int stuId = loginPrompt();
            if (stuId != -1) return stuId;
            break;
        }
        case 2:
            registerStudentPrompt();
            break;
        case 3:
            return -1;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}