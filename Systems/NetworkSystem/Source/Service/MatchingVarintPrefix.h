#pragma once

#include <boost/asio.hpp>
#include <google/protobuf/stubs/common.h>

class MatchingVarintPrefix {
public:
    explicit MatchingVarintPrefix() : decodedVarint(0), messageSize(0), bitsRead(0) {}

    template <typename Iterator>
    std::pair<Iterator, bool> operator()(Iterator begin, Iterator end) {
        Iterator i = begin;
        while (i != end) {
            // header
            if (messageSize == 0) {
                char c = *i;
                decodedVarint = ((c & 0x7F) << bitsRead) + decodedVarint;
                if ((c & 0x80) == 0)  {
                    messageSize = decodedVarint;
                    decodedVarint = 0;
                    bitsRead = 0;
                } else {
                    bitsRead += 7;
                }
            } else if (end - begin >= messageSize) {
                return std::make_pair(i, true);
            }
            i++;
        }
        return std::make_pair(i, false);
    }

private:
    google::protobuf::uint32 decodedVarint;
    int messageSize;
    int bitsRead;

};

namespace boost {
    namespace asio {
        template <> struct is_match_condition<MatchingVarintPrefix> : public boost::true_type {};
    }
}
