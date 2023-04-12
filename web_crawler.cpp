#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <vector>

typedef struct {
    std::string body;
    std::vector<std::string> urls;
} FetchResult;

FetchResult fetch(std::string url) {
    std::unordered_map<std::string, FetchResult> fake {
        {"http://golang.org/", {
            "The Go Programming Language",
            {
                "http://golang.org/pkg/",
			    "http://golang.org/cmd/"
            }
        }}
    };

    usleep(500000);
    return fake.at(url);
}

int main() {
    FetchResult res = fetch("http://golang.org/");
    std::cout << "body: " << res.body << ", #urls: " << res.urls.size() << std::endl;
}