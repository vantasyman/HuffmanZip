#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept> // 包含标准异常

#include "httplib.h" 

#include "HuffmanCompress.h" 

using namespace std;

int main() {
    HuffmanCompressor compressor;
    httplib::Server svr;
    svr.set_default_headers({
        { "Access-Control-Allow-Origin", "*" },
        { "Access-Control-Allow-Methods", "POST, GET, OPTIONS" },
        { "Access-Control-Allow-Headers", "Content-Type" }
        });
    svr.Post("/compress", [&](const httplib::Request& req, httplib::Response& res) {
        cout << "收到 /compress 请求" << endl;

        if (req.form.has_file("file")) {

            const auto file_data = req.form.get_file("file");

            string compressedData = compressor.compress(file_data.content);

            res.set_content(compressedData, "application/octet-stream");
            string downloadName = file_data.filename + ".huff";
            res.set_header("Content-Disposition", "attachment; filename=\"" + downloadName + "\"");

            cout << "压缩完成，已发回 " << downloadName << endl;
        }
        else {
            res.set_content("错误：没有文件被上传", "text/plain");
        }
        });
    svr.Post("/decompress", [&](const httplib::Request& req, httplib::Response& res) {
        cout << "收到 /decompress 请求" << endl;

        if (req.form.has_file("file")) {

            const auto file_data = req.form.get_file("file");

            string decompressedData = compressor.decompress(file_data.content);

            res.set_content(decompressedData, "application/octet-stream");

            string downloadName = file_data.filename;
            size_t pos = downloadName.rfind(".huff");
            if (pos != string::npos) {
                downloadName = downloadName.substr(0, pos);
            }
            else {
                downloadName += ".decompressed";
            }

            res.set_header("Content-Disposition", "attachment; filename=\"" + downloadName + "\"");
            cout << "解压完成，已发回 " << downloadName << endl;
        }
        else {
            res.set_content("错误：没有文件被上传", "text/plain; charset=utf-8");
        }
        });

    cout << "Huffman 服务器启动在 http://localhost:8080" << endl;
    svr.listen("localhost", 8080); 

    return 0;
}