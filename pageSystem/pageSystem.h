#ifndef PAGESYSTEM_H
#define PAGESYSTEM_H
#include <vector>
#include <filesystem>
namespace pageSystem {
    using directoryFiles_t = std::vector<std::pair<std::string, std::filesystem::directory_entry>>;
    class PageInfo {
        public:
            PageInfo(int total, int itemsPage, int curr);
            int totalPages;
            int itemsPerPage;
            int currentPage;

            static directoryFiles_t getPageItems(const directoryFiles_t& files);

    };
}

#endif
