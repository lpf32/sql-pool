# sql-pool
download libzdb: http://www.tildeslash.com/libzdb/dist/libzdb-3.0.tar.gz
#compile libzdb.c
cc -L/usr/local/libzdb/lib/ -lzdb -I/usr/local/libzdb/include/zdb test_libzdb.c -std=c99 -g -o test_libzdb
