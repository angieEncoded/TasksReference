#include "MainFrame.h"
#include <wx/wx.h>
#include <vector>
#include <string>
#include "Task.h"
#include "sqlite3.h"
#include "Database.h"
Database db;


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title){
	db.OpenDatabaseConnection();
	CreateControls();
	BindEventHandlers();
	AddSaveTasks();
}

void MainFrame::CreateControls() {
	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	// Create the new parent panel
	panel = new wxPanel(this);
	panel->SetFont(mainFont);
	
	// control spans the width of the frame and the text is centered in the middle
	headline = new wxStaticText(panel, wxID_ANY, "To do list", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	headline->SetFont(headlineFont);

	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 80), wxSize(495, 35), wxTE_PROCESS_ENTER); // wxTE_PROCESS_ENTER is input style
	addTaskButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
	tasksDisplay = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	clearTasks = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100, 525), wxSize(100, 35));
}

void MainFrame::BindEventHandlers() {
	addTaskButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	tasksDisplay->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
	clearTasks->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
}

void MainFrame::AddSaveTasks() {
	std::vector<Task> tasks = Task::LoadTasksFromFile("tasks.txt");
	for (const Task& task : tasks) {
		int index = tasksDisplay->GetCount();
		tasksDisplay->Insert(task.description, index);
		tasksDisplay->Check(index, task.done);
	}
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& evt) {
	AddTaskFromInput();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt) {
	AddTaskFromInput();
}

void MainFrame::OnListKeyDown(wxKeyEvent& evt) {
	switch (evt.GetKeyCode()) {
		case WXK_DELETE:
			DeleteSelectedTask();
			break;
		case WXK_UP:
			MoveSelectedTask(-1);
			break;
		case WXK_DOWN:
			MoveSelectedTask(+1);
			break;
	}
}

void MainFrame::OnClearButtonClicked(wxCommandEvent& evt) {
	if (tasksDisplay->IsEmpty()) {
		return;
	}
	wxMessageDialog dialog(this, "Are you sure you want to clear all tasks?", "Clear", wxYES_NO | wxCANCEL);
	int result = dialog.ShowModal();
	if (result == wxID_YES) {
		tasksDisplay->Clear();
	}
}

void MainFrame::OnWindowClosed(wxCloseEvent& evt) {
	std::vector<Task> tasks;
	for (unsigned int i = 0; i < tasksDisplay->GetCount(); i++) {
		Task task;
		task.description = tasksDisplay->GetString(i);
		task.done = tasksDisplay->IsChecked(i);
		tasks.push_back(task);
	}
	Task::SaveTasksToFile(tasks, "tasks.txt");
	db.CloseDatabaseConnection();
	evt.Skip();// so we dont get stuck on closing the window
	
}

void MainFrame::AddTaskFromInput() {
	wxString description = inputField->GetValue();
	if (!description.IsEmpty()) {
		tasksDisplay->Insert(description, tasksDisplay->GetCount());// make the new task appear at the bottom, use value 0 
		inputField->Clear();
	}
	inputField->SetFocus();
}

void MainFrame::DeleteSelectedTask() {
	int selectedIndex = tasksDisplay->GetSelection();
	if (selectedIndex == wxNOT_FOUND) {
		return;
	}
	tasksDisplay->Delete(selectedIndex);
}

void MainFrame::MoveSelectedTask(int offset) {

	int selectedIndex = tasksDisplay->GetSelection();
	if (selectedIndex == wxNOT_FOUND) {
		return;
	}
	unsigned int newIndex = selectedIndex + offset;
	if (newIndex >= 0 && newIndex < tasksDisplay->GetCount()) {
		SwapTasks(selectedIndex, newIndex);
		tasksDisplay->SetSelection(newIndex, true);
	}
	
}

void MainFrame::SwapTasks(int i, int j) {

	// Create the tasks
	Task taskI{ tasksDisplay->GetString(i).ToStdString(), tasksDisplay->IsChecked(i) }; // have to convert to a standard string from a wxWidgets string
	Task taskJ{ tasksDisplay->GetString(j).ToStdString(), tasksDisplay->IsChecked(j) };

	tasksDisplay->SetString(i, taskJ.description);
	tasksDisplay->Check(i, taskJ.done);

	tasksDisplay->SetString(j, taskI.description);
	tasksDisplay->Check(j, taskI.done);

}

