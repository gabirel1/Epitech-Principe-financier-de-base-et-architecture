#include "Client/Processor/OBData.hpp"

namespace proc
{
    std::optional<fix::Message> OBData::process(const std::string &_entry, Context &_ctx)
    {
        std::vector<std::string> words = utils::space_split(_entry);
        std::vector<const char *> cwords;
        std::string sym = "";
        std::string side = "";
        std::string subId = "";
        std::string depth = "";
        int param = 0;
        fix::MarketDataRequest request;

        if (words.size() < 1)
            return {};
        for (auto &_word : words)
            cwords.emplace_back(_word.c_str());
        while (param != -1) {
            param = getopt(words.size(), const_cast<char * const *>(cwords.data()), "u:");
            switch (param) {
                case ':':       // missing param
                case '?':       // unknow
                    return {};
                case 'm': sym = optarg;
                    break;
                case 's': side = optarg;
                    break;
                case 'd': depth = optarg;
                    break;
                case 'i': subId = optarg;
                    break;
            };
        }

        if (subId.empty() && words.at(0) != "unsub")
            return {};
        else if (words.at(0) != "unsub")
            subId = utils::id();
        if (sym.empty())
            return {};
        request.set146_noRelatedSym("1");
        request.set55_symbol(sym);
        request.set262_mDReqID(subId);
        if (words.at(0) == "fresh")
            request.set263_subscriptionRequestType("0");
        else if (words.at(0) == "sub")
            request.set263_subscriptionRequestType("1");
        else if (words.at(0) == "unsub")
            request.set263_subscriptionRequestType("2");
        else
            return {};
        if (depth == "0" || depth == "full")
            request.set264_marketDepth("0");
        else if (depth == "1" || depth == "top")
            request.set264_marketDepth("1");
        else if (utils::is_numeric(depth))
            request.set264_marketDepth(depth);
        else
            return {};
        if (side == "both" || side == "0|1") {
            request.set267_noMDEntryTypes("2");
            request.set269_mDEntryType("0,1");
        } else if (side == "0" || side == "1") {
            request.set267_noMDEntryTypes("1");
            request.set269_mDEntryType(side);
        } else {
            return {};
        }
        // build subscription
        return request;
    }

    std::optional<fix::MarketDataRequest> buildForRefresh()
    {
        fix::MarketDataRequest refresh;

        while (param != -1) {
            param = getopt(words.size(), const_cast<char * const *>(cwords.data()), "u:m:s:d:i");
            switch (param) {
                case 'm':
                case 's':
                case 'd':
                case 'i':
                case -1:
                    break;
                default:
                    return {};
            };
            if (param != -1 && !applyModification(param, optarg, refresh))
                return {};
        }
        return refresh;
    }

    bool applyModification(char _param, const std::string &_value, fix::MarketDataRequest &_refresh)
    {
        if (_param == 'm')
            return setSymbol(_value, _refresh);
        else if (_param == 'd')
            return setDepth(_value, _refresh);
        else if (_param == 'i') {
            request.set262_mDReqID(subId);
            // need to check subscribe
            return true;
        }
    }
}
