#ifndef WEB_CRAWLER_HPP
#define WEB_CRAWLER_HPP

#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <vector>

typedef struct {
    std::string body;
    std::vector<std::string> urls;
} FetchResult;

void serial_crawl(std::string start_url);
FetchResult fetch(std::string url);

#endif