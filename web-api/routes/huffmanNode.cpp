#include <node.h>
#include <vector>
#include <iostream>
#include <map>
#include <queue>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <iterator>
#include <algorithm>

struct nodes {
    public:
        unsigned char data;
        char* code;
        int sizeCode;
};

using namespace std;

/**
 * Class that creates a heap node for the Huffman tree
 */
class MinHeapNode
{
public:
    char value;	
    int freq;	
    MinHeapNode *left{}, *right{}; 

    /**
     * Constructor function that establishes value and frequency for the node
     * @param value
     * @param freq
     */
    MinHeapNode(char value, int freq)
    {
        this->value = value;
        this->freq = freq;
    }
};

/**
 * priority queue
 */
class compare
{
public:
    bool operator()(MinHeapNode* l, MinHeapNode* r) 
    {
        return (l->freq > r->freq); // Compares if left is larger than right
    }
};

/**
 * Class that includes different functions for the compression of a string via the Huffman algorithm
 */
class HuffmanTree
{
public:
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap; // Creates tree queue
    map<char, int> freq; 
    map<char, string> codes; 
    int size;
    char* data;

    /**
     * Measures the frequency of each character in the input string
     * @param str
     * @param n
     */
    void FreqMeasure()
    {
        //for (char i : str) { // For character in string increase frequency int
        //   freq[i]++;
        //}

        for (int i = 0; i < size; i++) {
            freq[data[i]]++;
        }
    }

    /**
     * Stores the given codes on the tree heap nodes
     * @param root
     * @param str
     */
    void storeCodes(MinHeapNode* root, const string& str)
    {
        if (root == nullptr)  
        {return;}
        if (root->value != '\0') 
        {codes[root->value] = str;} 

        storeCodes(root->left, str + "0");  
        storeCodes(root->right, str + "1"); 
    }

    /**
     * Generates a tree using heap nodes, the priority queue and storing its codes on the codes map
     */
    void generateTree()
    {
        MinHeapNode *left, *right, *top;  
        for (auto &v: freq) {  
            minHeap.push(new MinHeapNode(v.first, v.second));
        }
        while (minHeap.size() != 1)
        {
            left = minHeap.top(); 
            minHeap.pop(); 
            right = minHeap.top(); 
            minHeap.pop(); 
            top = new MinHeapNode('\0', left->freq + right->freq); 
            top->left = left; 
            top->right = right; 
            minHeap.push(top);
        }
        storeCodes(minHeap.top(), ""); 
    }

    /**
     * Helpful function to print out the dictionary
     */
    void printCodes()
    {
        for (auto &code : codes) { 
            printf("%x, ", code.first);
            printf("%s\n ", (code.second).c_str());
            //cout << code.first << ": " << code.second << endl;
        }

        string code;
        for (int i = 0; i < size; i++) {
            code += codes[data[i]];
        }
        cout << code << endl;
    }

    /**
     * Decodes a compressed string using the heap node as a dictionary, returning the original string
     * @param root
     * @param encodedString
     * @return
     */
    static string decode_file(MinHeapNode* root, const string& encodedString)
    {
        string decodedString;
        MinHeapNode* curr = root;
        
        for (char i : encodedString)
        {
            if (i == '0') { 
                curr = curr->left;
            }
            else if (i == '1') {  
                    curr = curr->right;
            }

            if (curr->left == nullptr and curr->right == nullptr) 
            {
                decodedString += curr->value;
                curr = root;
            }
        }
        return decodedString; 
    }
    
    /**
     * @brief Used to read the file we want to compress
     * 
     * @param filename Path to the file
     */
    void readFile(string filename) {
        ifstream file;

        file.open(filename.c_str(), ios::binary | ios::in | ios::ate);

        file.seekg(0, ios::end);
        this->size = file.tellg();
        cout << "File size: " << this->size << endl;
        file.seekg(0, ios::beg);

        data = new char[size];
        file.read(data, size);

        file.close();
    }
};

namespace huffmanNode {
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::Number;
    using v8::Value;
    using v8::String;
    using v8::Context;
    using v8::Array;
    using v8::Integer;

    /**
     * @brief Create a Object (json)
     * 
     * @param compress 
     * @param isolate 
     * @param context 
     * @return Local<Object> 
     */

    Local<Object> createObjectOfData(HuffmanTree* compress, Isolate *isolate, Local<Context> context) {
        Local<Object> object = Object::New(isolate);
        Local<Object> objectTmp = Object::New(isolate);

        int length;
        int i = 0;

        // Diccionario
        for (auto &code : compress->codes) {
            int code0 = code.first;   
            const char* code1 = (code.second).c_str();

            length = (code.second).length();

            Local<Array> arr = Array::New(isolate, length);

            for (int j = 0; j < length; j++) {
                arr->Set(context, j, Integer::New(isolate, (unsigned char)code1[j]));
            }
            
            Local<Array> arr2 = Array::New(isolate, 2);

            arr2->Set(context, 0, Integer::New(isolate, (unsigned char)code0));
            arr2->Set(context, 1, arr);

            objectTmp->Set(context, i, arr2).FromJust();
            i++;
        }

        object->Set(context, String::NewFromUtf8(isolate, "Dictionary").ToLocalChecked(), objectTmp).FromJust();

        // Code
        
        Local<Array> arr = Array::New(isolate, length);
        string code;
        for (int i = 0; i < compress->size; i++) {
            arr->Set(context, i, Integer::New(isolate, (unsigned char)((compress->codes[compress->data[i]]).c_str()[0])));     
        }

        object->Set(context, String::NewFromUtf8(isolate, "Codes").ToLocalChecked(), arr).FromJust();

        object->Set(context, String::NewFromUtf8(isolate, "DictionaryL").ToLocalChecked(), Integer::New(isolate, i)).FromJust();
        object->Set(context, String::NewFromUtf8(isolate, "CodesL").ToLocalChecked(), Integer::New(isolate, compress->size)).FromJust();
        
        return object;
    }


    /**
     * @brief Realiza el huffman a partir del algoritmo huffman
     * 
     * @param args 
     */
    void doHuffman(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        Local<Context> context = isolate->GetCurrentContext();

        v8::String::Utf8Value str(isolate, args[0]);
        string filename(*str);

        
        HuffmanTree *h1 = new HuffmanTree;

        h1->readFile(filename);
        h1->FreqMeasure();
        h1->generateTree();  
        args.GetReturnValue().Set(createObjectOfData(h1, isolate, context));

    }

    /**
     * @brief Create the file with the characters and their codes
     * 
     * @param Dictionary 
     * @param DicL 
     * @param code 
     * @param path 
     */
    void createDecodedFile(nodes* Dictionary, int DicL, string code, string path) {
        ofstream file;

        file.open(path.c_str(),  std::ios::out | std::ios::binary );
        cout << "Decompressing " << endl;

        int tmpSize;

        string tmpCode = "";

        for (int i = 0; i < code.length(); i++) {
            tmpCode += code[i];

            int j = 0;
            bool indexDic = false;
            for (j; j < DicL; j++) { 
                if (strcmp(Dictionary[j].code, tmpCode.c_str()) == 0) {
                    indexDic = true;
                    break;
                } 
            }
            
            data[j] = nodes[codes[i]].data[j];
                //file.write(reinterpret_cast<char*>(nodes[codes[i]].data[j]), sizeof(unsigned char*));
            }
            file.write(reinterpret_cast<char*>(data), tmpSize);
        }
    }

    /**
     * @brief Descomprime el archivo para assí volver a obtener el archivo original
     * 
     * @param args 
     */
    void Decompress(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> Dictionary = args[0]->ToObject(context).ToLocalChecked();
        Local<Integer> DictionaryL = args[1]->ToInteger(context).ToLocalChecked();
        Local<Array> arrCode = Local<Array>::Cast(args[2]);
        Local<Integer> codeL = args[3]->ToInteger(context).ToLocalChecked();
        string path = *String::Utf8Value(isolate, args[4]);
        int lengthDictionary = DictionaryL->Value();

        nodes *dictionaryNodes = new nodes[lengthDictionary];

        // Dictionary
        for (int i = 0; i < lengthDictionary; i++) {
            Local<Array> arr = Local<Array>::Cast(Dictionary->Get(context, i).ToLocalChecked());
            
            Local<Array> arr2 = Local<Array>::Cast(arr->Get(context, 2).ToLocalChecked());
            int lenArr2 = arr2->Length();

            dictionaryNodes[i].code = new char[lenArr2];

            for (int j = 0; j < lenArr2; j++) {
                Local<Value> value = arr2->Get(context, j).ToLocalChecked();
                dictionaryNodes[i].code[j] = (char)value->Uint32Value(context).ToChecked();
                //printf("%x ", dictionaryNodes[i].data[j]);
            }

            dictionaryNodes[i].sizeCode = lenArr2;

            Local<Value> value = arr->Get(context, 1).ToLocalChecked();
            dictionaryNodes[i].data = (unsigned char)value->Uint32Value(context).ToChecked();

        }

        // Codes
        int lengthCode = codeL->Value();

        string codee = "";

        for (int i = 0; i < lengthCode; i++) {
            Local<Value> value = arrCode->Get(context, i).ToLocalChecked();
            codee += (char)value->IntegerValue(context).ToChecked();
            //printf("%d ", codes[i]);
        }

        

    }

    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "huffman", doHuffman);
        NODE_SET_METHOD(exports, "decHuffman", Decompress);
    }

    

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}

/*
int main() {
    //string line;
    //ifstream fileToCompress("example_text.txt");
    //ofstream compressedFile("example_text.huffman");
    //string stringToCompress;

    //string str = "begin";
    //string encodedString, decodedString;
//

    HuffmanTree h1;
    string filename;
    filename = "example_text.txt";

    h1.readFile(filename);

    h1.FreqMeasure(); // Measures frequency for library
    h1.generateTree();  // Creates a Huffman tree

    cout << "Library is: " << endl;
    h1.printCodes();

    for (auto i: stringToCompress) {
        encodedString += h1.codes[i]; // Adds the Huffman compressed codes to the string
    }

    
    cout << endl;
    cout << "Compressed code is: " << endl;
    cout << encodedString << endl;

    cout << endl;
    cout << "Decompressed code is: " << endl;

    decodedString = h1.decode_file(h1.minHeap.top(), encodedString);

    cout << decodedString << endl;
    

    if (compressedFile.is_open()) {
        compressedFile << encodedString;
        compressedFile.close();
    }

    else {
        cout << "Unable to open file" << endl;
    }
    return 0;
}
*/