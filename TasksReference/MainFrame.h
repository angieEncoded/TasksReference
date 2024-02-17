#pragma once
#include <wx/wx.h>
#include "sqlite3.h"

class MainFrame : public wxFrame{

public:
	MainFrame(const wxString& title);

private:



	void CreateControls();
	void BindEventHandlers();
	void AddSaveTasks();

	
	void OnAddButtonClicked(wxCommandEvent& evt);
	void OnInputEnter(wxCommandEvent& evt);
	void OnListKeyDown(wxKeyEvent& evt);
	void OnClearButtonClicked(wxCommandEvent& evt);
	void OnWindowClosed(wxCloseEvent& evt);

	void AddTaskFromInput();
	void DeleteSelectedTask();
	void MoveSelectedTask(int offset);
	void SwapTasks(int i, int j);



	wxPanel* panel;
	wxStaticText* headline;
	wxTextCtrl* inputField;
	wxButton* addTaskButton;
	wxCheckListBox* tasksDisplay;
	wxButton* clearTasks;

};

