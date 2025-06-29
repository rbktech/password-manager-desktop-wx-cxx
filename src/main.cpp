#include "ui/mainframe/mainframe.h"

#include <wx/app.h>

class CApp : public wxApp
{
public:
    bool OnInit() override
    {
        return (new CMainFrame())->Show();
    }
};

IMPLEMENT_APP(CApp)