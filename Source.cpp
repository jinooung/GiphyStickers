#include <iostream>
#include <algorithm>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <json.hpp>

using namespace std; 
using json = nlohmann::json;

// Global variable declarations
vector <vector <json>> sticker_pages;
vector <json> sticker;

string APIKEY = "xFGHLDa9bRX5RX4Al9DGbIqtCpf6gQk7";
string address = "api.giphy.com";
string port = "80";
string hostAddress;

// Function declaration
json Giphy(string search, int limit, int offset);

int main() {
    // Offset to go to next page
    int offset = 0;

    // Limit shows how many stickers per page
    int limit = 1;

    // Sticker per page
    int stickers_per_page = 10;
    int page = 0;

    while (true) {
        // Enter search string
        string search = "";
        string reply = "";
        cout << "Enter search: ";
        cin >> search;

        cout << "Page " << page + 1 << endl;
        page++;
        // Add 10 sticker per page to vector
        for (int i = 0; i < stickers_per_page; i++) {
            json s = Giphy(search, limit, offset);
            cout << "Type: " <<  s.at("data")[0].at("type") << endl;
            cout << "Rating: " <<  s.at("data")[0].at("rating") << endl;
            cout << "Url: " << s.at("data")[0].at("url") << endl << endl;
            sticker.push_back(s);
            offset++;
        }

        // Prompt for user navigation
        cout << "Enter 'next' to grab next page, 'search' for new search, 'rank' to search ranks within results, or 'end' to terminate: ";
        cin >> reply;
        while (reply != "next" && reply != "search" && reply != "rank" && reply != "end") {
            cout << "Invalid response, please enter 'next', 'search', 'rank', or 'end': ";
            cin >> reply;
        }

        // Terminate if exit
        if (reply == "end") {
            return 0;
        }

        // When user enters 'next'
        while (reply == "next" || reply == "rank") {
             sticker_pages.push_back(sticker);
             sticker.clear();

             offset += stickers_per_page;

             if (reply == "rank") {
                 string rank;
                 int count = 0;
                 cout << "Please enter the rank/rating (ex. g, pg, pg-13, r): ";
                 cin >>  rank;
                 
                 for (int i = 0; i < sticker_pages.size(); i++) {
                     for (int j = 0; j < sticker_pages[i].size(); j++) {
                         if (sticker_pages[i][j].at("data")[0].at("rating") == rank) {
                             count++;
                         }
                     }
                 }

                 cout << "Total number of stickers with rating " + rank + ": " << count << endl;

             }
             else if (reply == "next") {
                 cout << "Page " << page + 1 << endl;
                 page++;
                 // Add 10 more stickers to next page to vector
                 for (int i = 0; i < stickers_per_page; i++) {
                     json s = Giphy(search, limit, offset);
                     cout << "Type: " << s.at("data")[0].at("type") << endl;
                     cout << "Rating: " << s.at("data")[0].at("rating") << endl;
                     cout << "Url: " << s.at("data")[0].at("url") << endl << endl;
                     sticker.push_back(Giphy(search, limit, offset));
                     offset++;
                 }
             }
             
             cout << "Enter 'next' to grab next page, 'search' for new search,'rank' to search ranks within results, or 'end' to terminate: ";
             cin >> reply;
             while (reply != "next" && reply != "search" && reply != "rank" && reply!= "end") {
                 cout << "Invalid response, please enter 'next', 'search', 'rank', or 'end': ";
                 cin >> reply;
             }

             // Terminate if exit
             if (reply == "end") {
                 return 0;
             }
        }

        if (reply == "search") {
                page = 0;
                sticker_pages.clear();
                sticker.clear();
        }
    }

    return 0;
}

json Giphy(string search, int limit, int offset) {
    asio::io_service io_service;

    // If port is not 80, then add :port
    if (port.compare("80") != 0) 
    {
        hostAddress = address + ":" + port;
    }
    else
    {
        hostAddress = address;
    }

    string queryStr = "/v1/stickers/search?api_key=" + APIKEY + "&q=" + search + "&limit=" + to_string(limit) + "&offset=" + to_string(offset);

    // Retrieves endpoints from server name
    asio::ip::tcp::resolver resolver(io_service);
    asio::ip::tcp::resolver::query query(address, port);
    asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Attempts each endpoint until an endpoint is connected
    // Connects to socket
    asio::ip::tcp::socket socket(io_service);
    asio::connect(socket, endpoint_iterator);

    // Form the request. 
    // We specify the "Connection: close" header so that the server will close the socket after transmitting the response. 
    // This will allow us to treat all data up until the EOF as the content.
    asio::streambuf request;
    ostream request_stream(&request);
    request_stream << "GET " << queryStr << " HTTP/1.1\r\n";  // note that you can change it if you wish to HTTP/1.0
    request_stream << "Host: " << hostAddress << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send request to socket
    asio::write(socket, request);

    // Read response
    asio::streambuf response;
    asio::read_until(socket, response, "\r\n");
    
    istream response_stream(&response);
    string http_version;
    string status_message;
    unsigned int status_code;

    response_stream >> http_version;
    response_stream >> status_code;
    getline(response_stream, status_message);

    // Error check for response
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
        cout << "Invalid response\n";
        exit(1);
    }
    if (status_code != 200)
    {
        cout << "Response returned with status code " << status_code << "\n";
        exit(1);
    }

    // Read response headers
    asio::read_until(socket, response, "\r\n\r\n");

    // Process the response headers.
    // Commented out for clearer console application readability
    std::string header;
    while (std::getline(response_stream, header) && header != "\r")
    {
       //cout << header << "\n";
    }

    //cout << "\n";

    // If there is a valid response, we convert the streambuf object into stringstream then into string
    // This allows us to access the JSON parser in the nholmann library
    ostringstream res_string;
    if (response.size() > 0)
    {
        res_string << &response;
    }

    // Read until EOF, writing data to output as we go.
    // Add onto current response stream to obtain complete JSON string
    asio::error_code error;
    while (asio::read(socket, response, asio::transfer_at_least(1), error))
    { 
        res_string << &response;
    }

    // Parse the JSON string
    json res_json;
    try {
        res_json = json::parse(res_string.str());
    }
    catch (json::parse_error& ex) {
        std::cout << "parse error at byte: " << ex.what() << endl;
    }

    // If error occurs outside of EOF
    if (error != asio::error::eof)
    {
        throw asio::system_error(error);
    }

    //cout << std::setw(4) << res_json.at("data")[0] << "\n\n";

    return res_json;
}