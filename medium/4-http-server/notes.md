```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>

int main() {
    // 1. Create socket (IPv4, TCP)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Define server address
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept from any IP
    address.sin_port = htons(8080);       // Listen on Port 8080

    // 3. Bind socket to the port
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // 4. Start listening
    listen(server_fd, 3);
    std::cout << "Server started on port 8080..." << std::endl;

    while (true) {
        // 5. Accept a connection
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        // 6. Read Request
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        std::cout << "Request received: " << buffer << std::endl;

        // 7. Send valid HTTP Response
        std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
        send(new_socket, hello.c_str(), hello.length(), 0);

        close(new_socket);
    }
    return 0;
}
```

# TCP Server — Every Argument, Every Detail

```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
```

| Header           | What it gives you                                                                    |
| ---------------- | ------------------------------------------------------------------------------------ |
| `<iostream>`     | `std::cout`, `std::endl`                                                             |
| `<sys/socket.h>` | `socket()`, `bind()`, `listen()`, `accept()`, `send()`, `close()`, `struct sockaddr` |
| `<netinet/in.h>` | `struct sockaddr_in`, `INADDR_ANY`, `htons()`                                        |
| `<unistd.h>`     | `read()`, `close()`                                                                  |
| `<string>`       | `std::string`, `.c_str()`, `.length()`                                               |

---

## Step 1 — `socket(AF_INET, SOCK_STREAM, 0)`

```cpp
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
```

### `server_fd` — what is it?

`fd` = file descriptor. In Unix, everything is a file — including sockets. `socket()` returns an `int` that is an index into a table the OS keeps of open "files" for your process.

```
Process file descriptor table:
  0  →  stdin
  1  →  stdout
  2  →  stderr
  3  →  your socket   ← server_fd = 3
```

You pass this integer to every subsequent call. If it returns `-1`, the call failed.

### Arg 1 — `AF_INET`

Tells the OS what kind of addresses this socket will use. `AF` = Address Family.

| Value       | Meaning                         |
| ----------- | ------------------------------- |
| `AF_INET`   | IPv4 (192.168.x.x)              |
| `AF_INET6`  | IPv6                            |
| `AF_UNIX`   | Local machine only (no network) |
| `AF_PACKET` | Raw Ethernet frames             |

### Arg 2 — `SOCK_STREAM`

Tells the OS how data flows.

| Value         | Meaning                                   |
| ------------- | ----------------------------------------- |
| `SOCK_STREAM` | TCP — ordered, reliable, connection-based |
| `SOCK_DGRAM`  | UDP — fire and forget, no connection      |
| `SOCK_RAW`    | Raw IP — you craft the headers yourself   |

### Arg 3 — `0`

The specific protocol within the type. `0` means "pick the default for this combo."

- `SOCK_STREAM` + `0` → OS picks **TCP** (only real option)
- `SOCK_DGRAM` + `0` → OS picks **UDP**
- Could write `IPPROTO_TCP` explicitly — identical result

---

## Step 2 — `sockaddr_in address`

```cpp
sockaddr_in address;
```

`sockaddr_in` is a struct defined in `<netinet/in.h>`. Its actual layout:

```cpp
struct sockaddr_in {
    sa_family_t    sin_family;   // 2 bytes — address family tag
    in_port_t      sin_port;     // 2 bytes — port number
    struct in_addr sin_addr;     // 4 bytes — IP address
    char           sin_zero[8];  // 8 bytes — padding, never touched
};
```

`sin_zero` exists only to make `sockaddr_in` the same total size as the generic `sockaddr` struct so the cast in `bind()` is safe. Total: 16 bytes. At this point `address` is uninitialized — just raw stack memory. The next three lines fill it in.

---

## Step 2a — `address.sin_family = AF_INET`

Every `sockaddr_*` struct starts with `sin_family`. When you cast your struct to the generic `sockaddr*` and pass it to `bind()`, the OS reads THIS field first to know which struct type it's actually dealing with. It's the struct's own type tag.

```
AF_INET  = 2    (IPv4)
AF_INET6 = 10   (IPv6)
AF_UNIX  = 1    (local socket)
```

Must match what you passed to `socket()`.

---

## Step 2b — `address.sin_addr.s_addr = INADDR_ANY`

Two layers here.

`sin_addr` is itself a nested struct:

```cpp
struct in_addr {
    uint32_t s_addr;  // 4 bytes — raw IPv4 address packed into one 32-bit integer
};
```

An IPv4 address like `192.168.1.5` is just four bytes packed together:

```
192      168      1        5
0xC0     0xA8     0x01     0x05   →   0xC0A80105
```

`s_addr` holds that raw 32-bit integer.

`INADDR_ANY` is the constant `0` — meaning `0.0.0.0`. Not a real IP — it's a wildcard telling the OS: "accept connections on **all** network interfaces on this machine" (WiFi, Ethernet, loopback — all of them).

To restrict to localhost only:

```cpp
address.sin_addr.s_addr = inet_addr("127.0.0.1");
```

---

## Step 2c — `address.sin_port = htons(8080)`

**`8080`** — the port to listen on. Ports 0–1023 are reserved for system services (80 = HTTP, 443 = HTTPS, 22 = SSH). 1024–65535 are free for your use. 8080 is the classic developer HTTP port.

**`htons()`** — Host To Network Short. A function, not a type.

Your CPU (x86/ARM) is **little-endian**: stores least significant byte first.
The network protocol is **big-endian**: most significant byte first.

```
8080 decimal  =  0x1F90 hex

Your CPU stores:    0x90  0x1F   (little-endian, LSB first)
Network expects:    0x1F  0x90   (big-endian, MSB first)

htons() flips those two bytes.
```

Without `htons()` you'd silently listen on port `0x901F = 36895` instead of 8080.

---

## Step 3 — `bind(server_fd, (struct sockaddr*)&address, sizeof(address))`

Glues the address struct onto the socket. Before this the socket exists but has no location. After this the OS registers: "socket fd=3 owns port 8080 on this machine."

### Arg 1 — `server_fd`

Which socket to configure. The integer from `socket()`.

### Arg 2 — `(struct sockaddr*)&address`

Two things:

- `&address` — the memory address of your `sockaddr_in` struct
- `(struct sockaddr*)` — casts that pointer to the generic base type

`bind()` works for all address families. Its actual signature:

```cpp
int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
```

It doesn't know about `sockaddr_in` specifically. You hand it the generic pointer; it reads `sin_family` inside to figure out how to interpret the bytes. C-style polymorphism through casting.

### Arg 3 — `sizeof(address)`

Because the cast lost the type info, `bind()` has no idea how many bytes your struct is. You pass the size explicitly. `sizeof(sockaddr_in) = 16`.

**Return value:** `0` on success, `-1` on failure. This code ignores it — production code always checks.

---

## Step 4 — `listen(server_fd, 3)`

### Arg 1 — `server_fd`

Which socket to put into listening mode. After this call the socket changes state — it can no longer send data directly, only queue incoming connections.

### Arg 2 — `3` (the backlog)

While your code is busy with one client, others may arrive. The OS queues them.

```
Your code busy with client A

Client B arrives  →  OS queues B     [B]
Client C arrives  →  OS queues C     [B, C]
Client D arrives  →  OS queues D     [B, C, D]
Client E arrives  →  REFUSED — queue full (backlog = 3)
```

Each `accept()` call pulls one off the front of the queue. `3` is fine for learning — production uses hundreds or thousands.

---

## Step 5 — `accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)`

Pulls one pending connection off the queue. **Blocks forever** if no client is waiting.

### Arg 1 — `server_fd`

The listening socket to pull from.

### Arg 2 — `(struct sockaddr*)&address`

An **output** parameter. `accept()` **writes** the client's IP and port into your `address` struct. After the call: `address.sin_addr` = client's IP, `address.sin_port` = client's port. Same cast trick as `bind()`.

### Arg 3 — `(socklen_t*)&addrlen`

Also an **output** parameter — a pointer to your `addrlen` int. `accept()` writes back how many bytes it actually filled into the address struct. It's a pointer because the function needs to modify it. `(socklen_t*)` casts your `int*` to the type the function expects — `socklen_t` is just a typedef for `unsigned int`.

### Return value — `new_socket`

A brand new file descriptor, dedicated to this one client. `server_fd` is untouched and keeps listening.

```
server_fd  = 3   ← permanent, always listening
new_socket = 4   ← client 1  (close when done)
new_socket = 5   ← client 2  (close when done)
new_socket = 6   ← client 3  (close when done)
```

`server_fd` is the receptionist. `new_socket` is the private room you take each client into.

---

## Step 6 — `char buffer[1024] = {0}`

A raw byte array on the stack, 1024 bytes, **all zeroed** by `= {0}`. Without it, the array holds garbage from whatever was previously on the stack. Zero-filling means if incoming data is shorter than 1024 bytes, the rest is null bytes — treating it as a C string is safe.

---

## Step 6b — `read(new_socket, buffer, 1024)`

### Arg 1 — `new_socket`

Which file descriptor to read from. `read()` is a generic POSIX call — works on files, pipes, and sockets equally because they're all file descriptors.

### Arg 2 — `buffer`

Where to write the incoming bytes. The array name `buffer` decays to a pointer to its first element.

### Arg 3 — `1024`

Maximum bytes to read in one call. Won't read more even if more is waiting. Blocks until at least 1 byte arrives.

**Return value:** Number of bytes actually read. Ignored here — production code loops on `read()` because one call may not get the full message.

A real browser HTTP request landing in `buffer` looks like:

```
GET / HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0
...
```

---

## Step 7 — `send(new_socket, hello.c_str(), hello.length(), 0)`

```cpp
std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
```

This is a hand-crafted HTTP response. HTTP is a plain-text protocol with a specific format:

```
HTTP/1.1 200 OK          ← status line
Content-Type: text/plain ← header
Content-Length: 12       ← header: tells browser how many bytes in the body
                         ← blank line separates headers from body
Hello World!             ← body (exactly 12 bytes — matches Content-Length)
```

### Arg 1 — `new_socket`

Which socket to send on. The dedicated client socket.

### Arg 2 — `hello.c_str()`

`send()` is a C function — it expects `const char*`, not `std::string`. `.c_str()` gives you a raw null-terminated pointer to the string's internal buffer.

### Arg 3 — `hello.length()`

How many bytes to send. `send()` doesn't know where your data ends — you tell it explicitly.

### Arg 4 — `0` (flags)

| Flag           | Meaning                                             |
| -------------- | --------------------------------------------------- |
| `0`            | Default — just send it                              |
| `MSG_DONTWAIT` | Don't block if buffer is full, return error instead |
| `MSG_NOSIGNAL` | Don't crash if the client already disconnected      |
| `MSG_OOB`      | Send out-of-band urgent data                        |

`0` = none of the above. Normal blocking send.

---

## Step 8 — `close(new_socket)`

Closes the client socket. Internally this sends a **TCP FIN** packet to the client — signaling "I'm done talking." Frees the file descriptor number so the OS can reuse it.

`server_fd` is **not** closed — the `while(true)` loops back to `accept()` and waits for the next client.

---

## Full Picture

```
socket()    →  create the tool (get a file descriptor)
bind()      →  stamp an address onto it (IP + port)
listen()    →  open for business (start queuing clients)
loop:
  accept()  →  block until a client arrives, get a new dedicated socket
  read()    →  receive their bytes into a buffer
  send()    →  write your response bytes back
  close()   →  send TCP FIN, free the file descriptor
              (server_fd stays alive, loop repeats)
```

You built a working HTTP server from raw syscalls — no framework, no library, just the OS API.
