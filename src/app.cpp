#ifndef __APP_CPP__
#define __APP_CPP__
#include "mainframe.cpp"


class application : public wxApp{
    public:
        application(){}
        ~application(){}
        bool OnInit(){
            if ( !wxApp::OnInit() )
            return false;

            new mainframe("Traveloka Service");

            return true;
        }
};

wxIMPLEMENT_APP(application);

#endif