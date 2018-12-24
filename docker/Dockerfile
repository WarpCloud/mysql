FROM 172.16.1.99/transwarp/kundb/compile-mariadb:v1.0 AS compile

COPY . /root/mysql_src
RUN mkdir /root/mysql_bld && \
    cd /root/mysql_bld && \
    cmake ../mysql_src -DCMAKE_INSTALL_PREFIX=/usr/local/mysql -DDOWNLOAD_BOOST=1 -DWITH_BOOST=/root/boost/ -DBUILD_CONFIG=mysql_release -DWITH_EMBEDDED_SERVER=false -DWITH_EMBEDDED_SHARED_LIBRARY=false -DWITH_UNIT_TESTS=OFF -DWITHOUT_TOKUDB=1 -DWITH_JEMALLOC=no &&\
    make -j 4 && \
    make install && \
    cd /root && rm -fr mysql_bld mysql_srcs

FROM 172.16.1.99/transwarp/kundb/compile-mariadb:v1.0 AS release

COPY --from=compile /usr/local/mysql /usr/local/mysql

WORKDIR /usr/local/mysql
