#pragma once

#include <string>
#include <vector>

struct TItemWebSite {
    std::string webSize;
    std::string login;
    std::string password;
};

typedef std::vector<TItemWebSite> ListWebSite;

struct TItemPage {
    std::string name;
    ListWebSite listWebSize;
};

typedef std::vector<TItemPage> ListPage;

class CMainData
{
public:
    ListPage mList;

public:
    CMainData();
    ~CMainData();
};