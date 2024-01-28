FROM gcc

WORKDIR /app

RUN  apt-get update; apt-get install -y cmake;

ADD CMakeLists.txt /app/CMakeLists.txt

ADD test/ /app/test

ADD include/Common/ /app/include/Common
ADD src/Common/ /app/src/Common

ADD include/Server/ /app/include/Server
ADD src/Server/ /app/src/Server

RUN cmake .
RUN make MarketCommon
RUN make MarketServer

ENTRYPOINT ["/app/MarketServer"]