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
    // ��ȡ������Ϣ
    boost::asio::streambuf request_buffer(4096);
    request_buffer.prepare(4096);
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
        cout << "��ȡ��Ŀjson����" << endl;
        json j = getsuduku(1);
        //std::string json = read_file("sudu1.json");
        string json = j.dump();    // {"happy":true,"pi":3.141}
        //cout << j.dump(4) << std::endl;//����������ʽ���
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
    else if (request_method == "POST") {
        string response_header =
            "HTTP/1.1 200 OK\r\n"
            //������������ʵ���Ӧͷ
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Type: application/json\r\n";
        // ��ȡJSON�ļ�����
        cout << "��ȡjson������" << endl;

        // ��ȡ������
        boost::asio::read_until(socket, request_buffer, "\r\n\r\n");
        //cout << request_buffer << endl;
        string request_body = "";
        if (request_buffer.size() > 0) {
            // ��ȡ������ĳ���
            std::istreambuf_iterator<char> eos;
            string reqstr(std::istreambuf_iterator<char>(&request_buffer), eos);
            size_t pos = reqstr.find("\r\n\r\n");
            //cout << pos << endl;
            //npos��ʾsize_t�����ֵ
            if (pos != string::npos) {
                pos += 4; // ����\r\n\r\n��λ��
                request_body = reqstr.substr(pos); // ��ȡ�����岿��
                //cout << request_body <<endl;
            }
        }
        else {
            cout << "��ĿΪ��" << endl;
        }

        // ��������ת��ΪJSON
        json j_request;
        try {
            if (!request_body.empty()) {
                j_request = json::parse(request_body);
                cout << j_request <<endl;
            }
            else {
                cout << "��������" << endl;
            }
        }
        catch (const json::exception& e) {
            std::cerr << "����������ʧ�ܣ�" << e.what() << std::endl;
            // ����������Ӧ��Ϣ
            if (!request_body.empty()) {
                std::cerr << "���������ݣ�" << request_body << std::endl;
            }
            response =
                "HTTP/1.1 400 Bad Request\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: 0\r\n"
                "\r\n";
            // ���ʹ�����Ӧ��Ϣ
            boost::asio::write(socket, boost::asio::buffer(response));
            return;
        }

        // ����getsuduku������������
        try {
            json j_response = getsudukuSolve(1, j_request);
            cout << "��:" << j_response << endl;
            // ����Ӧת��Ϊ�ַ���
            string response_body = j_response.dump();

            // ������Ӧ��Ϣ
            response =
                response_header +
                "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
                "\r\n"
                + response_body;

            // ������Ӧ��Ϣ
            boost::asio::write(socket, boost::asio::buffer(response));
        }
        catch (const json::exception& e) {
            cout << "�𰸻�ȡ����,��������Ŀ�Ƿ�" << endl;
        }
        
        

        
    }
    else {
        cout << "����ʽ����" << endl;
    }

    // �ر�socket����
    boost::system::error_code ignored_error;
    socket.shutdown(tcp::socket::shutdown_send, ignored_error);  // �رշ��ͷ��������
    }





int main() {
    

    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
        io_context.run();
        cout << "������ʼ" << endl;
        mutex mtx;
        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            cout << "���յ�����" << endl;
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