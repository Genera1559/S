#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include <boost/asio/read_until.hpp>
#include "suduku_get_json.h"
using boost::asio::ip::tcp;
using namespace std;


void thread_fun(tcp::socket socket, string& response) {
    // 读取请求消息
    boost::asio::streambuf request_buffer(4096);
    request_buffer.prepare(4096);
    boost::asio::read_until(socket, request_buffer, "\r\n");

    // 忽略请求头
    istream request_stream(&request_buffer);
    string request_method;
    request_stream >> request_method;
    response = "aab";
    // 处理GET请求
    if (request_method == "GET") {
        // 构建响应头
        string response_header =
            "HTTP/1.1 200 OK\r\n"
            //添加允许跨域访问的响应头
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Type: application/json\r\n";
        // 读取JSON文件内容
        cout << "获取题目json内容" << endl;
        json j = getsuduku(1);
        //std::string json = read_file("sudu1.json");
        string json = j.dump();    // {"happy":true,"pi":3.141}
        //cout << j.dump(4) << std::endl;//按照如下形式输出
        // 构建响应消息
        response =
            response_header +
            "Content-Length: " + std::to_string(json.size()) + "\r\n"
            "\r\n"
            + json;
        // 发送响应消息
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(response), ignored_error);
        cout << "发送一次响应" << endl;
        //int res = get_json_data();
    }
    else if (request_method == "POST") {
        string response_header =
            "HTTP/1.1 200 OK\r\n"
            //添加允许跨域访问的响应头
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Type: application/json\r\n";
        // 读取JSON文件内容
        cout << "获取json答案内容" << endl;

        // 读取请求体
        boost::asio::read_until(socket, request_buffer, "\r\n\r\n");
        //cout << request_buffer << endl;
        string request_body = "";
        if (request_buffer.size() > 0) {
            // 获取请求体的长度
            std::istreambuf_iterator<char> eos;
            string reqstr(std::istreambuf_iterator<char>(&request_buffer), eos);
            size_t pos = reqstr.find("\r\n\r\n");
            //cout << pos << endl;
            //npos表示size_t的最大值
            if (pos != string::npos) {
                pos += 4; // 跳过\r\n\r\n的位置
                request_body = reqstr.substr(pos); // 提取请求体部分
                //cout << request_body <<endl;
            }
        }
        else {
            cout << "题目为空" << endl;
        }

        // 将请求体转换为JSON
        json j_request;
        try {
            if (!request_body.empty()) {
                j_request = json::parse(request_body);
                cout << j_request <<endl;
            }
            else {
                cout << "空请求体" << endl;
            }
        }
        catch (const json::exception& e) {
            std::cerr << "解析请求体失败：" << e.what() << std::endl;
            // 构建错误响应消息
            if (!request_body.empty()) {
                std::cerr << "请求体内容：" << request_body << std::endl;
            }
            response =
                "HTTP/1.1 400 Bad Request\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: 0\r\n"
                "\r\n";
            // 发送错误响应消息
            boost::asio::write(socket, boost::asio::buffer(response));
            return;
        }

        // 调用getsuduku函数处理请求
        try {
            json j_response = getsudukuSolve(1, j_request);
            cout << "答案:" << j_response << endl;
            // 将响应转换为字符串
            string response_body = j_response.dump();

            // 构建响应消息
            response =
                response_header +
                "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
                "\r\n"
                + response_body;

            // 发送响应消息
            boost::asio::write(socket, boost::asio::buffer(response));
        }
        catch (const json::exception& e) {
            cout << "答案获取出错,可能是题目非法" << endl;
        }
        
        

        
    }
    else {
        cout << "请求方式错误" << endl;
    }

    // 关闭socket连接
    boost::system::error_code ignored_error;
    socket.shutdown(tcp::socket::shutdown_send, ignored_error);  // 关闭发送方向的连接
    }





int main() {
    

    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
        io_context.run();
        cout << "监听开始" << endl;
        mutex mtx;
        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            cout << "接收到请求" << endl;
            std::string response;
            //thread_fun(std::move(socket), ref(response));
            thread th1(thread_fun, std::move(socket),ref(response));
            th1.detach();

        }
        io_context.run();

    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}