#include "settingspage.h"

CSettingsPage::CSettingsPage(wxWindow* parent)
    : wxDialog(parent,
          NewControlId(),
          wxT("Settings page"),
          wxDefaultPosition,
          wxDefaultSize,
          wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE)
{
    // const char* label[] = { "WebSite", "Login", "Password", "" };
    // const int sizeLabel = sizeof(label) / sizeof(label[0]);

    this->SetBackgroundColour(*wxWHITE);

    wxTextCtrl* txtNamePage =
        new wxTextCtrl(this, NewControlId(), wxT(""), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);

    wxBoxSizer* main_box = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* st_box = new wxStaticBoxSizer(wxHORIZONTAL, this, wxT("Name page"));
    st_box->Add(txtNamePage);

    main_box->Add(st_box, 0, wxEXPAND | wxALL, 10);
    main_box->Add(new ScrolledWidgetsPane(this), 1, wxEXPAND);

    SetSizerAndFit(main_box);
}

CSettingsPage::~CSettingsPage()
{
}