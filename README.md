Algorithmic complexity:
    API CALL:
        The complexity of making th API call is O(1) (constant time) since the response
        is fetched in constant time for a fixed number of trades.

    Parsing trades:
        The complexity of parsing the trades is O(n) wher n is number of trades. 
        The program loops through each tread, and each operation inside the loop
        (accessing the trade's fields) is in constant time O(1).

        Overall complexity of parsing algorithm is O(n) where n is number of trades
        being parsed.


The function fetch_agg_trades() uses libcurl to make a GET request to the Binance 
Futures /fapi/v1/aggTrades endpoint. It constructs the URL with the symbol (e.g., BTCUSDT) 
and the limit for the number of trades.

The function parse_and_print_trades() uses JsonCpp to parse (call for process_trade) the JSON 
response and extract the fields (a, p, q, f, l, T, and m) for each trade.

Each trade is stored in a Trade structure.

The function measure_parsing_speed() uses the std::chrono library to measure the time taken 
to fetch and parse the trades. It prints the elapsed time in seconds.

The main() function specifies the symbol (BTCUSDT) and the limit (10 trades by default). 

I did implement solution where we use signle request to fetch limited trades (10 by default,
info in docker-compose.yaml) but current code is not optimized to stream trades continuously.


Build and run docker container:
                                1. docker-compose build
                                2. docker-compose up








