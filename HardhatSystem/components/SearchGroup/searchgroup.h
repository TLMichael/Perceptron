#ifndef SEARCHGROUP_SEARCHROUP_H_
#define SEARCHGROUP_SEARCHROUP_H_

// SearchGroup lib import
#include "search.h"
#include "videolistmodel.h"
#include "cvsearch.h"

#define SEARCHGROUP_INITIALIZA \
    SEARCH_INITIALIZA;  \
    VIDEOLISTMODEL_INITIALIZA;  \
    CVSEARCH_INITIALIZA;


#endif//SEARCHGROUP_SEARCHROUP_H_
