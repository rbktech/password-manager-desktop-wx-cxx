#include "mainframe.h"

#include "panel/panel.h"
#include "panel/settingspage.h"

#define WIDTH 1000
#define HEIGHT 700

CMainFrame::CMainFrame()
    : wxFrame(nullptr, wxID_ANY, wxT("Code Manager"))
{
    SetIcon(wxICON(lock));

    m_txtFind = new CTextCtrl(this, wxT("Find"));
    m_txtFind->SetCallBack([&](const wxString& value, const size_t& size){ FindKey(value, size); });

    m_choicebook = new wxListbook(this, NewControlId());

    m_get_key = new wxTextCtrl(this, NewControlId(), wxT(""), wxDefaultPosition, wxSize(500, 100), wxTE_MULTILINE | wxTE_READONLY);
    // m_get_key->LoadFile(wxT("KeyByAllTheDoor.xml"));

    CreateMenuBar();

    wxBoxSizer* mainBox = new wxBoxSizer(wxVERTICAL);
    mainBox->Add(m_txtFind, 0, wxEXPAND);
    mainBox->Add(m_get_key, 0, wxEXPAND);
    mainBox->Add(m_choicebook, 1, wxEXPAND);
    SetSizerAndFit(mainBox);

    int result = CFile::load("../keys.csv", mTableKeys);
    if(result == 0) {

        TItemPage page;

        TItemWebSite webSite;

        m_choicebook->AddPage(new CPanel(m_choicebook, &page), wxT("main"));

        for(auto p : mTableKeys) {

            webSite.webSize = p.at(0);
            webSite.login = p.at(1);
            webSite.password = p.at(2);

            GetClassMethod(m_choicebook->GetSelection(), &CPanel::SetItemRow, false, webSite);
        }
    }

    this->SetSize(wxSize(WIDTH, HEIGHT));
    // this->CallAfter(OnLoad, wxCommandEvent());
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::FindKey(const wxString& value, const size_t& size)
{
    bool find = false;
    TItemWebSite webSite;

    m_get_key->SetValue(value);

    GetClassMethod(m_choicebook->GetSelection(), &CPanel::ClearTable, false);

    for(auto& row : mTableKeys) {

        for(auto& col : row) {

            // find = (size > 0) ? (col.find(value) != wxNOT_FOUND ? true : false) : (true);
            // find = !(size > 0) || (col.find(value) != wxNOT_FOUND ? true : false);
            find = size <= 0 || col.find(value) != wxNOT_FOUND;
            if(find == true) {

                webSite.webSize = row.at(0);
                webSite.login = row.at(1);
                webSite.password = row.at(2);

                GetClassMethod(m_choicebook->GetSelection(), &CPanel::SetItemRow, false, webSite);

                break;
            }
        }
    }
}

void CMainFrame::OnLoad(const wxCommandEvent& event)
{
    mData.mList.clear();

    try {

        mSecure.loadFileAndDecrypt(&mData);

        m_choicebook->DeleteAllPages();
        for(auto& itemPage : mData.mList)
            m_choicebook->AddPage(new CPanel(m_choicebook, &itemPage), itemPage.name);

    } catch(const char* message) {
        printf(message);
    }
}

void CMainFrame::OnUpload(wxCommandEvent& event)
{
    mData.mList.clear();

    size_t nPage = m_choicebook->GetPageCount();
    for(int iPage = 0; iPage < nPage; iPage++) {

        ListWebSite listWebSite;

        CPanel* objPage = dynamic_cast<CPanel*>(m_choicebook->GetPage(iPage));
        if(objPage != nullptr) {
            objPage->GetListValue(listWebSite);

            TItemPage itemPage;
            itemPage.name = objPage->mNamePage;
            itemPage.listWebSize = listWebSite;

            mData.mList.push_back(itemPage);
        }
    }

    m_choicebook->DeleteAllPages();

    try {

        mSecure.encryptFileAndUpload(&mData);

        for(auto& itemPage : mData.mList)
            m_choicebook->AddPage(new CPanel(m_choicebook, &itemPage), itemPage.name);

    } catch(const char* message) {
        printf(message);
    }
}

template <typename TReturn, typename TClass, typename... TParam>
TReturn CMainFrame::GetClassMethod(int numPage, TReturn (TClass::*func)(TParam...), TReturn error, TParam&&... param)
{
    if(m_choicebook->GetPageCount() != 0) {
        wxWindow* windowPage = m_choicebook->GetPage(numPage);
        if(windowPage != nullptr) {
            TClass* panelPage = dynamic_cast<TClass*>(windowPage);
            if(panelPage != nullptr && func != nullptr)
                return (panelPage->*func)(param...);
            else
                return error;
        } else
            return error;
    } else
        return error;
}

void CMainFrame::OnAddNewItem(wxCommandEvent& event)
{
    GetClassMethod(m_choicebook->GetSelection(), &CPanel::AddNewItem, false);
}

void CMainFrame::ResizePageInListbook()
{
    wxArrayInt arraySizeCol;

    int pageCount = m_choicebook->GetPageCount();
    for(int page = 0; page < pageCount; page++) {

        wxArrayInt new_arraySizeCol;
        GetClassMethod(page, &CPanel::GetSizeAllColumn, false, new_arraySizeCol);

        if(new_arraySizeCol.size() == arraySizeCol.size()) {
            for(unsigned int i = 0; i < new_arraySizeCol.size(); i++)
                if(arraySizeCol[i] < new_arraySizeCol[i])
                    arraySizeCol[i] = new_arraySizeCol[i];
        } else
            arraySizeCol = new_arraySizeCol;
    }

    for(int page = 0; page < pageCount; page++)
        GetClassMethod(page, &CPanel::SetSizeAllColumn, false, *(const_cast<const wxArrayInt*>(&arraySizeCol)));
}

void CMainFrame::CreateMenuBar()
{
    wxMenuBar* menuBar = new wxMenuBar;

    wxMenu* file = new wxMenu;
    wxMenu* edit = new wxMenu;

    wxMenuItem* load = new wxMenuItem(file, NewControlId(), wxT("Load"));
    wxMenuItem* upload = new wxMenuItem(file, NewControlId(), wxT("Upload"));
    file->Append(load);
    file->Append(upload);

    wxMenuItem* addPage = new wxMenuItem(edit, NewControlId(), wxT("Add Page"));
    wxMenuItem* deletePage = new wxMenuItem(edit, NewControlId(), wxT("Delete Page"));
    wxMenuItem* addNewItem = new wxMenuItem(edit, NewControlId(), wxT("Add new item"));
    wxMenuItem* deleteItem = new wxMenuItem(edit, NewControlId(), wxT("Delete item"));

    edit->Append(addPage);
    edit->Append(addNewItem);
    edit->AppendSeparator();
    edit->Append(deletePage);
    edit->Append(deleteItem);

    menuBar->Append(file, wxT("File"));
    menuBar->Append(edit, wxT("Edit"));

    Bind(wxEVT_COMMAND_MENU_SELECTED, &CMainFrame::OnLoad, this, load->GetId());
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CMainFrame::OnUpload, this, upload->GetId());

    Bind(wxEVT_COMMAND_MENU_SELECTED, &CMainFrame::OnAddPage, this, addPage->GetId());
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CMainFrame::OnDeletePage, this, deletePage->GetId());
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CMainFrame::OnAddNewItem, this, addNewItem->GetId());
    Bind(wxEVT_COMMAND_MENU_SELECTED, &CMainFrame::OnDeleteItem, this, deleteItem->GetId());

    SetMenuBar(menuBar);
}

void CMainFrame::OnDeleteItem(wxCommandEvent& event)
{
    if(m_choicebook->GetPageCount() != 0) {
        int selected = m_choicebook->GetSelection();
        if(selected != -1)
            GetClassMethod(selected, &CPanel::DeleteItem, false);
        else
            wxMessageBox(wxT("Select item"));
    } else
        wxMessageBox(wxT("No table for delete item"));
}

void CMainFrame::OnAddPage(wxCommandEvent& event)
{
    wxTextEntryDialog dlg(this, wxT("Enter name page"), wxT("Name new page"));
    if(dlg.ShowModal() == wxID_OK) {
        wxString name = dlg.GetValue();

        TItemPage page;
        page.name = name.ToStdString();

        mData.mList.push_back(page);

        m_choicebook->AddPage(new CPanel(m_choicebook, &page), name);
    }
}

void CMainFrame::OnDeletePage(wxCommandEvent& event)
{
    if(m_choicebook->GetPageCount() != 0) {
        int selected = m_choicebook->GetSelection();
        if(selected != -1) {
            wxString namePage = m_choicebook->GetPageText(selected);
            wxMessageDialog dlg(this, wxT("Delete page ") + namePage, wxT("Delete page?"), wxOK | wxCANCEL);
            if(dlg.ShowModal() == wxID_OK)
                m_choicebook->DeletePage(selected);
        } else
            wxMessageBox(wxT("Select page"));
    } else
        wxMessageBox(wxT("No page for delete"));
}