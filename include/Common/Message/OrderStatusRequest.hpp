#pragma once

#include "Common/Message/Fix.hpp"

namespace fix
{
    class OrderStatusRequest : public Message
    {
    public:
        OrderStatusRequest();
        ~OrderStatusRequest();
        // void set37_OrderID(const std::string &_val);
        void set11_ClOrdID(const std::string &_val);
        // void set41_OrigClOrdID(const std::string &_val);
        void set55_Symbol(const std::string &_val);
        void set54_Side(const std::string &_val);
    protected:
        static constexpr const char *m_msgType = "H";
    };
}