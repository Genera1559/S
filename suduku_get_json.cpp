#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <random>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;


// ����Ź������������ܸ���
const int N = 9;
const int TOTAL_CELLS = N * N;

// ���������ĳ�������Ƿ��������ָ������
bool isValid(int grid[N][N], int row, int col, int num) {
    // ��������Ƿ��Ѿ����ָ�����
    for (int i = 0; i < N; i++) {
        if (grid[row][i] == num) {
            return false;
        }
    }

    // ��������Ƿ��Ѿ����ָ�����
    for (int i = 0; i < N; i++) {
        if (grid[i][col] == num) {
            return false;
        }
    }

    // �������С�Ź������Ƿ��Ѿ����ָ�����
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }

    return true;
}

// ʹ�û��ݷ�����������Ŀ
bool generateSudokuHelper(int grid[N][N], vector<int>& nums, int index) {
    if (index == TOTAL_CELLS) {
        return true; // ������Ŀ�������
    }

    int row = index / N;
    int col = index % N;

    // �����������
    for (int i = 0; i < N; i++) {
        int num = nums[i];
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;

            if (generateSudokuHelper(grid, nums, index + 1)) {
                return true;
            }

            grid[row][col] = 0;
        }
    }

    return false;
}

bool generateSudoku(int grid[N][N]) {
    vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // ʹ�� std::random_device �������������������
    std::random_device rd;

    // ʹ����������ӳ�ʼ��α�����������
    std::mt19937 g(rd());

    // �� nums �����������
    std::shuffle(nums.begin(), nums.end(), g);

    return generateSudokuHelper(grid, nums, 0);
}



json saveSudokuToJson( int grid[N][N]) {
    json data;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            data["grid"][i][j] = grid[i][j];
        }
    }
    return data;
    //ofstream file(filename);
    //if (file.is_open()) {
    //    file << data.dump(4); // ��4���ո������ķ�ʽ���� JSON ����
    //    file.close();
    //    cout << "������Ŀ�ѱ���Ϊ " << filename << std::endl;
    //}
    //else {
    //    cerr << "�޷�����������Ŀ�� " << filename << std::endl;
    //}
}

json getsuduku(int num) {
    int grid[N][N] = { 0 };

    if (generateSudoku(grid)) {
        cout << "���ɵ�������Ŀ��" << endl;
        //string filename = "sudu";
        //filename += to_string(num);
        //filename += ".json";
        json json_data = saveSudokuToJson(grid);
        return json_data;
    }
    else {
        cout << "������Ŀʧ��" << endl;
    }
}
//int main() {
//    json getsuduku(1);
//}
//int get_json_data() {
//    thread th1(getsuduku, 1);
//    thread th2(getsuduku, 2);
//    thread th3(getsuduku, 3);
//    thread th4(getsuduku, 4);
//    thread th5(getsuduku, 5);
//    thread th6(getsuduku, 6);
//    thread th7(getsuduku, 7);
//    thread th8(getsuduku, 8);
//    thread th9(getsuduku, 9);
//    th1.join();
//    th2.join();
//    th3.join();
//    th4.join();
//    th5.join();
//    th6.join();
//    th7.join();
//    th8.join();
//    th9.join();
//    return 0;
//}
