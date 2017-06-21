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
*  @file     chat_message.h
*  @brief    client和server的通讯编码格式
*  此实例源于http://www.boost.org/doc/libs/
*  1_55_0/doc/html/boost_asio/examples/cpp03_examples.html
*  
*  Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
*  
*  Distributed under the Boost Software License, Version 1.0. (See accompanying
*  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
* 
*  在boost的官方实例基础上稍作改动
*
*  @author   李坤
*  @email    1061152718@qq.com
*  @version  4.0.1.5(版本号)
*  @date     2017/06/02
*  @license  Massachusetts Institute of Technology License (MIT)
*
*----------------------------------------------------------------------------
*  Remark         : Description
*----------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*----------------------------------------------------------------------------
*  2017/06/02 | 3.0.0.1   | 李坤            | Create file
*----------------------------------------------------------------------------
*
*****************************************************************************/

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

/**
    * @brief server和client共用的数据包类\n
    * 一条数据分为三部分：\n
    * 1、字符串头的4个字节表示数据部分的长度，最大为9999，不足补0\n
    * 2、字符串第五个字符表示命令类型，详见MessageCode.h\n
    * 3、字符串后面为命令内容
    * 4、如：一条信息\t019b2player2|styisagay
    * \t\t\t解读为这条命令共19个字符, 为一条ANSWER_FOR_ROOM命令, 表示\n
    * \t\t\t这是房间内的第二个玩家, 角色为player2, 玩家名称为styisagay
*/

class chat_message
{
public:
    enum { header_length = 4 };
    enum { max_body_length = 512 };
    
    chat_message()
    : body_length_(0)
    {
    }
    
    const char* data() const
    {
        return data_;
    }
    
    char* data()
    {
        return data_;
    }
    
    size_t length() const
    {
        return header_length + body_length_;
    }
    
    const char* body() const
    {
        return data_ + header_length;
    }
    
    char* body()
    {
        return data_ + header_length;
    }
    
    size_t body_length() const
    {
        return body_length_;
    }
    
    void body_length(size_t new_length)
    {
        body_length_ = new_length;
        if (body_length_ > max_body_length)
            body_length_ = max_body_length;
    }

    /** 
    * @brief 将报头的4字节字符串转换成数字
    *
    * @return 返回说明
    *        -<em>false</em> 转换出错，输入非法
    *        -<em>true</em> 成功编码，chat_message实例内的body_length已经成功编码
    */

    bool decode_header()
    {
        using namespace std; 
        char header[header_length + 1] = "";
        strncat(header, data_, header_length);
        body_length_ = atoi(header);
        if (body_length_ > max_body_length)
        {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    /** 
    * @brief 将字符串长度编码到编码头中
    */
    
    void encode_header()
    {
        using namespace std;
        char header[header_length + 1] = "";
        sprintf(header, "%4d", body_length_);
        memcpy(data_, header, header_length);
    }
    
private:
    char data_[header_length + max_body_length];
    size_t body_length_;
};

#endif // CHAT_MESSAGE_HPP
