#include <getopt.h>

#include "Client/Processor/User.hpp"
#include "Common/Message/Logon.hpp"
#include "Common/Message/Logout.hpp"
#include "Common/Message/Tag.hpp"

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
        } else if (words.at(0) == "logout") {
            if (words.size() != 1)
                return {};
            return buildLogout();
        } else if (words.at(0) == "status") {
            if (words.size() != 1)
                return {};
            std::cout << _ctx << std::endl;
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

    fix::Message User::buildLogout() const
    {
        return fix::Logout();
    }
}