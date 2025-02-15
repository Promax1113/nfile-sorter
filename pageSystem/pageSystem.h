#ifndef PAGESYSTEM_H
#define PAGESYSTEM_H
#include <filesystem>
#include <vector>
namespace PageSystem {
using directoryFiles_t =
    std::vector<std::string>;
class PageInfo {
public:
  int totalPages;
  int itemsPerPage;
  int currentPage;

  PageInfo(int perPage, int current);
  directoryFiles_t getPageItems(const directoryFiles_t &files);
};
} // namespace PageSystem

#endif
