#ifndef WEB_CRAWLER_HPP
#define WEB_CRAWLER_HPP

#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <thread>

typedef struct {
    std::string body;
    std::vector<std::string> urls;
} FetchResult;

typedef struct {
    std::mutex mu;
    std::unordered_map<std::string, bool> fetched;
} MutexFetched;

typedef struct {
    int a;
    bool b;
} Test;

extern std::unordered_map<std::string, FetchResult> const FAKE;
std::mutex cout_mutex{};

void mutex_crawl(std::string start_url, MutexFetched& mf);
void serial_crawl(std::string start_url, MutexFetched& mf);
FetchResult fetch(std::string url);

#endif