# qiwi-api-brute

Its just fun code for checking qiwi api token brute. Working with proxy and threads, for compile you neede (make, libcurl)

# Building
https://github.com/keekway/qiwi-api-brute.git

cd qiwi-api-brute

make

# Using
./main -proxy=http://127.0.0.1:8123 -threads=12

-proxy [can be http, socks, tor, polipo, privoxy]
-threads [normal more then 12]
