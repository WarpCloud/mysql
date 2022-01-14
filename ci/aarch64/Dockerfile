FROM 172.16.1.99/kundb-ci/aarch64/mysql-compiler:debian10 AS compile

COPY . /root/mysql_src

RUN mkdir -p /root/boost2 && wget ftp://172.16.1.32/pub/kundb/boost_1_73_0.tar.gz -P /root/boost2 && \
    mkdir -p /root/jemalloc && wget ftp://172.16.1.32/pub/kundb/jemalloc-5.2.1.tar.gz -P /root/jemalloc && \
    cd /root/jemalloc && tar -xvf jemalloc-5.2.1.tar.gz && cd jemalloc-5.2.1 && ./configure && make install

RUN mkdir /root/mysql_bld && \
    cd /root/mysql_bld && \
    cmake ../mysql_src -DCMAKE_INSTALL_PREFIX=/usr/local/mysql80-org -DDOWNLOAD_BOOST=1 -DWITH_BOOST=/root/boost2/boost_1_73_0.tar.gz \
      -DBUILD_CONFIG=mysql_release -DMAX_INDEXES=255 -DWITH_EMBEDDED_SERVER=false -DWITH_EMBEDDED_SHARED_LIBRARY=false -DWITH_UNIT_TESTS=OFF -DWITH_SSL=system -DCMAKE_EXE_LINKER_FLAGS=-latomic && \
    make -j 8 && \
    make install && \
    cd /root && rm -fr mysql_bld mysql_srcs

FROM 172.16.1.99/kundb-ci/aarch64/base:debian10

COPY --from=compile /usr/local/mysql80-org /usr/local/mysql80-org
COPY --from=compile /usr/local/lib/libjemalloc* /usr/local/lib/

WORKDIR /usr/local/mysql80-org
