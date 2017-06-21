/*****************************************************************************
*  Copyright (C) 2017 李坤 1061152718@qq.com
*
*  此文件属于软件学院2017c++大项目泡泡堂选题的项目文件.
*
*  此项目是开源项目, 在期末答辩之后, 我们可能会在假期里对一些因时间不够未完成的功能进
*  行补充, 以及适配windows平台, 在未来如果技术允许的情况下, 会酌情开发ios版本和anroid
*  版本, 期待您能够为这个开源项目提供宝贵意见, 帮助我们做得更好, 如果能够贡献宝贵的代
*  码那就更令人开心了.
*
*  本项目遵守MIT开源协议, 这也就是说, 您需要遵守的唯一条件就是在修改后的代码或者发行
*  包包含原作者的许可信息. 除非获得原作者的特殊许可, 在任何项目(包括商业项目)里使用
*  本项目的文件都需要包含作者的许可.
*
*  如果对项目有疑问或者其他建议, 欢迎联系13167211978@163.com, 1061152718@qq.com,
*  我们期待能和您互相交流合作, 学习更多的知识.
*
*  另外注意: 此项目需要您自行配置cocos环境,安装boost库, 如果遇到相关问题的话, 欢迎将
*  错误日志发给我们, 您的帮助将有助于改善游戏的体验.
*
*  @file     LocalServer.h
*  @brief    server相关类
*  
*
*  @author   李坤
*  @email    1061152718@qq.com
*  @version  4.0.1.5(版本号)
*  @date     2017/06/11
*  @license  Massachusetts Institute of Technology License (MIT)
*
*----------------------------------------------------------------------------
*  Remark         : Description
*----------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*----------------------------------------------------------------------------
*  2017/06/11 | 4.0.0.1   | 李坤            | Create file
*----------------------------------------------------------------------------
*
*****************************************************************************/

#ifndef _LocalServer_h_
#define _LocalServer_h_

#include "cocos2d.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include "chat_message.h"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

//==================================chart_participant========================

/**
*   @brief  chart_session的基类\n
*           保证每个chart_session都需要重载自己的deliver方法
*/

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef boost::shared_ptr<chat_participant> chat_participant_ptr;

//==================================chat_room===============================

/**
*   @brief  chart_session的基类\n
*           保证每个chart_session都需要重载自己的deliver方法
*/

class chat_room
{
public:

    /** 
    * @brief                将新加入的会话加入房间内以便以广播的方式发送数据
    *
    * @param participant    已成功连接的、需要加入聊天房间的会话
    *
    */
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);
        std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
                      boost::bind(&chat_participant::deliver, participant, _1));
    }
    
    /** 
    * @brief                将退出的会话对象退出房间，不再向其广播数据
    *
    * @param participant    已成功连接的、需要退出聊天房间的会话
    *
    */
    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);
    }
    
    /** 
    * @brief                向房间内所有会话发送数据
    *
    * @param msg            需要发送的数据
    *
    */
    void deliver(const chat_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();
        
        std::for_each(participants_.begin(), participants_.end(),
                      boost::bind(&chat_participant::deliver, _1, boost::ref(msg)));
    }
    
private:
    std::set<chat_participant_ptr>  participants_;      ///保存房间内所有参与的会话
    chat_message_queue              recent_msgs_;       ///保存房间内需要发送的信息
    //---------------------------------------------------------------------------
    // 房间内允许保留的最大数据量
    //---------------------------------------------------------------------------
    enum { max_recent_msgs = 0 };
};


//==================================chat_session===============================

/**
*   @brief  会话类\n
*           继承自chat_participant类，需要自定义发送消息的方法
*/
class chat_session
: public chat_participant,
public boost::enable_shared_from_this<chat_session>
{
public:

    /** 
    * @brief                初始化会话，此时并不加入房间，只有成功链接时才加入
    *
    * @param io_service     用来执行异步操作的io_service对象
    * @param room           房间对象
    *
    */
    chat_session(boost::asio::io_service& io_service, chat_room& room)
    : socket_(io_service),
    room_(room)
    {
    }
    
    /** 
    * @brief                获取会话的套接字
    */
    tcp::socket& socket()
    {
        return socket_;
    }
    
    /** 
    * @brief                初始化会话，此时并不加入房间，只有成功链接时才加入
    *
    * @param io_service     用来执行异步操作的io_service对象
    * @param room           房间对象
    *
    */
    void start()
    {
        room_.join(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                boost::bind(
                                            &chat_session::handle_read_header, shared_from_this(),
                                            boost::asio::placeholders::error)); 
    }
    
    /** 
    * @brief                将某个客户端收到的消息发送给此会话对应的客户端\n
    *                       发送完第一条需要发送的信息后，调用handle_write，如果有剩余的信息的话则继续发送
    *
    * @param msg            收到的消息
    *
    */
    void deliver(const chat_message& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front().data(),
                                                         write_msgs_.front().length()),
                                     boost::bind(&chat_session::handle_write, shared_from_this(),
                                                 boost::asio::placeholders::error));
        }
    }
    
    /** 
    * @brief        在接受到消息后处理数据字符串，并调用处理命令的函数(handle_read_body)
    *
    * @param error  调用I/O例程时返回的错误码
    *
    */
    void handle_read_header(const boost::system::error_code& error)
    {
        //收到客户端消息后调用1
        if (!error && read_msg_.decode_header())
        {
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                    boost::bind(&chat_session::handle_read_body, shared_from_this(),
                                                boost::asio::placeholders::error));
        }
        else
        {
            room_.leave(shared_from_this());
        }
    }
    
    /** 
    * @brief        当接受完数据并处理完字符串长度后调用该函数，处理数据体, 在操作\n
    *               结束后继续读取报头, 在读取结束后调用handle_read_header继续处理
    *
    * @param error  调用I/O例程时返回的错误码
    *
    */
    void handle_read_body(const boost::system::error_code& error)
    {
        if (!error)
        {
            //收到客户端消息后调用2
            room_.deliver(read_msg_);
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                    boost::bind(&chat_session::handle_read_header, shared_from_this(),
                                                boost::asio::placeholders::error));
        }
        else
        {
            room_.leave(shared_from_this());
        }
    }
    
    /** 
    * @brief        当发送完第一条待发送数据结束后，如果仍有待发送消息则继续\n
    *               递归调用发送handle_write发送剩余数据
    *
    * @param error  调用I/O例程时返回的错误码
    *
    */
    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
                boost::asio::async_write(socket_,
                                         boost::asio::buffer(write_msgs_.front().data(),
                                                             write_msgs_.front().length()),
                                         boost::bind(&chat_session::handle_write, shared_from_this(),
                                                     boost::asio::placeholders::error));
            }
        }
        else
        {
            room_.leave(shared_from_this());
        }
    }
    
private:
    tcp::socket             socket_;            ///该会话的套接字              
    chat_room&              room_;              ///该会话所在的房间
    chat_message            read_msg_;          ///读取到的数据
    chat_message_queue      write_msgs_;        ///该会话中需要发送的数据队列 
};

typedef boost::shared_ptr<chat_session> chat_session_ptr;


//==================================chat_server===============================

/**
*   @brief  chart_server类\n
*           全局只有一个用来执行异步操作的io_service对象，并在\n
*           初始化的时候建立一个监听器侦听收到的链接请求
*/
class chat_server
{
public:

    /** 
    * @brief                将传入的端点信息与监听器绑定，开始侦听来\n
    *                       自client的链接，并调用start_accept函数\n
    *                       处理到来的链接请求
    *
    * @param io_service     用来执行异步操作的io_service对象
    * @param endpoint       要监听的端口号
    *
    */
    chat_server(boost::asio::io_service& io_service,
                const tcp::endpoint& endpoint)
    : io_service_(io_service),
    acceptor_(io_service, endpoint) 
    {
        start_accept();
    }
    
    /** 
    * @brief                启动异步接收操作，当收到异步链接后，创建一个新的会话\n
    *                       并执行handle_accept，如果没有错误的话handle_accept\n
    *                       会继续调用该函数以等待新的异步链接请求
    *
    */
    void start_accept()
    {
        chat_session_ptr new_session(new chat_session(io_service_, room_));
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&chat_server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }
    
    /** 
    * @brief                当调用I/O例程时没有返回错误码时，将新的会话加入chart_room，\n
    *                       会话正式开始读取异步客户端发来的信息
    *
    * @param session        接收到异步链接请求的时候创建的会话对象，如果调用I/O例程时没有\n
    *                       返回错误码则开始启动异步接收客户端信息
    * @param error          调用I/O例程时返回的错误码
    *
    */
    void handle_accept(chat_session_ptr session,
                       const boost::system::error_code& error)

    {
        if (!error)
        {
            session->start();
        }
        
        start_accept();
    }
    
private:
    boost::asio::io_service&    io_service_;        ///用来执行异步操作的io_service对象(全局只会产生一个)
    tcp::acceptor               acceptor_;          ///用来侦听收到的链接请求的监听器
    chat_room                   room_;              ///聊天房间
                                                    /** 每当接收到新的链接请求并当不存在链接错误时将新的会话加入房间 */
};

typedef boost::shared_ptr<chat_server>  chat_server_ptr;
typedef std::list<chat_server_ptr>      chat_server_list;

//==================================LocalServer===============================

/**
 *   @brief  LocalServer类，继承自Node类，可被cocos内存管理系统自动管理\n
 */
class LocalServer : public cocos2d::Node
{
public:
    /**
    * @brief    创建LocalServer对象并运行server
    */
    static LocalServer* create(void)
    {
        LocalServer *sprite = new LocalServer();
        if (sprite)
        {
            sprite->autorelease();
            sprite->runServer();
            
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    /**
    * @brief    开启一个新线程，使server驻留后台运行
    */
    void runServer(void)
    {
        std::thread t(&LocalServer::server, this);
        t.detach();
    }
    
    /**
    * @brief    在分离线程中运行server\n
    *           只要io_service不被关闭，该线程就会在后台持续运行，直到主线程结束后被清理
    */
    int server(void)
    {
        try
        {
            boost::asio::io_service io_service;
            chat_server_list servers;
            
            using namespace std;
            tcp::endpoint endpoint(tcp::v4(), 11332);
            chat_server_ptr server(new chat_server(io_service, endpoint));
            servers.push_back(server);
            
            io_service.run();
            
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }
        
        return 0;
    }
    
};


#endif /* _LocalServer_h_ */
