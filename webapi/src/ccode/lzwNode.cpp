#include <node.h>

#include <string>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <fstream>

using namespace std;

struct nodes {
    public:
        unsigned char* data;
        int size;
};


class lzwCompressor {
private:
    string *dictionary;
    int dici = 0;
    int *code;
    int codei = 0;
    char *data;
    size_t size;

public:

    string* getDictionary() { return this->dictionary; }
    int getDictionaryI() { return this->dici; }
    int* getCode() { return this->code; }
    int getCodeI() { return this->codei; }
    size_t getSize() { return this->size; }

    bool searchInDictionary(string search, int chars) {
        bool ret = false;
        for (int i = 0; i < size && !ret; i++) {
            if (dictionary[i].length() == chars) {
                if (dictionary[i] == search) {
                    ret = true;
                    break;
                } else if (dictionary[i] == "") {
                    break;
                } 
            } 
        }
        return ret;
    }

    int indexInDictionary(string search, int chars) {
        int ret = -1;
        for (int i = 0; i < size && ret == -1; i++) {
            if (dictionary[i].length() == chars) {
                if (dictionary[i] == search) {
                    ret = i;
                    break;
                } else if (dictionary[i] == "") {
                    break;
                } 
            } 
        }
        return ret;
    }

    void compress(string filename) {
        readFile(filename);
        initCharacters();
        
        string p;
        string c;
        string pc;

        p = data[0];      

        for (int i = 1; i < size; i++) {
            c = data[i];
            
            pc = p;
            pc += c;
            
            if (searchInDictionary(pc, pc.length())) {
                p = "";
                p += pc;

            } else {
                int codeDic = indexInDictionary(p, p.length());
                code[codei] = codeDic;
                codei++;

                dictionary[dici] = pc;
                dici++;

                p = "";
                p += c;

            }
            c = "";    
            pc = "";        
        }




        
    }

    void initCharacters() {
        bool put = true;
        bool end = false;
        string character;
        for (int i = 0; i < size && !end; i++) {
            character += data[i];

            for (int j = 0; j < dici + 1 && put; j++) {
                if (dictionary[j] == character) {
                    put = false;  
                }
            }

            if (put) {
                dictionary[dici] = character;
                dici++;
            }
            put = true;
            character = "";
        }

    }

    void readFile(string filename) {
        std::ifstream file;
        
        file.open(filename.c_str(), std::ios::binary | std::ios::in | std::ios::ate);

        file.seekg(0, std::ios::end);
        size = file.tellg();
        file.seekg(0, std::ios::beg);

        data = new char[size];
        file.read(data, size);

        code = new int[size];
        for (int i = 0; i < size; i++) {
            code[i] = -1;
        }

        dictionary = new string[size];
        
        file.close();
    }

    void Decompress(string path, nodes* nodes, int nodesL, int* codes, int codesL) {
        ofstream file;

        file.open(path.c_str(),  std::ios::out | std::ios::binary );

        cout << "Decompressing " << endl;

        int tmpSize;
        for (int i = 0; i < codesL; i++) {
            tmpSize = nodes[codes[i]].size;
            unsigned char data[tmpSize];
            for (int j = 0; j < tmpSize; j++) { 
                //printf("%x ", nodes[codes[i]].data[j]);
                data[j] = nodes[codes[i]].data[j];
                //file.write(reinterpret_cast<char*>(nodes[codes[i]].data[j]), sizeof(unsigned char*));
            }
            file.write(reinterpret_cast<char*>(data), tmpSize);
        }
    };

};



namespace lzwNode {
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
    //using node;

    
    Local<Object> createObjectOfData(lzwCompressor* compress, Isolate *isolate, Local<Context> context) {
        Local<Object> object = Object::New(isolate);
        Local<Object> objectTmp = Object::New(isolate);

        int length;

        // Diccionario
        for (int i = 0; i < compress->getDictionaryI(); i++) {
            const char *tmp = compress->getDictionary()[i].c_str();   
            length = compress->getDictionary()[i].length();

            Local<Array> arr = Array::New(isolate, length);

            for (int j = 0; j < length; j++) {
                arr->Set(context, j, Integer::New(isolate, (unsigned char)tmp[j]));
            }

            objectTmp->Set(context, i, arr).FromJust();
        }
        object->Set(context, String::NewFromUtf8(isolate, "Dictionary").ToLocalChecked(), objectTmp).FromJust();

        length = compress->getCodeI();

        // Código 
        Local<Array> arr = Array::New(isolate, length);
        for (int i = 0; i < length; ++i) {
            arr->Set(context, i, Integer::New(isolate, compress->getCode()[i]));
        }
        object->Set(context, String::NewFromUtf8(isolate, "Code").ToLocalChecked(), arr).FromJust();

        // Tamaño diccionario
        auto dici = Number::New(isolate, compress->getDictionaryI());
        object->Set(context, String::NewFromUtf8(isolate, "DictionaryL").ToLocalChecked(), dici).FromJust();

        // Tamaño código 
        auto codei = Number::New(isolate, compress->getCodeI());
        object->Set(context, String::NewFromUtf8(isolate, "CodeL").ToLocalChecked(), codei).FromJust();

        return object;
        
    };

    void doLZW(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();

        Local<Context> context = isolate->GetCurrentContext();

        v8::String::Utf8Value str(isolate, args[0]);

        string filename(*str);

        lzwCompressor *lzw = new lzwCompressor;
        lzw->compress(filename);

        args.GetReturnValue().Set(createObjectOfData(lzw, isolate, context));   

        
        //arr->Set(context, 0, Integer::New(isolate, 1));
        //arr->Set(context, 1, String::NewFromUtf8(isolate, "oo").ToLocalChecked());
        //arr->Set(context, 2, String::NewFromUtf8(isolate, "data").ToLocalChecked());

        
        delete lzw;
    }

    void doDecompress(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        Local<Context> context = isolate->GetCurrentContext();

        // Diccionario
        Local<Object> Dictionary = args[0]->ToObject(context).ToLocalChecked();
        Local<Integer> DictionaryL = args[1]->ToInteger(context).ToLocalChecked();
        int lengthDictionary = DictionaryL->Value();

        nodes *dictionaryNodes = new nodes[lengthDictionary];

        for (int i = 0; i < lengthDictionary; i++) {
            Local<Array> arr = Local<Array>::Cast(Dictionary->Get(context, i).ToLocalChecked());
            int lenArr = arr->Length();

            dictionaryNodes[i].data = new unsigned char[lenArr];

            for (int j = 0; j < lenArr; j++) {
                Local<Value> value = arr->Get(context, j).ToLocalChecked();
                dictionaryNodes[i].data[j] = (unsigned char)value->Uint32Value(context).ToChecked();
                //printf("%x ", dictionaryNodes[i].data[j]);
            }

            dictionaryNodes[i].size = lenArr;        
        }

    
        // Código 
        Local<Array> arrCode = Local<Array>::Cast(args[2]);
        Local<Integer> codeL = args[3]->ToInteger(context).ToLocalChecked();
        int lengthCode = codeL->Value();

        int *codes = new int[lengthCode];

        for (int i = 0; i < lengthCode; i++) {
            Local<Value> value = arrCode->Get(context, i).ToLocalChecked();
            codes[i] = (int)value->IntegerValue(context).ToChecked();
            //printf("%d ", codes[i]);
        }

        Local<Value> pathJ = args[4];

        args.GetReturnValue().Set(args[4]);
        string path = *String::Utf8Value(isolate, pathJ);

        cout << path << endl;

        lzwCompressor lzw;
        lzw.Decompress(path, dictionaryNodes, lengthDictionary, codes, lengthCode);

        //args.GetReturnValue().Set(args[3]);
        
    }

    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "lzw", doLZW);
        NODE_SET_METHOD(exports, "decLzw", doDecompress);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
}


