const lzw = require('./build/Release/lzwNode');
const lz78 = require('./build/Release/lz78Node');

//                   path: ../files/temp.pdf
function compressLZW(path) {
    const json = lzw.lzw(path);
    return json;
}

//                           outPath: ../outfiles/temp.pdf"
function decompressLZW(json, outPath) {
    lzw.decLzw(json.Dictionary, json.DictionaryL, json.Code, json.CodeL, outPath)
}

//                    path: ../files/temp.gif
function compressLZ78(path) {
    const json = lz78.lz78(path);
    return json;
}
//                            outPath: ../outfiles/temp.gif"
function decompressLZ78(json, outPath) {
    lz78.decLz78(json.Dictionary, json.DictionaryL, json.Tuplas, json.TuplasL, outPath);
}

const comLZW = compressLZW("../files/temp.pdf");
decompressLZW(comLZW, "../outfiles/temp.pdf");