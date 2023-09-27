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


// 定义九宫格行列数和总格数
const int N = 9;
const int TOTAL_CELLS = N * N;

// 检查数独的某个格子是否可以填入指定数字
bool isValid(int grid[N][N], int row, int col, int num) {
    // 检查行中是否已经出现该数字
    for (int i = 0; i < N; i++) {
        if (grid[row][i] == num) {
            return false;
        }
    }

    // 检查列中是否已经出现该数字
    for (int i = 0; i < N; i++) {
        if (grid[i][col] == num) {
            return false;
        }
    }

    // 检查所在小九宫格中是否已经出现该数字
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

// 使用回溯法生成数独题目
bool generateSudokuHelper(int grid[N][N], vector<int>& nums, int index) {
    if (index == TOTAL_CELLS) {
        return true; // 数独题目生成完成
    }

    int row = index / N;
    int col = index % N;

    // 随机填入数字
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

    // 使用 std::random_device 生成真正的随机数种子
    std::random_device rd;

    // 使用随机数种子初始化伪随机数生成器
    std::mt19937 g(rd());

    // 对 nums 进行随机乱序
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
    //    file << data.dump(4); // 以4个空格缩进的方式保存 JSON 数据
    //    file.close();
    //    cout << "数独题目已保存为 " << filename << std::endl;
    //}
    //else {
    //    cerr << "无法保存数独题目到 " << filename << std::endl;
    //}
}

json getsuduku(int num) {
    int grid[N][N] = { 0 };

    if (generateSudoku(grid)) {
        cout << "生成的数独题目：" << endl;
        //string filename = "sudu";
        //filename += to_string(num);
        //filename += ".json";
        json json_data = saveSudokuToJson(grid);
        return json_data;
    }
    else {
        cout << "生成题目失败" << endl;
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
