#include "App.h"
#include "MainFrame.h"
#include "sqlite3.h"
#include <stdio.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("To Do list");
	mainFrame->SetClientSize(800, 600);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}
