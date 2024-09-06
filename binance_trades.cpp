#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <json/json.h>
#include <chrono>
#include <cstdlib>
#include <memory> // Include for std::unique_ptr

using namespace std;

const string BASE_URL = "https://fapi.binance.com";
const string AGG_TRADES_ENDPOINT = "/fapi/v1/aggTrades";

struct Trade {
    uint64_t tradeId;  // Changed to uint64_t to match large values
    string price;
    string quantity;
    uint64_t firstTradeId;  // Changed to uint64_t
    uint64_t lastTradeId;   // Changed to uint64_t
    uint64_t timestamp;      // Changed to uint64_t
    bool isBuyerMaker;  
};

// Callback function for curl to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

// Function to fetch aggregate trades from Binance API
std::string fetch_agg_trades(const string& symbol, int limit) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        string url = BASE_URL + AGG_TRADES_ENDPOINT + "?symbol=" + symbol + "&limit=" + to_string(limit);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
    }
    return readBuffer;
}

// Function to process individual trade data and print it
void process_trade(const Json::Value &trade) {
    uint64_t aggregate_trade_id = trade["a"].asLargestUInt();  
    string price = trade["p"].asString();
    string quantity = trade["q"].asString();
    uint64_t first_trade_id = trade["f"].asLargestUInt(); 
    uint64_t last_trade_id = trade["l"].asLargestUInt();
    uint64_t timestamp = trade["T"].asLargestUInt();
    bool buyer_maker = trade["m"].asBool();

    cout << "Aggregate Trade ID: " << aggregate_trade_id << endl;
    cout << "Price: " << price << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "First Trade ID: " << first_trade_id << endl;
    cout << "Last Trade ID: " << last_trade_id << endl;
    cout << "Timestamp: " << timestamp << endl;
    cout << "Buyer Maker: " << (buyer_maker ? "Yes" : "No") << endl;
    cout << "-------------------------" << endl;
}

// Function to parse the JSON response and process each trade
void parse_and_print_trades(const string& json_response) {
    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    string errs;

    // Using unique_ptr to manage the Json::CharReader
    const unique_ptr<Json::CharReader> reader(readerBuilder.newCharReader());
    
    if (!reader->parse(json_response.c_str(), json_response.c_str() + json_response.length(), &root, &errs)) {
        cerr << "Failed to parse JSON: " << errs << endl;
        return;
    }

    for (const auto& trade : root) {
        process_trade(trade);
    }
}

// Function to measure the speed of parsing the trades
void measure_parsing_speed(const string& symbol, int limit) {
    auto start_time = chrono::high_resolution_clock::now();

    string json_response = fetch_agg_trades(symbol, limit);
    
    if (!json_response.empty()) {
        parse_and_print_trades(json_response);  // This function is now defined
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "\nTime taken to parse " << limit << " trades: " << elapsed.count() << " seconds\n";
}

int main() {
    const char* symbol = getenv("SYMBOL");
    const char* limit_str = getenv("LIMIT");
    int limit = limit_str ? atoi(limit_str) : 10;

    if (!symbol) {
        symbol = "BTCUSDT";
    }

    measure_parsing_speed(symbol, limit);
    return 0;
}
