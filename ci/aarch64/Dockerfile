FROM 172.16.1.99/kundb/aarch64/mysql-compiler:v1 AS compile

COPY . /root/mysql_src
RUN mkdir /root/mysql_bld && \
    cd /root/mysql_bld && \
    cmake ../mysql_src -DCMAKE_INSTALL_PREFIX=/usr/local/mysql57 -DDOWNLOAD_BOOST=1 -DWITH_BOOST=/root/boost/ -DBUILD_CONFIG=mysql_release -DMAX_INDEXES=255 -DWITH_EMBEDDED_SERVER=false -DWITH_EMBEDDED_SHARED_LIBRARY=false -DWITH_UNIT_TESTS=OFF &&\
    make -j 2 && \
    make install && \
    cd /root && rm -fr mysql_bld mysql_srcs

FROM 172.16.1.99/kundb/aarch64/ubuntu:16.04 AS release

COPY --from=compile /usr/local/mysql57 /usr/local/mysql57

WORKDIR /usr/local/mysql57