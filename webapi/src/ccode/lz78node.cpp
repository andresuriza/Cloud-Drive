#include <node.h>

#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include "linkedList.cpp"

using namespace std;

struct nodes2 {
    public:
        unsigned char* data;
        int size;
};

struct TuplaSimple {
    public:
        unsigned char data;
        int index;
};

class lz78Compressor
{
private:
    LinkedList *dictionary = new LinkedList;
    LinkedList *tuplas = new LinkedList;
    char *data;
    size_t size;
    int current_index = 0;

public:
    LinkedList* getDictionary() { return dictionary; }
    LinkedList* getTuplas() { return tuplas; }
    

    void compress()
    {
        dictionary->addData("", 0);
        current_index++;
        string tmp;
        string toTupla;
        int next= 0;
        int x = 0;
        tmp = (unsigned char)this->data[0];
        int length = this->size;
        int contain = dictionary->contains(tmp);
        while (x < length)
        {
            contain = dictionary->contains(tmp);
            toTupla = (unsigned char)this->data[x];

            if (contain == -1)
            {
                dictionary->addData(tmp, current_index);
                tuplas->addData(toTupla, next);

                current_index++;
                x++;

                tmp = (unsigned char)this->data[x];
                next = 0;
            }
            else if (contain != -1)
            {
                next = contain;
                tmp += (unsigned char)this->data[x + 1];
                toTupla = (unsigned char)this->data[x + 1];
                x++;
            }
        }

    }

    void tuplastxt(string filename){
        fstream file;
        file.open(filename,std::ios_base::out);
        for (int i = 0; i <tuplas->getSize(); i++){
            file<<"(";
            file<<tuplas->getIndex(i);
            file<<",";
            file<<tuplas->getData(i);
            file<<")\n";
        }
        file.close();
    }

    void dictionarytxt(string filename){
        fstream file;
        file.open(filename,std::ios_base::out);
        for (int i = 0; i <dictionary->getSize(); i++){
            file<<dictionary->getIndex(i);
            file<<"-";
            file<<dictionary->getData(i);
            file<<"\n";
        }
        file.close();
    }

    void decompress(){
        string decompressed;
        for (int i = 0; i < tuplas->getSize(); i++)
        {
            decompressed+= dictionary->getData(tuplas->getIndex(i)) + tuplas->getData(i) ;
        }
        cout <<endl<<endl<<"EL ARCHIVO DESCOMPRIMIDO ES: "<< decompressed <<endl;
    }

  void createGIF(){
        ofstream file;

        file.open("out/fuck-you.gif",  std::ios::out | std::ios::binary );

        string giff;
          for (int i = 0; i < tuplas->getSize(); i++)
        {
            //gif << dictionary->getData(tuplas->getIndex(i)) + tuplas->getData(i) ;
            giff += dictionary->getData(tuplas->getIndex(i)) + tuplas->getData(i);

            unsigned char temp[giff.length()];

            

            for (int j = 0; j < giff.length(); j++) {
                temp[j] = (unsigned char)giff.c_str()[j];
                cout << giff.length() << endl;
                
            }

            file.write(reinterpret_cast<char*>(temp), giff.length());

            giff = "";
        }      
        
    }

    void readFile(string filename)
    {
        std::ifstream file;

        file.open(filename.c_str(), std::ios::binary | std::ios::in | std::ios::ate);

        file.seekg(0, std::ios::end);
        this->size = file.tellg() + 1;
        file.seekg(0, std::ios::beg);

        data = new char[this->size ];
        file.read(data, this->size); 

        file.close();
        this->printdata(this->size); 
    }

    void printdata(int size)
    {
        for (int i = 0; i < size; i++)
        {
            //printf("%X ", (unsigned char)this->data[i]);
            cout<<this->data[i];
        }
        cout<<endl;
    }

    void Decompress(string path, nodes2* nodes, int nodesL, TuplaSimple *tupla, int tuplaL) {
        ofstream file;

        file.open(path.c_str(),  std::ios::out | std::ios::binary );

        cout << "Decompressing " << endl;

        int tmpSize;

        for (int i = 0; i < tuplaL - 1; i++) {
            tmpSize = nodes[tupla[i].index].size;
            unsigned char data[tmpSize + 1];

            for (int j = 0; j < tmpSize; j++) { 
                //printf("%x ", nodes[codes[i]].data[j]);
                data[j] = nodes[tupla[i].index].data[j];
                //file.write(reinterpret_cast<char*>(nodes[codes[i]].data[j]), sizeof(unsigned char*));
            }

            data[tmpSize] = tupla[i].data;
            file.write(reinterpret_cast<char*>(data), tmpSize + 1);
        }
    };
};

namespace lz78Node {
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

    Local<Object> createObjectOfData(lz78Compressor *compress, Isolate *isolate, Local<Context> context) {
        Local<Object> object = Object::New(isolate);
        Local<Object> objectTmp = Object::New(isolate);

        int length;

        // Diccionario
        for (int i = 0; i < compress->getDictionary()->getSize(); i++) {
            const char *tmp = compress->getDictionary()->getData(i).c_str();   
            length = compress->getDictionary()->getData(i).length();

            Local<Array> arr = Array::New(isolate, length);

            for (int j = 0; j < length; j++) {
                arr->Set(context, j, Integer::New(isolate, (unsigned char)tmp[j]));
            }

            objectTmp->Set(context, i, arr).FromJust();
        }
        object->Set(context, String::NewFromUtf8(isolate, "Dictionary").ToLocalChecked(), objectTmp).FromJust();

        length = compress->getTuplas()->getSize();

        // Tuplas
        Local<Object> objectTmp2 = Object::New(isolate);

        for (int i = 0; i < compress->getTuplas()->getSize(); i++) {
            const char *tmp = compress->getTuplas()->getData(i).c_str();   
            length = compress->getTuplas()->getData(i).length();

            Local<Array> arrTupla = Array::New(isolate, length);

            arrTupla->Set(context, 0, Integer::New(isolate, (int)compress->getTuplas()->getIndex(i)));
            const char* tmp2 = compress->getTuplas()->getData(i).c_str();
            arrTupla->Set(context, 1, Integer::New(isolate, (unsigned char)tmp2[0]));

            objectTmp2->Set(context, i, arrTupla).FromJust();
        }
        object->Set(context, String::NewFromUtf8(isolate, "Tuplas").ToLocalChecked(), objectTmp2).FromJust();

        length = compress->getTuplas()->getSize();

        // Tamaño diccionario
        auto dici = Number::New(isolate, compress->getDictionary()->getSize());
        object->Set(context, String::NewFromUtf8(isolate, "DictionaryL").ToLocalChecked(), dici).FromJust();

        // Tamaño Tuplas
        auto codei = Number::New(isolate, compress->getTuplas()->getSize());
        object->Set(context, String::NewFromUtf8(isolate, "TuplasL").ToLocalChecked(), codei).FromJust();

        return object;
        
    };

    void doLZ78(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();

        Local<Context> context = isolate->GetCurrentContext();

        // Nombre del archivo
        v8::String::Utf8Value str(isolate, args[0]);
        string filename(*str);

        // Comprimir
        lz78Compressor *lz78 = new lz78Compressor;
        lz78->readFile(filename);
        lz78->compress();

        // Hacer el objeto y retornarlo
        args.GetReturnValue().Set(createObjectOfData(lz78, isolate, context));   
        
        delete lz78;
    }

    void doDecompress(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        Local<Context> context = isolate->GetCurrentContext();

        // Diccionario
        Local<Object> Dictionary = args[0]->ToObject(context).ToLocalChecked();
        Local<Integer> DictionaryL = args[1]->ToInteger(context).ToLocalChecked();
        int lengthDictionary = DictionaryL->Value();

        nodes2 *dictionaryNodes = new nodes2[lengthDictionary];

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
        Local<Object> Tuplas = args[2]->ToObject(context).ToLocalChecked();
        Local<Integer> TuplasL = args[3]->ToInteger(context).ToLocalChecked();

        //Local<Array> arrCode = Local<Array>::Cast(args[2]);
        //Local<Integer> codeL = args[3]->ToInteger(context).ToLocalChecked();
        int lengthCode = TuplasL->Value();

        TuplaSimple *tuplasNode = new TuplaSimple[lengthCode];

        for (int i = 0; i < lengthCode; i++) {
            Local<Array> arr = Local<Array>::Cast(Tuplas->Get(context, i).ToLocalChecked());

            Local<Value> value1 = arr->Get(context, 0).ToLocalChecked();
            Local<Value> value2 = arr->Get(context, 1).ToLocalChecked();

            tuplasNode[i].index = (int)value1->IntegerValue(context).ToChecked();
            tuplasNode[i].data = (unsigned char)value2->Uint32Value(context).ToChecked();

            
        }

        lz78Compressor lz78;

        lz78.Decompress("out/notaaaas.gif", dictionaryNodes, lengthDictionary, tuplasNode, lengthCode);
        

        //args.GetReturnValue().Set(args[2]);


        
        //lz78.Decompress("out/punto.pdf", dictionaryNodes, lengthDictionary, codes, lengthCode);

        //args.GetReturnValue().Set(args[2]);
        
    }

    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "lz78", doLZ78);
        NODE_SET_METHOD(exports, "decLz78", doDecompress);
    }

    

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
};