#include <getopt.h>

#include "Client/Processor/User.hpp"
#include "Common/Message/Logon.hpp"
#include "Common/Message/Logout.hpp"
#include "Common/Message/Tag.hpp"
#include "Common/Core/Logger.hpp"

namespace proc
{
    std::optional<fix::Message> User::process(fix::Serializer::AnonMessage &_msg, Context &_ctx)
    {
        if (_msg.at(fix::Tag::MsgType) == fix::Logon::MsgType) {
            _ctx.User = _msg.at(fix::Tag::TargetCompId);
            _ctx.HeartBit = utils::to<uint32_t>(_msg.at(fix::Tag::HearBtInt));
            _ctx.Loggin = true;
        }
        else if (_ctx.Loggin && _msg.at(fix::Tag::MsgType) == fix::Logout::MsgType) {
            _ctx.reset();
        }
        return {};
    }

    std::optional<fix::Message> User::process(const std::string &_entry, Context &_ctx)
    {
        std::vector<std::string> words = utils::space_split(_entry);
        int param = 0;
        optind = 1;

        if (words.size() < 1)
            return {};
        if (words.at(0) == "logon") {
            std::vector<const char *> cwords;

            for (auto &_word : words)
                cwords.emplace_back(_word.c_str());
            while (param != -1) {
                param = getopt(words.size(), const_cast<char * const *>(cwords.data()), "u:");
                switch (param) {
                    case ':':       // missing param
                    case '?':       // unknow
                        return {};
                    case 'u':
                        return buildLogon(optarg);
                };
            }
        } else if (words.at(0) == "new_order") {
            std::vector<const char *> cwords;
            std::string orderId = "";
            std::string quantity = "";
            std::string price = "";
            std::string side = "";
            std::string symbol = "";

            for (auto &_word : words)
                cwords.emplace_back(_word.c_str());

            while (param != -1) {
                param = getopt(words.size(), const_cast<char * const *>(cwords.data()), "i:q:p:s:S:");
                switch (param) {
                    case ':':       // missing param
                    case '?':       // unknow
                        return {};
                    case 'i':
                        orderId = optarg;
                        break;
                    case 'q':
                        quantity = optarg;
                        break;
                    case 'p':
                        price = optarg;
                        break;
                    case 's':
                        side = optarg;
                        break;
                    case 'S':
                        symbol = optarg;
                        break;
                };
            }
            if (orderId.empty() || quantity.empty() || price.empty() || side.empty() || symbol.empty()) {
                Logger::Log("[User Input]: Missing parameter for new_order");
                return {};
            }
            return buildNewOrder(orderId, quantity, price, side, symbol);
        } else if (words.at(0) == "logout") {
            if (words.size() != 1)
                return {};
            return buildLogout();
        } else if (words.at(0) == "status") {
            if (words.size() != 1)
                return {};
            std::cout << _ctx << std::endl;
            return {};
        } else if (words.at(0) == "help") {
            if (words.size() != 1)
                return {};
            std::cout << "Commands:" << std::endl;
            std::cout << "  logon -u <user>" << std::endl;
            std::cout << "  new_order -i <id> -q <quantity> -p <price> -s <side>(buy|sell) -S <symbol>(GOLD|USD|EURO)" << std::endl;
            std::cout << "  logout" << std::endl;
            std::cout << "  status" << std::endl;
            std::cout << "  help" << std::endl;
            return {};
        }
        return {};
    }

    std::optional<fix::Message> User::build(char _tag, Context &_ctx) const
    {
        if (_tag == fix::Logon::cMsgType)
            return buildLogon(_ctx.User);
        else if (_tag == fix::Logout::cMsgType)
            return buildLogout();
        return {};
    }

    fix::Message User::buildLogon(const UserId &_id) const
    {
        fix::Logon logon;

        logon.header.set49_SenderCompId(_id);
        logon.set98_EncryptMethod("0");
        logon.set108_HeartBtInt("30");
        return logon;
    }

    fix::Message User::buildNewOrder(const std::string &_orderId, const std::string &_quantity, const std::string &_price, const std::string &_side, const std::string &_symbol) const
    {
        fix::NewOrderSingle order;

        order.header.set49_SenderCompId(_orderId);
        order.set11_clOrdID(_orderId);
        order.set21_handlInst("3");
        order.set38_orderQty(_quantity);
        order.set40_ordType("2");
        order.set44_price(_price);
        order.set54_side((_side == "buy") ? "3" : "4");
        order.set55_symbol(_symbol);
        order.set60_transactTime(utils::get_timestamp());

        return order;
    }

    fix::Message User::buildLogout() const
    {
        return fix::Logout();
    }
}