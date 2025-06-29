#ifndef CSETTINGSPAGE_H
#define CSETTINGSPAGE_H

#include "elements/textctrl.h"
#include <map>
#include <wx/wx.h>

class ScrolledWidgetsPane : public wxScrolledWindow
{
private:
    typedef std::pair<wxStaticBitmap*, CTextCtrl*> ItemListLabel;
    typedef std::map<int, ItemListLabel> ListBtmLabel;
    typedef std::map<int, ItemListLabel> ListTxtLabel;

    ListBtmLabel m_btmList;
    ListTxtLabel m_txtList;

    wxFlexGridSizer* m_grid;

    bool m_empty;

    int t = 0;

public:
    ScrolledWidgetsPane(wxWindow* parent)
        : wxScrolledWindow(parent, NewControlId())
    {
        m_empty = false;
        m_grid = new wxFlexGridSizer(2, 10, 10);

        AppendLabel();

        SetScrollRate(5, 5);
        this->SetSizer(m_grid);
        this->SetMinSize(wxSize(300, 400));
    }

    void AppendLabel()
    {
        wxStaticBitmap* btm = new wxStaticBitmap(this, NewControlId(), wxICON(), wxDefaultPosition, wxSize(10, 10));
        CTextCtrl* txt = new CTextCtrl(this, wxT("Add name"), wxBORDER_NONE);
        // txt->SetCallBack([]() { &ScrolledWidgetsPane::OnSetFocusAddLabel; });

        m_grid->Add(btm, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
        m_grid->Add(txt, 0, wxALL, 10);
        Layout();

        ItemListLabel item = std::make_pair(btm, txt);
        m_btmList.insert(std::make_pair(btm->GetId(), item));
        m_txtList.insert(std::make_pair(txt->GetId(), item));

        txt->Bind(wxEVT_LEFT_DOWN, &ScrolledWidgetsPane::OnSetFocusAddLabel, this);
        txt->Bind(wxEVT_KILL_FOCUS, &ScrolledWidgetsPane::OnKillFocusDeleteLabel, this);
        // txt->SetBind();
    }

    void DeleteLabel(ItemListLabel*& item, int id_btm, int id_txt)
    {
        // НЕ УДАЛЯЕТ!!! НАЙТИ СПОСОБ УДАЛЯТЬ!!!
        m_grid->Hide(item->first);
        m_grid->Hide(item->second);
        // this->RemoveChild(item->first);
        // this->RemoveChild(item->second);

        m_btmList.erase(id_btm);
        m_txtList.erase(id_txt);

        Layout();
    }

    // void OnSetFocusAddLabel()
    void OnSetFocusAddLabel(wxMouseEvent& event)
    {
        event.Skip();

        wxStaticBitmap* btm = m_txtList[event.GetId()].first;

        auto itr = std::prev(m_btmList.end());
        // auto itr = m_btmList.rbegin();
        if(btm == itr->second.first) {

            // wxWidgets не удаляет внутренние элементы
            // Ограничение, чтобы не создавалось как можно больше не нужных объектов

            // std::cout << itr->second.second->GetValue() << std::endl;
            // if((--itr)->second.second->GetLastPosition() != 0)

            // if(m_btmList.size() != 1 && m_btmList.size() != 1)
            //    if((++itr)->second.second->GetLastPosition() == 0)
            //        return;

            btm->Bind(wxEVT_LEFT_DOWN, &ScrolledWidgetsPane::OnDeleteItem, this);
            btm->SetIcon(wxICON(cross));

            AppendLabel();
        }
    }

    void OnKillFocusDeleteLabel(wxFocusEvent& event)
    {
        int id_txt = event.GetId();
        ItemListLabel* item = &m_txtList[id_txt];

        if(item->first != nullptr && item->second != nullptr) {

            int id_btm = item->first->GetId();
            if(item->second->GetLastPosition() == 0)
                DeleteLabel(item, id_btm, id_txt);
        }

        event.Skip();
    }

    void OnDeleteItem(wxMouseEvent& event)
    {
        int id_btm = event.GetId();
        ItemListLabel* item = &m_btmList[id_btm];

        if(item->first != nullptr && item->second != nullptr) {

            int id_txt = item->second->GetId();
            DeleteLabel(item, id_btm, id_txt);
        }
    }
};

class CSettingsPage : public wxDialog
{
private:
    std::vector<wxTextCtrl*> m_list;

    void OnDeleteItem(wxMouseEvent& event);

public:
    wxString GetValue()
    {
        return wxT("");
    }

    CSettingsPage(wxWindow* parent);
    ~CSettingsPage();
};

#endif // CSETTINGSPAGE_H