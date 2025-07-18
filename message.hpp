#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>


class Message {
    public: 
        Message() : bodyLength_(0) {}
        
        enum {maxBytes = 512};
        enum {header = 4};

        Message(std::string message){
            bodyLength_ = getNewBodyLength(message.size());
            encodeHeader();
            std::memcpy(data + header, message.c_str(), bodyLength_);
        };

        void printMessage(){
            std::string message = getData();
            std::cout<<"Message recieved: "<<message<<std::endl;
        }

        std::string getData(){
            int length = header + bodyLength_;
            std::string result(data, length);
            return result;
        }

        std::string getBody(){
            std::string dataStr = getData();
            std::string result = dataStr.substr(header, bodyLength_);
            return result;
        }

        size_t getNewBodyLength(size_t newLength){
            if(newLength > maxBytes){
                return maxBytes;
            }
            return newLength;
        }

        void encodeHeader(){
            char new_header[header+1] = "";
            sprintf(new_header, "%4d", static_cast<int>(bodyLength_));
            memcpy(data, new_header, header);
        }
        
       bool decodeHeader(){
            char new_header[header+1] = "";
            strncpy(new_header, data, header);
            new_header[header] = '\0';
            int headerValue = atoi(new_header);
            if(headerValue > maxBytes){
                bodyLength_ = 0;
                return false;
            }
            bodyLength_ = headerValue;
            return true;
        }

        size_t getBodyLength(){
            return bodyLength_;
        }

    private: 
        char data[header+maxBytes];
        size_t bodyLength_;
};

#endif MESSAGE_HPP
