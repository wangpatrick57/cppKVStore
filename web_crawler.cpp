#include "web_crawler.hpp"

void serial_crawl(std::string start_url, std::unordered_map<std::string, bool>& fetched) {
    bool already_fetched = fetched.find(start_url) != fetched.end() && fetched.at(start_url);
    fetched[start_url] = true;

    if (already_fetched) {
        return;
    }

    try {
        FetchResult res = fetch(start_url);
        std::cout << "fetched body " << res.body << std::endl;

        for (std::string url : res.urls) {
            serial_crawl(url, fetched);
        }
    } catch (const std::out_of_range& e) {
        std::cout << "didn't find " << start_url << std::endl;
    }
}

FetchResult fetch(std::string url) {
    std::unordered_map<std::string, FetchResult> fake {
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

    usleep(500000);
    return fake.at(url);
}

int main() {
    std::unordered_map<std::string, bool> fetched;
    serial_crawl("http://golang.org/", fetched);
}