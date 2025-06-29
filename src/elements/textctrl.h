#pragma once

#include <wx/textctrl.h>

#include <functional>

class CTextCtrl : public wxTextCtrl
{
public:
    using CallbackType = std::function<void(const wxString&, const size_t&)>;

private:
    size_t mCountChar = 0;

    wxString mLabelBackground;

    CallbackType mFunction;

    /**
     * @brief Функция инициализации
     * @param background_label - наименование поисковика
     */
    void Initial(const wxString& labelBackground);

    /**
     * @brief Удаление наименования поиска в wxTextCtrl
     * @param event
     */
    void OnSetFocused(wxFocusEvent& event);

    /**
     * @brief Вставка наименования поиска в wxTextCtrl, если он пустой
     * @param event
     */
    void OnKillFocused(wxFocusEvent& event);

    void OnSetChar(wxCommandEvent& event);

public:
    CTextCtrl(wxWindow* parent, const wxString& labelBackground);

    CTextCtrl(wxWindow* parent, const wxString& labelBackground, long style);

    ~CTextCtrl() final = default;

    void SetCallBack(const CallbackType& function);
};