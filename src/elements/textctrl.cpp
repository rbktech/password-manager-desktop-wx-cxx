#include "textctrl.h"

#define COLOR_SET_FOCUS wxColor(0, 0, 0)
#define COLOR_KILL_FOCUS wxColor(128, 128, 128)

CTextCtrl::CTextCtrl(wxWindow* parent, const wxString& labelBackground)
    : wxTextCtrl(parent, NewControlId())
{
    Initial(labelBackground);
}

CTextCtrl::CTextCtrl(wxWindow* parent, const wxString& labelBackground, long style)
    : wxTextCtrl(parent, NewControlId(), wxT(""), wxDefaultPosition, wxDefaultSize, style)
{
    Initial(labelBackground);
}

void CTextCtrl::Initial(const wxString& labelBackground)
{
    mLabelBackground = labelBackground;

    this->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(CTextCtrl::OnSetFocused), nullptr, nullptr);
    this->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(CTextCtrl::OnKillFocused), nullptr, nullptr);
}

void CTextCtrl::SetCallBack(const CallbackType& function)
{
    mFunction = function;
}

void CTextCtrl::OnSetChar(wxCommandEvent& event)
{
    wxString value = event.GetString();

    mCountChar = value.size();

    if(mFunction != nullptr) {
        mFunction(value, mCountChar);
    }

    event.Skip();
}

void CTextCtrl::OnSetFocused(wxFocusEvent& event)
{
    if(mCountChar == 0) {

        this->Clear();
        this->SetForegroundColour(COLOR_SET_FOCUS);
        this->Bind(wxEVT_TEXT, &CTextCtrl::OnSetChar, this, this->GetId());
    }

    event.Skip();
}

void CTextCtrl::OnKillFocused(wxFocusEvent& event)
{
    if(mCountChar == 0) {

        this->Unbind(wxEVT_TEXT, &CTextCtrl::OnSetChar, this, this->GetId());
        this->SetForegroundColour(COLOR_KILL_FOCUS);
        this->SetValue(mLabelBackground);
    }

    event.Skip();
}