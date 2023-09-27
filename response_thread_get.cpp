#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include <boost/asio/read_until.hpp>
#include "suduku_get_json.h"
using boost::asio::ip::tcp;
using namespace std;


void thread_fun(tcp::socket socket,string &response) {
    // ��ȡ������Ϣ
    boost::asio::streambuf request_buffer;
    boost::asio::read_until(socket, request_buffer, "\r\n");

    // ��������ͷ
    istream request_stream(&request_buffer);
    string request_method;
    request_stream >> request_method;
    response = "aab";
    // ����GET����
    if (request_method == "GET") {
        // ������Ӧͷ
        string response_header =
            "HTTP/1.1 200 OK\r\n"
            //������������ʵ���Ӧͷ
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Type: application/json\r\n";
        // ��ȡJSON�ļ�����
        cout << "��ȡjson����" << endl;
        json j = getsuduku(1);
        //std::string json = read_file("sudu1.json");
        string json = j.dump();    // {"happy":true,"pi":3.141}
        cout << j.dump(4) << std::endl;//����������ʽ���
        // ������Ӧ��Ϣ
        response =
            response_header +
            "Content-Length: " + std::to_string(json.size()) + "\r\n"
            "\r\n"
            + json;
        // ������Ӧ��Ϣ
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(response), ignored_error);
        cout << "����һ����Ӧ" << endl;
        //int res = get_json_data();
    }
}



int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
        cout << "������ʼ" << endl;
        mutex mtx;
        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            cout << "���յ�����" << endl;
            std::string response;
            thread th1(thread_fun, std::move(socket),ref(response));
            th1.detach();

        }

    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}