
#include "HuffmanCompress.h" 


string HuffmanCompressor::compress(const string& inputData) {

    map<unsigned char, int> frequencies;
    long long totalByteCount = 0;
    for (unsigned char byte : inputData) {
        frequencies[byte]++;
        totalByteCount++;
    }

    if (totalByteCount == 0) {
        return ""; 
    }
    //Î»±àÂë²Ù×÷

    HuffmanTree hTree(frequencies);
    map<unsigned char, string> huffmanCodes = hTree.getCodes();
    stringstream ss_out(ios::binary | ios::out);
    ss_out.write(reinterpret_cast<const char*>(&totalByteCount), sizeof(totalByteCount));
    for (int i = 0; i < 256; ++i) {
        int freq = frequencies[static_cast<unsigned char>(i)];
        ss_out.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
    }
    unsigned char buffer = 0; int bitCount = 0;
    for (unsigned char byte : inputData) {
        string code = huffmanCodes[byte];
        for (char bit : code) {
            buffer = (buffer << 1) | (bit == '1' ? 1 : 0);
            bitCount++;
            if (bitCount == 8) {
                ss_out.write(reinterpret_cast<const char*>(&buffer), 1);
                buffer = 0; bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        ss_out.write(reinterpret_cast<const char*>(&buffer), 1);
    }
    cout << "¹§Ï²¹§Ï²£¬ÄÚ´æÑ¹Ëõ³É¹¦" << endl;
    return ss_out.str();
}

string HuffmanCompressor::decompress(const string& inputData) {
    stringstream ss_in(inputData, ios::binary | ios::in);

    long long totalByteCount = 0;
    ss_in.read(reinterpret_cast<char*>(&totalByteCount), sizeof(totalByteCount));
    if (totalByteCount == 0) {
        return "";
    }
    //¶ÁÈ¡ÆµÂÊ±í
    map<unsigned char, int> frequencies;
    for (int i = 0; i < 256; ++i) {
        int freq;
        ss_in.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        if (freq > 0) frequencies[static_cast<unsigned char>(i)] = freq;
    }

    HuffmanTree hTree(frequencies);
    HuffmanNode* currentNode = hTree.get_root();
    if (currentNode == nullptr) {
        cerr << "½¨Ê÷´íÎó£¬Ê§°Ü" << endl;
        return ""; 
    }
    stringstream ss_out(ios::binary | ios::out);

    unsigned char buffer;
    long long bytesDecoded = 0;
    while (bytesDecoded < totalByteCount && ss_in.read(reinterpret_cast<char*>(&buffer), 1)) {
        for (int i = 7; i >= 0; --i) {
            bool bit = (buffer >> i) & 1;
            currentNode = bit ? currentNode->right : currentNode->left;

            if (!currentNode->left && !currentNode->right) {
                ss_out.write(reinterpret_cast<const char*>(&currentNode->data), 1);
                bytesDecoded++;
                if (bytesDecoded == totalByteCount) break;
                currentNode = hTree.get_root();
            }
        }
    }

    cout << "Successfully decompressed from memory" << endl;
    return ss_out.str();
}