#include "Task.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "Database.h"
#include <stdio.h>
#include <sql.h>


static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}



void Task::SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename) {

	std::ofstream ostream(filename); // open a stream to the file
	ostream << tasks.size(); // write the number of tasks to the file
	for (const Task& task : tasks) { // loop over all the tasks
		std::string description = task.description;  // pull the description 
		std::replace(description.begin(), description.end(), ' ', '_'); // optimization for reading, replacing whitespaces with underscores forces the task to be read as a single string instead of multiple strings
		ostream << '\n' << description << ' ' << task.done; // write the line to the file
	}

}



std::vector<Task> Task::LoadTasksFromFile(const std::string& filename) {

	// If we don't have a file, return an empty vector of tasks
	if (!std::filesystem::exists(filename)) {
		return std::vector<Task>();
	}

	std::vector<Task> tasks;
	std::ifstream istream(filename);
	int numberOfTasks;
	istream >> numberOfTasks; // dump the first line

	// dump subsequent lines
	for (int i = 0; i < numberOfTasks; i++) {
		std::string description;
		bool done;
		istream >> description >> done; // will break on whitespace
		std::replace(description.begin(), description.end(), '_', ' ');
		tasks.push_back(Task{ description, done });
	}

	return tasks;


}

