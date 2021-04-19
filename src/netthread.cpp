#include "router.cpp"
DEFINE_EVENT_TYPE(wxEVT_MYTHREAD)
class netthread : public wxThread {
private:
	router* route;

	void* Entry() {
		wxCommandEvent evt(wxEVT_MYTHREAD, GetId());
		route->config();
		route->listen();
		return 0;
	}
protected:
	
public:
	netthread(router* route) : wxThread(wxTHREAD_DETACHED) {
		this->route = route;

		if (wxTHREAD_NO_ERROR == Create()) {
			Run();
		}
	}
};