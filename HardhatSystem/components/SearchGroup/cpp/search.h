#ifndef GROUP_SEARCHGROUP_CPP_SEARCH_H_
#define GROUP_SEARCHGROUP_CPP_SEARCH_H_

// JQToolsLibrary import
#include <JQToolsLibrary>

#define VIDEO_INITIALIZA                                               \
{                                                                       \
    qmlRegisterType<Search::Manage>("Search", 1, 0, "SearchManage"); \
}

namespace Search
{

class Manage: public AbstractTool
{
    Q_OBJECT
    Q_DISABLE_COPY(Manage)

public:
    Manage() = default;

    ~Manage() = default;

public slots:

};

}

#endif//GROUP_SEARCHGROUP_CPP_SEARCH_H_
