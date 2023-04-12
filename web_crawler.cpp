#include "web_crawler.hpp"

std::unordered_map<std::string, FetchResult> const FAKE = {
    {"http://golang.org/", {
        "The Go Programming Language",
        {
            "http://golang.org/pkg/",
            "http://golang.org/cmd/"
        }
    }},
    {"http://golang.org/pkg/", {
        "Packages",
        {
            "http://golang.org/",
            "http://golang.org/cmd/",
            "http://golang.org/pkg/fmt/",
            "http://golang.org/pkg/os/"
        }
    }},
    {"http://golang.org/pkg/fmt/", {
        "Package fmt",
        {
            "http://golang.org/",
            "http://golang.org/pkg/"
        }
    }},
    {"http://golang.org/pkg/os/", {
        "Package os",
        {
            "http://golang.org/",
            "http://golang.org/pkg/"
        }
    }}
};

void mutex_crawl(std::string start_url, MutexFetched& mf) {
    mf.mu.lock();
    bool already_fetched = mf.fetched.find(start_url) != mf.fetched.end() && mf.fetched.at(start_url);
    mf.fetched[start_url] = true;
    mf.mu.unlock();

    if (already_fetched) {
        return;
    }

    try {
        FetchResult res = fetch(start_url);
        cout_mutex.lock();
        std::cout << "fetched body " << res.body << std::endl;
        cout_mutex.unlock();
        std::vector<std::thread> threads;

        for (std::string url : res.urls) {
            threads.emplace_back(mutex_crawl, url, std::ref(mf));
        }

        for (std::thread& t : threads) {
            t.join();
        }
    } catch (const std::out_of_range& e) {
        cout_mutex.lock();
        std::cout << "didn't find " << start_url << std::endl;
        cout_mutex.unlock();
    }
}

void serial_crawl(std::string start_url, MutexFetched& mf) {
    bool already_fetched = mf.fetched.find(start_url) != mf.fetched.end() && mf.fetched.at(start_url);
    mf.fetched[start_url] = true;

    if (already_fetched) {
        return;
    }

    try {
        FetchResult res = fetch(start_url);
        cout_mutex.lock();
        std::cout << "fetched body " << res.body << std::endl;
        cout_mutex.unlock();

        for (std::string url : res.urls) {
            serial_crawl(url, mf);
        }
    } catch (const std::out_of_range& e) {
        cout_mutex.lock();
        std::cout << "didn't find " << start_url << std::endl;
        cout_mutex.unlock();
    }
}

FetchResult fetch(std::string url) {
    usleep(500000);
    return FAKE.at(url);
}

int main() {
    MutexFetched mf{};
    mutex_crawl("http://golang.org/", mf);
}