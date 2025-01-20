#include "pageSystem.h"
#include <algorithm>
namespace pageSystem{
    PageInfo::PageInfo(int total, int itemsPage, int curr){
        this->totalPages = total;
        this->itemsPerPage = itemsPage;
        this->currentPage = curr;
    }

    directoryFiles_t PageInfo::getPageItems(const directoryFiles_t& files) {
        int start = currentPage * itemsPerPage;
        /*
        this uses min since if there's 25 items in list and start = 20, end will become 30.
        min chooses the smaller number betwen the size or the calculated value to ensure its
        not accessing non-existent indexes.
        */
        int end = std::min(start + itemsPerPage, (int)files.size());
        return directoryFiles_t(files.begin() + start, files.begin() + end);
    }
}
