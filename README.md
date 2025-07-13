# ChatRoomImplementation

This project is a simple, multi-client chat room application built with C++. It uses the **Boost.Asio** library for asynchronous networking and **multi-threading** to handle concurrent users efficiently.

## Core Components

The application's architecture is built around three main classes:

  * **`Session`**: Manages an individual client's connection. It handles all asynchronous reading and writing of messages to and from the client's socket. Each session runs in its own thread.
  * **`Room`**: Represents the central chat room. It maintains a list of all connected participants, allowing it to broadcast incoming messages to every client in the session.
  * **`Message`**: A simple class that encapsulates the data being sent. It handles the encoding and decoding of message headers and bodies for network transmission.

## Low-Level Design

```mermaid
classDiagram
    class Participant {
        <<interface>>
        +deliver(Message& message)*
        +write(Message& message)*
    }
    
    class Session {
        -tcp::socket clientSocket
        -boost::asio::streambuf buffer
        -Room& room
        -std::deque~Message~ messageQueue
        +Session(tcp::socket s, Room& room)
        +start()
        +deliver(Message& message)
        +write(Message& message)
        +async_read()
        +async_write(std::string messageBody, size_t messageLength)
    }
    
    class Room {
        -std::deque~Message~ messageQueue
        -std::set~ParticipantPointer~ participants
        +join(ParticipantPointer participant)
        +leave(ParticipantPointer participant)
        +deliver(ParticipantPointer participant, Message& message)
    }
    
    class Message {
        -char data[header+maxBytes]
        -size_t bodyLength_
        +Message()
        +Message(std::string message)
        +getData() std::string
        +getBody() std::string
        +getNewBodyLength(size_t newLength) size_t
        +encodeHeader()
        +decodeHeader() bool
        +getBodyLength() size_t
        +printMessage()
    }
    
    class Server {
        -boost::asio::io_context io_context
        -tcp::acceptor acceptor
        -Room room
        +main(int argc, char* argv[])
        +accept_connection(io_context, port, acceptor, room, endpoint)
    }
    
    class Client {
        -boost::asio::io_context io_context
        -tcp::socket socket
        -std::thread input_thread
        +main(int argc, char* argv[])
        +async_read(tcp::socket& socket)
    }
    
    class boost_asio_io_context {
        +run()
        +post(function)
    }
    
    class Thread_Model {
        <<concept>>
        +IO Thread: Runs io_context.run()
        +Input Thread: Handles user input
        +Async Callbacks: Run on IO Thread
    }
    
    class MessageFlow {
        <<concept>>
        +Client Input → Socket Write
        +Socket Read → Message Parse → Room Broadcast
        +Room Broadcast → Session Queue → Socket Write
    }
    
    class AsyncPattern {
        <<concept>>
        +async_read_until(delimiter)
        +async_write(buffer)
        +post(function)
        +Completion Handlers
    }
    
    Participant <|-- Session
    Session *-- "1" Room : has-reference
    Session o-- "*" Message : manages-in-queue
    Room o-- "*" Message : manages-in-queue
    Room o-- "*" Participant : contains
    Server ..> Session : creates
    Server ..> Room : creates
    Client ..> Message : sends/receives
    Server ..> boost_asio_io_context : uses
    Client ..> boost_asio_io_context : uses
    Client -- Thread_Model : implements
    Session -- AsyncPattern : implements
    Message -- MessageFlow : participates-in
    
    note for MessageFlow "Message path: 
    1. Client input → Client socket
    2. Server reads from socket
    3. Server creates Message
    4. Room broadcasts to participants
    5. Each Session queues message
    6. Session writes to client socket"
    
    note for Thread_Model "Thread architecture:
    1. Main thread runs io_context
    2. Client has dedicated input thread
    3. All async operations handled by io_context
    4. Socket operations are non-blocking"
    
    note for AsyncPattern "Asynchronous operations:
    1. async_read_until() with completion handler
    2. async_write() with completion handler
    3. Handlers capture shared_from_this()
    4. post() for thread-safe socket writes"
```

## Build and Run

1.  **Clone** the repository.
2.  Download and set up the libraries from the official [Boost release page](https://www.boost.org/releases/latest/).
    **Note:** This project is best developed and run on a Linux-based system. 🐧
3.  **Build** the project by running `make`.
4.  **Start the Server**:
    ```bash
    ./chatApp <port>
    ```
5.  **Start a Client**:
    ```bash
    ./clientApp <port>
    ```
