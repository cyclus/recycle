
FROM cyclus/cyclus:stable

COPY . /recycle
WORKDIR /recycle
RUN mkdir Release
WORKDIR /recycle/Release
RUN cmake .. -DCMAKE_BUILD_TYPE=Release
RUN make -j2 && make install

