#include "Common/Core/Logger.hpp"
#include "Client/Processor/ReportHandler.hpp"
#include "Common/Message/Tag.hpp"

namespace proc
{
    std::optional<fix::Message> ReportHandler::process(fix::Serializer::AnonMessage &_msg, Context &_ctx)
    {
        if (!_ctx.Loggin && _msg.at(fix::Tag::MsgType) != fix::Logon::MsgType) {
            Logger::Log("[ReportHandler] You are not logged in, please log in first");
            return {};
        }
        Logger::Log("[ReportHandler] Received message: { MsgType: '", _msg.at(fix::Tag::MsgType), "' }");
        if (_msg.at(fix::Tag::MsgType) == fix::Reject::MsgType) {
            Logger::Log("[ReportHandler] Received reject message: { RefSeqNum: '", _msg.at(fix::Tag::RefSeqNum), "', Text: '", _msg.at(fix::Tag::Text), "' }");
            return {};
        }
        if (_msg.at(fix::Tag::MsgType) == fix::ExecutionReport::MsgType) {
            treatExecutionReport(_msg, _ctx);
        } else {
            Logger::Log("[ReportHandler] Message not handled by the ReportHandler: { MsgType: '", _msg.at(fix::Tag::MsgType), "' }");
        }
        return {};
    }

    std::optional<fix::Message> ReportHandler::build(char _tag, Context &_ctx) const
    {
        std::ignore = _tag;
        std::ignore = _ctx;

        return {};
    }

    void ReportHandler::treatExecutionReport(fix::Serializer::AnonMessage &_msg, Context &_ctx)
    {
        Logger::Log("[ReportHandler] Received a new Execution Report");

        std::string orderId = _msg.at(fix::Tag::OrderID);
        std::string quantity = _msg.at(fix::Tag::OrderQty);
        std::string price = _msg.at(fix::Tag::Price);
        std::string side = _msg.at(fix::Tag::Side);
        std::string symbol = _msg.at(fix::Tag::Symbol);
        std::string status = _msg.at(fix::Tag::OrdStatus);

        if (status == "8") {
            Logger::Log("[ReportHandler] Order rejected: [", orderId, "] reason: '", _msg.at(fix::Tag::Text), "'");
            return;
        }
        Logger::Log("[ReportHandler] {NewOrder} New order: ", orderId, " ", quantity, " ", price, " ", side, " ", symbol);

        OrderClient _order;
        _order.orderId = orderId;
        _order.quantity = std::stoull(quantity);
        _order.price = std::stod(price);
        _order.type = (side == "1") ? OrderType::Bid : OrderType::Ask;
        _order.status = static_cast<OrderStatus>(std::stoi(status));
        _order.symbol = symbol;

        if (_ctx.userInfos.getOrderToCancel() != "" && _order.status == OrderStatus::Replaced) {
            _ctx.userInfos.replaceOrder(_ctx.userInfos.getOrderToCancel(), _order);
            _ctx.userInfos.clearOrderToCancel();
        }
        if (_order.status == OrderStatus::Filled || _order.status == OrderStatus::Canceld)
            _ctx.userInfos.removeOrder(_order.orderId);
        else
            _ctx.userInfos.addOrder(_order);
        _ctx.userInfos.addHistory(_order);
    }
}