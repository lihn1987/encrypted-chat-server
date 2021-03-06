#ifndef __SERVER__HPP__
#define __SERVER__HPP__
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>
#include <boost/asio.hpp>
#include <google/protobuf/message.h>
#include "client_item.hpp"
#include "config/config.hpp"
class Server{
public:
    Server(boost::asio::io_context& ioc);
    ~Server();
public:
    void Start();
    void Write(std::shared_ptr<::google::protobuf::Message> msg);
    std::list<std::shared_ptr<ClientItem>> GetClientList();
private:
    void OnAccept(std::shared_ptr<ClientItem> new_item,
      const boost::system::error_code& error);
    void OnReadMsg(std::shared_ptr<ClientItem> new_item,
      std::shared_ptr<::google::protobuf::Message> msg);
    void OnError(std::shared_ptr<ClientItem> item);
private://message process
    bool ProcessMessage(std::shared_ptr<ClientItem> new_item, std::shared_ptr<::google::protobuf::Message> msg);
    bool ProcessPing(std::shared_ptr<ClientItem> new_item, std::shared_ptr<::google::protobuf::Message> _msg);
    bool ProcessLogin(std::shared_ptr<ClientItem> new_item, std::shared_ptr<::google::protobuf::Message> _msg);
private:
    uint64_t GetTimeStamp();
private:
    boost::asio::io_context& ioc;
    boost::asio::io_service::work work;
    boost::asio::ip::tcp::acceptor acceptor;
    std::mutex client_list_mutex;
    std::list<std::shared_ptr<ClientItem>> client_list;
};
#endif