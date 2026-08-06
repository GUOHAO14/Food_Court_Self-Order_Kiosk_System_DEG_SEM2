#pragma once

// String & Format Helpers 
bool stringsEqual(const char* s1, const char* s2);
void toUpperString(char* str);
bool isValidStudentIdFormat(const char* input);
int extractNumericId(const char* tpStr);
bool loadStudentFromCsv(const char* targetId, const char* filename = "students.csv");
bool saveStudentToCsv(const char* studentId, const char* filename = "students.csv");

// Kiosk Auth Prompts
void registerStudentPrompt();
int loginPrompt();
int studentAuthMenu();