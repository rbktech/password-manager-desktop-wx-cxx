#ifndef PANEL_H
#define PANEL_H

#include <wx/grid.h>

#include "panel/basepanel.h"
#include "xmlcodemanager.h"

class CPanel : public CBasePanel
{
private:
    wxGrid* mGrid;

    void GetSizeCell(wxArrayString& array, int count, int number, int (wxGrid::*ifunc)(int) const);

    virtual void SetValue(const wxArrayString& arrayString, int countAttribute) final;
    virtual void GetValue(wxArrayString& arrayString) final;

public:
    CPanel(wxWindow* window, TItemPage* page);
    ~CPanel();

    bool AddNewItem();
    bool DeleteItem();
    bool GetSizeAllColumn(wxArrayInt& ArraySizeCol);
    bool SetSizeAllColumn(const wxArrayInt& ArraySizeCol);

    bool ClearTable();

    bool SetItemRow(TItemWebSite& webSite);
    void GetListValue(ListWebSite& listWebSite) final;
};

#endif // PANEL_H