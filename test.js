function Resolve(url, arr, callback) {
    const array_1 = arr;
    const url_1 = url;
    const xhr = new XMLHttpRequest();
    xhr.open('POST', url_1, true);

    xhr.onload = function() {
        if (xhr.status === 200) {
            console.log('数组上传成功!');
            const responseText = xhr.responseText;
            const responseJSON = JSON.parse(responseText);
            console.log('服务器返回的数据:', responseJSON);
            callback(responseJSON); // 调用回调函数，并传递响应数据
            exportJSON(responseJSON); // 导出 JSON 数据
        } 
        else {
            console.error('数组上传失败!');
        }
    };
    
    xhr.send(JSON.stringify(array_1));
}

function exportJSON(data) {
    const jsonStr = JSON.stringify(data);
    // 在这里执行导出操作，例如写入文件或发送到其他服务端
    console.log('导出的 JSON 数据:', jsonStr);
}

let url = 'http://10.133.35.103:8080';
let arr = {
    "grid": [
        [
            9,
            8,
            6,
            1,
            4,
            2,
            7,
            3,
            5
        ],
        [
            1,
            4,
            2,
            7,
            0,
            5,
            9,
            8,
            6
        ],
        [
            7,
            3,
            5,
            9,
            0,
            6,
            1,
            4,
            2
        ],
        [
            8,
            9,
            1,
            6,
            2,
            4,
            3,
            5,
            7
        ],
        [
            6,
            2,
            4,
            3,
            0,
            7,
            8,
            9,
            1
        ],
        [
            3,
            5,
            7,
            8,
            9,
            1,
            6,
            2,
            4
        ],
        [
            4,
            6,
            9,
            2,
            1,
            8,
            5,
            7,
            3
        ],
        [
            2,
            1,
            8,
            5,
            7,
            3,
            4,
            6,
            9
        ],
        [
            5,
            7,
            3,
            4,
            6,
            9,
            2,
            1,
            8
        ]
    ]
};
console.log(arr);
Resolve(url, arr, function(responseJSON) {
    // 在这里定义和使用回调函数
    console.log(responseJSON);
  });
