#pragma once
#include <string>
#include <vector>

class Task {
public:
	std::string description;
	bool done;

	static void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename);
	static std::vector<Task> LoadTasksFromFile(const std::string& filename);
	static void SaveTaskToDatabase(const std::vector<Task>& task, int position, sqlite3* db);

};


