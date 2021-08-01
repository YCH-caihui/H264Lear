// Huffuman.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>



using namespace std;


//每个符号定义以为一个结构体，包括字符和出现频次
typedef struct
{
    unsigned char character;
    unsigned int frequency;
}CharNode;

static bool open_input_file(ifstream& input, const char* inputName)
{
    input.open(inputName);
    if (!input.is_open())
    {
        return false;
    }
    return true;
}

struct MinHeapNode
{
    char data;
    unsigned int freq;
    MinHeapNode* left, * right;

    MinHeapNode(char data, unsigned freq)
    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

typedef struct MinHeapNode MinHeapNode;

struct compara
{
    bool operator()(MinHeapNode* l, MinHeapNode* r)
    {
        return (l->freq > r->freq);
    }
};

static void get_huffman_code(MinHeapNode* root, string code) 
{
    if (!root)
    {
        return;
    }
    if (root->data != -1)
    {
        cout << root->data << ":" << code;
    }
    get_huffman_code(root->left, code + "0");
    get_huffman_code(root->right, code + "1");
}

int main()
{
    ifstream inputFile;
    if (!open_input_file(inputFile, "input.txt"))
    {
        cout << "Error: opening input file failed!" << endl;
        return -1;
    }
    char buf = inputFile.get();
    CharNode nodeArr[256] = { {0, 0} };
    while (inputFile.good())
    {
        cout << buf;
        nodeArr[buf].character = buf;
        nodeArr[buf].frequency++;
        buf = inputFile.get();

    }
    cout << endl;

    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compara> minHeap;
    for (int idx = 0; idx < 256; idx++) 
    {
        if (nodeArr[idx].frequency > 0)
        {
            cout << "Node" << idx << ": [" << nodeArr[idx].character << ", " << nodeArr[idx].frequency << "]" << endl;
            minHeap.push(new MinHeapNode(nodeArr[idx].character, nodeArr[idx].frequency));
        }
    }

    MinHeapNode* leftNode = NULL, * right = NULL, * topNode = NULL;
    while (minHeap.size() != 1) 
    {
        leftNode = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

        topNode = new MinHeapNode(-1, leftNode->freq + right->freq);
        topNode->left = leftNode;
        topNode->right = right;
        minHeap.push(topNode);
    }
    get_huffman_code(topNode, nullptr);

    inputFile.close();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
