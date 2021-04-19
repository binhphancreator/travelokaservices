#ifndef LOGGER_CPP
#define LOGGER_CPP

class logger {
private:
	wxTextCtrl* m_txtctrl;
public:
	logger(wxTextCtrl* m_txtctrl) {
		this->m_txtctrl = m_txtctrl;
	}

    void logLine(const wxString& timestr, const wxString& detailstr)
    {
        m_txtctrl->AppendText(wxString::Format("%9s %10s", timestr, detailstr));
    }

    void logRecord(const wxString& msg) {
        wxDateTime now = wxDateTime::Now();
        logLine
        (
            wxString::Format("%d:%d:%d", now.GetTm().hour, now.GetTm().min, now.GetTm().sec),
            msg + "\n"
        );
    }
};

#endif // !LOGGER_CPP
