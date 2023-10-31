//  Send a message on the 'Hello' topic
#include <string>
#include <iostream>
#include <conio.h>
#include <thread>
#include <deque>

#include <zmq.hpp>

int main()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a PUSH socket and connect to interface
    zmq::socket_t pushSocket{context, zmq::socket_type::push};
    pushSocket.bind("tcp://*:5555");

    std::deque<std::string> messageBuffer;  // Buffer to hold messages

    int i = 0;
    while (!_kbhit()) {
        //  Write three messages, each with an envelope and content
        char message[5];
        snprintf(message, sizeof(message), "%04d", i);

        // Push the message into the buffer
        messageBuffer.push_back(message);

        while (!messageBuffer.empty()) {
            zmq::message_t zmqMessage(messageBuffer.front().begin(), messageBuffer.front().end());
            zmq::send_result_t result = pushSocket.send(zmqMessage, zmq::send_flags::dontwait);
            if (!result) {
                // If message could not be sent, break the loop and try again later
                break;
            }
            else {
                std::cout << "Sent: " << messageBuffer.front() << std::endl;
                messageBuffer.pop_front();
            }
            
        }
        std::cout << message << std::endl;
        i++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    while (true) {
        std::cout << "Not send!" << std::endl;
    }
    
    return 0;
}
