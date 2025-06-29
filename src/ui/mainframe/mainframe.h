#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include "elements/textctrl.h"
#include <wx/frame.h>
#include <wx/listbook.h>

#include "ctrl/secure.h"

class CMainFrame : public wxFrame
{
private:
    CTextCtrl* m_txtFind;
    wxListbook* m_choicebook;

    CMainData mData;
    CSecure mSecure;

    wxTextCtrl* m_get_key;

    std::vector<std::vector<std::string>> mTableKeys;

    /**
     * @brief Шаблон функция для получения класса СPanel и выполнения её функции
     * @param numPage - номер страницы в wxListbook
     * @param func - функция выполнения
     * @param error - значение возвращаемое в случае ошибки
     * @param param - параметры передаваемы в функцию
     * @return перееменную функции
     */
    template <typename TReturn, typename TClass, typename... TParam>
    TReturn GetClassMethod(int numPage, TReturn (TClass::*func)(TParam...), TReturn error, TParam&&... param);

    /**
     * @brief Выравнивает столбцы на всех страницах wxListbook по максимальным размерам колнки
     */
    void ResizePageInListbook();

    /**
     * @brief Создание Menu
     */
    void CreateMenuBar();

    /**
     * @brief Загрузка xml - файла
     * @param event
     */
    void OnLoad(const wxCommandEvent& event);

    /**
     * @brief Сохранение xml - файла
     * @param event
     */
    void OnUpload(wxCommandEvent& event);

    /**
     * @brief Добавление нового элемента на страницу в таблицу
     * @param event
     */
    void OnAddNewItem(wxCommandEvent& event);

    /**
     * @brief Удаление элемента на странице из таблицы
     * @param event
     */
    void OnDeleteItem(wxCommandEvent& event);

    /**
     * @brief Добавление новой страницы wxListbook
     * @param event
     */
    void OnAddPage(wxCommandEvent& event);

    /**
     * @brief Удаление страницы wxListBook
     * @param event
     */
    void OnDeletePage(wxCommandEvent& event);

    void OnSetKey(wxCommandEvent& event);

    void OnSetFocus(wxFocusEvent& event);

    void OnKillFocus(wxFocusEvent& event);

    void FindKey(const wxString& value, const size_t& size);

public:
    CMainFrame();
    ~CMainFrame();
};

#endif // CMAINFRAME_H