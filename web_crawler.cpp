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

bool test_and_set(std::string start_url, MutexFetched& mf) {
    mf.mu.lock();
    bool already_fetched = mf.fetched.find(start_url) != mf.fetched.end() && mf.fetched.at(start_url);
    mf.fetched[start_url] = true;
    mf.mu.unlock();
    return already_fetched;
}

void mutex_condvar_crawl(std::string start_url, MutexFetched& mf) {
}

void mutex_join_crawl(std::string start_url, MutexFetched& mf) {
    if (test_and_set(start_url, mf)) {
        return;
    }

    try {
        FetchResult res = fetch(start_url);

        {
            std::lock_guard<std::mutex> lk{cout_mutex};
            std::cout << "found body " << res.body << " from " << start_url << std::endl;
        }

        std::vector<std::thread> threads;

        for (std::string url : res.urls) {
            std::thread t{mutex_join_crawl, url, std::ref(mf)};
            threads.push_back(std::move(t));
        }

        for (std::thread& t : threads) {
            t.join();
        }
    } catch (const std::out_of_range& e) {
        {
            std::lock_guard<std::mutex> lk{cout_mutex};
            std::cout << "didn't find " << start_url << std::endl;
        }
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
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return FAKE.at(url);
}

int main() {
    MutexFetched mf{};
    mutex_join_crawl("http://golang.org/", mf);
}