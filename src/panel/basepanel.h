#ifndef CBASEPANEL_H
#define CBASEPANEL_H

#include "wx/wx.h"
#include "xmlparse_template.h"

#include "ctrl/maindata.h"

typedef std::map<int, wxString> ListParamsXml;

class CBasePanel : public wxPanel
{
protected:
    enum ParamsXml { WEB = 0, PAGE, NAMEPAGE, WIDTHCOL, HEIGHTROW, NOT_FOUND, NEXT, END_PARAMS };

    ListParamsXml m_listParamsXml;

    virtual void GetListValue(ListWebSite& listWebSite) = 0;

public:
    wxString mNamePage;

    CBasePanel(wxWindow* window, wxString namePage);
    ~CBasePanel();
};

#endif // CBASEPANEL_H