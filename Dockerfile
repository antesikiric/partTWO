# Use an official Ubuntu as the base image
FROM ubuntu:20.04

# Set non-interactive mode for apt-get to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    curl \
    libcurl4-openssl-dev \
    libjsoncpp-dev \
    make \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /usr/src/app

# Copy the C++ source code into the container
COPY binance_trades.cpp .

# Compile the C++ program
RUN g++ -o binance_trades binance_trades.cpp -lcurl -ljsoncpp -std=c++11 -I/usr/include/jsoncpp

# Run the compiled program when the container starts
CMD ["./binance_trades"]
