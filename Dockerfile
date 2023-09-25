# Use an official GCC base image from Docker Hub
FROM gcc:latest

# Install CMake and GDB
RUN apt-get update && \
    apt-get install -y cmake gdb

# Create a build/Debug directory and build the C++ application in Debug mode
RUN mkdir -p /app/src
# COPY Matrix /app/src
# COPY BinaryTree /app/src
COPY LinkedList /app/src

RUN mkdir -p /build/Debug && cd /build/Debug && cmake -DCMAKE_BUILD_TYPE=Debug /app/src
RUN cd /build/Debug && make

# Set the entry point to run GDB with the compiled binary
# CMD ["gdb", "/build/Debug/main"]
CMD ["/build/Debug/main"]

# Set the entry point to run a long-lived process (in this case 'tail -f /dev/null')
# CMD ["tail", "-f", "/dev/null"]