#pragma once
#include <string>

// String & Format Helpers
std::string toUpperString(std::string str);
bool isValidStudentIdFormat(const std::string& input);
int extractNumericId(const std::string& tpStr);

bool loadStudentFromCsv(const std::string& targetId, const std::string& filename = "students.csv");
bool saveStudentToCsv(const std::string& studentId, const std::string& filename = "students.csv");

// Kiosk Auth Prompts
void registerStudentPrompt();
int loginPrompt();
int studentAuthMenu();