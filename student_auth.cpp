#include "student_auth.h"
#include <iostream>
#include <fstream>

using namespace std;

//Custom string comparison
bool stringsEqual(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) return false;
        i++;
    }
    return s1[i] == s2[i];
}

// In-place conversion to upper case
void toUpperString(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - ('a' - 'A');
        }
    }
}

// Validate starts with 'TP' followed by 6 digits
bool isValidStudentIdFormat(const char* input) {
    int len = 0;
    while (input[len] != '\0') len++;

    if (len != 8) return false;

    if ((input[0] != 'T' && input[0] != 't') || (input[1] != 'P' && input[1] != 'p')) {
        return false;
    }

    for (int i = 2; i < 8; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return false;
        }
    }
    return true;
}

int extractNumericId(const char* tpStr) {
    int num = 0;
    for (int i = 2; tpStr[i] != '\0'; i++) {
        if (tpStr[i] >= '0' && tpStr[i] <= '9') {
            num = num * 10 + (tpStr[i] - '0');
        }
    }
    return num;
}

bool loadStudentFromCsv(const char* targetId, const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    char line[256];
    while (file.getline(line, sizeof(line))) {
        if (line[0] == '\0') continue;

        // Parse first column up to comma or line endings
        char existingId[64];
        int i = 0;
        while (line[i] != '\0' && line[i] != ',' && line[i] != '\r' && line[i] != '\n') {
            existingId[i] = line[i];
            i++;
        }
        existingId[i] = '\0';

        // Check against CSV header line
        char upperExisting[64];
        int j = 0;
        for (; existingId[j] != '\0'; j++) {
            if (existingId[j] >= 'a' && existingId[j] <= 'z') {
                upperExisting[j] = existingId[j] - ('a' - 'A');
            }
            else {
                upperExisting[j] = existingId[j];
            }
        }
        upperExisting[j] = '\0';

        if (stringsEqual(upperExisting, "STUDENT_ID")) {
            continue;
        }

        if (stringsEqual(existingId, targetId)) {
            file.close();
            return true; // Match found in CSV
        }
    }

    file.close();
    return false;
}

bool saveStudentToCsv(const char* studentId, const char* filename) {
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

// Prompts
void registerStudentPrompt() {
    char inputId[64];

    cout << endl << "=============== Student Registration ===============" << endl;

    while (true) {
        cout << "Enter new Student ID (Format: TP000000, e.g., TP074123 or 0 to cancel): ";
        cin >> inputId;

        if (stringsEqual(inputId, "0")) {
            cout << "Registration cancelled." << endl;
            return;
        }

        toUpperString(inputId);

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
    char inputId[64];

    while (true) {
        cout << endl << "=============== Student Login ===============" << endl;
        cout << "Enter your Student ID (Format: TP000000) (0 to cancel): ";
        cin >> inputId;

        if (stringsEqual(inputId, "0")) {
            return -1;
        }

        toUpperString(inputId);

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