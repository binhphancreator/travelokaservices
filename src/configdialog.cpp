#ifndef CONFIGDIALOG_CPP
#define CONFIGDIALOG_CPP

#include <wx/dialog.h>
#include "enviroment.cpp"
#include "logger.cpp"
#include "models.cpp"

class configdialong : public wxDialog {
private:
	enviroment* env;
	logger* log;
protected:
	wxStaticText* labelHost;
	wxTextCtrl* m_textCtrHost;
	wxStaticText* labelPort;
	wxTextCtrl* m_textCtrPort;
	wxStaticText* labelDBHost;
	wxTextCtrl* m_textDBHost;
	wxStaticText* labelDBUser;
	wxTextCtrl* m_textDBUser;
	wxStaticText* labelDBPass;
	wxTextCtrl* m_textDBPass;
	wxStaticText* labelDBSchema;
	wxTextCtrl* m_textDBSchema;
	wxButton* btnApply;

public:
	configdialong(wxWindow* parent,enviroment* env,logger* log) : wxDialog(parent, wxID_ANY, "Config service") {
		this->env = env;
		this->log = log;

		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

		labelHost = new wxStaticText(this, wxID_ANY, wxT("Host"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(labelHost, 0, wxALL, 5);

		m_textCtrHost = new wxTextCtrl(this, wxID_ANY, env->get("host"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(m_textCtrHost, 0, wxALL | wxEXPAND, 5);

		labelPort = new wxStaticText(this, wxID_ANY, wxT("Port"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(labelPort, 0, wxALL, 5);

		m_textCtrPort = new wxTextCtrl(this, wxID_ANY, env->get("port"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(m_textCtrPort, 0, wxALL | wxEXPAND, 5);

		labelDBHost = new wxStaticText(this, wxID_ANY, wxT("DB HOST"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(labelDBHost, 0, wxALL, 5);

		m_textDBHost = new wxTextCtrl(this, wxID_ANY, env->get("db_host"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(m_textDBHost, 0, wxALL | wxEXPAND, 5);

		labelDBUser = new wxStaticText(this, wxID_ANY, wxT("DB USER"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(labelDBUser, 0, wxALL, 5);

		m_textDBUser = new wxTextCtrl(this, wxID_ANY, env->get("db_user"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(m_textDBUser, 0, wxALL | wxEXPAND, 5);

		labelDBPass = new wxStaticText(this, wxID_ANY, wxT("DB PASS"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(labelDBPass, 0, wxALL, 5);

		m_textDBPass = new wxTextCtrl(this, wxID_ANY, env->get("db_pass"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(m_textDBPass, 0, wxALL | wxEXPAND, 5);

		labelDBSchema = new wxStaticText(this, wxID_ANY, wxT("DB SCHEMA"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(labelDBSchema, 0, wxALL, 5);

		m_textDBSchema = new wxTextCtrl(this, wxID_ANY, env->get("db_schema"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(m_textDBSchema, 0, wxALL | wxEXPAND, 5);

		btnApply = new wxButton(this, wxID_ANY, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0);
		sizer->Add(btnApply, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
		SetSizer(sizer);
		SetSize(300, 500);

		//connect event

		Connect(btnApply->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(configdialong::onApply), NULL, this);

		Show();
	}

	~configdialong(){}

	void onApply(wxCommandEvent& event) {
		env ->set("host", m_textCtrHost->GetValue())
			->set("port", m_textCtrPort->GetValue())
			->set("db_host", m_textDBHost->GetValue())
			->set("db_user", m_textDBUser->GetValue())
			->set("db_pass", m_textDBPass->GetValue())
			->set("db_schema", m_textDBSchema->GetValue());

		log->logRecord("Config success");
		db = new database(env, log);
		Close();
	}

};

#endif // !CONFIGDIALOG_CPP