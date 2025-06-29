#include "panel.h"
// #include <sorting.h>
#include <wx/stdpaths.h>

CPanel::CPanel(wxWindow* window, TItemPage* page)
    // : CXmlCodeManager(window, namePanel)
    : CBasePanel(window, page->name)
{
    mGrid = new wxGrid(this, NewControlId());
    mGrid->CreateGrid(0, 3, wxGrid::wxGridSelectRows);

    mGrid->SetColLabelValue(0, wxT("WebSite"));
    mGrid->SetColLabelValue(1, wxT("Login"));
    mGrid->SetColLabelValue(2, wxT("Password"));

    // Hide labels rows
    mGrid->SetRowLabelSize(0);

    wxBoxSizer* h_box = new wxBoxSizer(wxHORIZONTAL);
    h_box->Add(mGrid, 1, wxEXPAND);
    this->SetSizerAndFit(h_box);

    for(auto& itemWebSite : page->listWebSize)
        SetItemRow(itemWebSite);

    mGrid->AutoSizeColumns();
}

CPanel::~CPanel()
{
}

void CPanel::GetSizeCell(wxArrayString& array, int count, int number, int (wxGrid::*ifunc)(int) const)
{
    wxString t_string;
    for(int i = 0; i < count; i++)
        t_string.append(wxString::Format(wxT("%d;"), (mGrid->*ifunc)(i)));

    array.push_back(m_listParamsXml[number]);
    array.push_back(t_string);
}

void CPanel::GetValue(wxArrayString& arrayString)
{
    int countRow = mGrid->GetNumberRows();
    int countCol = mGrid->GetNumberCols();

    arrayString.push_back(m_listParamsXml[NAMEPAGE]);
    arrayString.push_back(mNamePage);

    GetSizeCell(arrayString, countCol, WIDTHCOL, &wxGrid::GetColSize);
    GetSizeCell(arrayString, countRow, HEIGHTROW, &wxGrid::GetRowSize);

    for(int row = 0; row < countRow; row++) {
        arrayString.push_back(m_listParamsXml[WEB]);
        for(int col = 0; col < countCol; col++) {
            arrayString.push_back(mGrid->GetColLabelValue(col));
            arrayString.push_back(mGrid->GetCellValue(row, col));
        }
    }
}

bool CPanel::SetItemRow(TItemWebSite& itemWebSite)
{
    int nRows = mGrid->GetNumberRows();

    mGrid->AppendRows();

    mGrid->SetCellValue(nRows, 0, itemWebSite.webSize);
    mGrid->SetCellValue(nRows, 1, itemWebSite.login);
    mGrid->SetCellValue(nRows, 2, itemWebSite.password);

    mGrid->AutoSizeColumns();

    return true;
}

void CPanel::GetListValue(ListWebSite& listWebSite)
{
    int nRows = mGrid->GetNumberRows();

    for(int iRows = 0; iRows < nRows; iRows++) {

        TItemWebSite itemWebSite;
        itemWebSite.webSize = mGrid->GetCellValue(iRows, 0);
        itemWebSite.login = mGrid->GetCellValue(iRows, 1);
        itemWebSite.password = mGrid->GetCellValue(iRows, 2);

        listWebSite.push_back(itemWebSite);
    }
}

void CPanel::SetValue(const wxArrayString& arrayString, int countAttribute)
{
    mGrid->AppendRows();

    int countRow = mGrid->GetNumberRows() - 1;
    int col = 0;

    while(mGrid->GetNumberCols() < countAttribute)
        mGrid->AppendCols();

    for(auto itr = arrayString.begin(); itr != arrayString.end(); ++itr) {
        mGrid->SetColLabelValue(col, *itr);
        mGrid->SetCellValue(countRow, col++, *(++itr));
    }
}

bool CPanel::GetSizeAllColumn(wxArrayInt& ArraySizeCol)
{
    mGrid->AutoSize();

    int countCol = mGrid->GetNumberCols();
    for(int col = 0; col < countCol; col++)
        ArraySizeCol.push_back(mGrid->GetColSize(col));

    return true;
}

bool CPanel::SetSizeAllColumn(const wxArrayInt& ArraySizeCol)
{
    int size = ArraySizeCol.size();
    for(int i = 0; i < size; i++)
        mGrid->SetColSize(i, ArraySizeCol[i]);

    int width = 0;
    int height = 0;

    int countRow = mGrid->GetNumberRows();
    int countCol = mGrid->GetNumberCols();

    for(int col = 0; col < countCol; col++)
        height += mGrid->GetColSize(col);

    for(int row = 0; row < countRow; row++)
        width += mGrid->GetRowSize(row);

    this->SetMinSize(wxSize(height, width));

    return true;
}

bool CPanel::AddNewItem()
{
    return mGrid->AppendRows();
}

bool CPanel::DeleteItem()
{
    if(mGrid->GetNumberRows() != 0) {
        wxArrayInt arrayNumberRows = mGrid->GetSelectedRows();
        if(arrayNumberRows.empty() == false) {

            wxString message;
            wxString caption;
            int size = arrayNumberRows.size();

            if(size == 1) {
                wxString t_str = mGrid->GetRowLabelValue(arrayNumberRows.front());
                message = wxString::Format(wxT("Delete item %S?"), t_str);
                caption = wxT("Delete item table");
            } else {
                // BubbleSort(&arrayNumberRows[0], size - 1);
                message = wxT("Delete items table?");
                caption = wxT("Deletion items table");
            }

            wxMessageDialog dlg(this, message, caption, wxOK | wxCANCEL);

            if(dlg.ShowModal() == wxID_OK)
                for(auto& p : arrayNumberRows)
                    mGrid->DeleteRows(p);
        } else
            wxMessageBox(wxT("Selected item from table for delete"));
    } else
        wxMessageBox(wxT("No item for delete"));

    return true;
}

bool CPanel::ClearTable()
{
    int nRows = mGrid->GetNumberRows();
    if(nRows > 0)
        mGrid->DeleteRows(0, nRows);

    return true;
}