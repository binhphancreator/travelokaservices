#include "wx/wxprec.h"


#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"

#include "logo.xpm"

#define USE_EXECUTE

#ifdef USE_EXECUTE
#define EXEC(cmd) wxExecute((cmd), wxEXEC_SYNC)
#else
#define EXEC(cmd) system(cmd)
#endif



#ifndef __MAINFRAME_CPP__
#define __MAINFRAME_CPP__

#include "netthread.cpp"
#include "webapi.cpp"
#include "configdialog.cpp"

enum
{
    SERVICE_QUIT = wxID_EXIT,
    SERVICE_ABOUT = wxID_ABOUT,
    SERVICE_START_SERV,
    SERVICE_STOP_SERV,
    SERVICE_DETAIL,
    SERVICE_CLEAR_LOG,
    SERVICE_CONFIG,
    SERVICE_PING,
    SERVICE_OPENCHROME
};

class mainframe : public wxFrame{
    private:
        enviroment* env = new enviroment;
        router* route;
        logger* log;
    protected:
        wxTextCtrl* m_txtctrl;

    public:
        mainframe(const wxString &title) : wxFrame(NULL,wxID_ANY,title){
            
            //menubar

            wxMenuBar* menuBar = new wxMenuBar;

            wxMenu* menuLog = new wxMenu;
            menuLog->Append(SERVICE_CLEAR_LOG, "&Clear log\tCtrl-L");
            menuLog->AppendSeparator();
            menuLog->Append(SERVICE_QUIT, "E&xit\tAlt-X");
            menuBar->Append(menuLog, "&Log");

            wxMenu* menuService = new wxMenu;
            menuService->Append(SERVICE_OPENCHROME, "Open &Chrome");
            menuService->AppendSeparator();
            menuService->Append(SERVICE_CONFIG, "Config &service");
            menuService->AppendSeparator();
            menuService->Append(SERVICE_START_SERV, "&Start service\tCtrl-N");
            menuService->Append(SERVICE_STOP_SERV, "Stop &service");

            menuBar->Append(menuService, "&Service");

            wxMenu* menuHelp = new wxMenu;
            menuHelp->Append(SERVICE_DETAIL, "&Show detail");
            menuHelp->AppendSeparator();
            menuHelp->Append(SERVICE_ABOUT, "&About");
            menuBar->Append(menuHelp, "&Help");

            SetMenuBar(menuBar);

            wxTextCtrl* header = new wxTextCtrl(this, wxID_ANY, "",
                wxDefaultPosition, wxDefaultSize,
                wxTE_READONLY);
            DoLogLine(header, "  Time", " Detail");
            m_txtctrl = new wxTextCtrl(this, wxID_ANY, "",
                wxDefaultPosition, wxDefaultSize,
                wxTE_MULTILINE | wxTE_READONLY);

            // use fixed width font to align output in nice columns
            wxFont font(wxFontInfo().Family(wxFONTFAMILY_TELETYPE));
            header->SetFont(font);
            m_txtctrl->SetFont(font);

            m_txtctrl->SetFocus();

            // layout and show the frame
            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            sizer->Add(header, wxSizerFlags().Expand());
            sizer->Add(m_txtctrl, wxSizerFlags(1).Expand());
            SetSizer(sizer);

            SetIcon(wxIcon(logo_xpm));
            SetSize(600, 400);

            //connect event

            Connect(SERVICE_CLEAR_LOG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onClear));
            Connect(SERVICE_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onQuit));
            Connect(SERVICE_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onAbout));
            Connect(SERVICE_CONFIG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onConfig));
            Connect(SERVICE_START_SERV, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onStartService));
            Connect(SERVICE_STOP_SERV, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onStopService));
            Connect(SERVICE_OPENCHROME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onOpenChrome));

            //Router and Service
            log = new logger(m_txtctrl);
            route = new router(env, log);
            db = new database(env, log);

            //register API

            webapi::registerapi(route);

            //show frame

            Show();
        }

        ~mainframe(){
            Disconnect(SERVICE_CLEAR_LOG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onClear));
            Disconnect(SERVICE_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onQuit));
            Disconnect(SERVICE_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onAbout));
            Disconnect(SERVICE_CONFIG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onConfig));
            Disconnect(SERVICE_START_SERV, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onStartService));
            Disconnect(SERVICE_STOP_SERV, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onStopService));
            Disconnect(SERVICE_OPENCHROME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(mainframe::onOpenChrome));
        }


        void DoLogLine(wxTextCtrl* text,const wxString& timestr,const wxString& detailstr)
        {
            text->AppendText(wxString::Format("%9s %10s", timestr, detailstr));
        }

        void onClear(wxCommandEvent& WXUNUSED(event))
        {
            m_txtctrl->Clear();
        }

        void onQuit(wxCommandEvent& WXUNUSED(event)) {
            Close(true);
        }

        void onAbout(wxCommandEvent& WXUNUSED(event)) {
            wxMessageDialog dialog(this,
                "Traveloka service\n"
                "(c) Restful API WEB Service\n"
                "(c) 2021 Phan Van Binh\n",
                "About traveloka service",
                wxOK | wxICON_INFORMATION);


            dialog.ShowModal();
        }

        void onConfig(wxCommandEvent& WXUNUSED(event)) {
            new configdialong(this,env,log);
        }

        void onStartService(wxCommandEvent& WXUNUSED(event)) {
            if (!db->get_status()) {
                log->logRecord("Error connect Database.Please reconfig database before starting service");
                return;
            }
            if (route->isRunning()) {
                log->logRecord("Service is running.Starting failed");
                return;
            }

            new netthread(route);
            wxLaunchDefaultBrowser(wxString::Format("http://%s:%s", env->get("host"), env->get("port")));
        }

        void onStopService(wxCommandEvent& WXUNUSED(event)) {
            route->stop();
            route->close();
        }

        void onOpenChrome(wxCommandEvent& WXUNUSED(event)) {
            wxLaunchDefaultBrowser(wxString::Format("http://%s:%s", env->get("host"), env->get("port")));
        }
};

#endif