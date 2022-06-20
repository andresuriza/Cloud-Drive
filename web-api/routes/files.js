const router = require('express').Router();
let files = require('../models/files.model');
const lzw = require("./build/Release/lzwNode");
const lz78 = require("./build/Release/lz78Node");


router.route('/').get((req, res) => {
    files.find()
    .then(files => res.json(files))
    .catch(err => res.status(400).json('Error: ' + err));
});

router.route('/add').post((req, res) => {
    const fileType = req.body.fileType;
    const fileName = req.body.fileName;
    const user = req.body.user;

    const newFile = new files({
        fileType,
        fileName,
        user,
    });

    newFile.save()
    .then(() => res.json('New File Uploaded'))
    .catch(err => res.status(400).json('Error: ' + err));
});

router.route('/compressLZW').get((req, res) => {
    const json = lzw.lzw("./files/temp.pdf");
    
    console.log(json);
    
    const dictionary = json.Dictionary;
    const codes = json.Code;
    const dictionaryL = json.DictionaryL;
    const codesL = json.CodeL;

    const newFile = new files({
        dictionary,
        codes,
        dictionaryL,
        codesL,
    });

    newFile.save()
    .then(() => res.json('New File Uploaded'))
    .catch(err => res.status(400).json('Error: ' + err));
})

router.route('/compressLZ78').get((req, res) => { 
    const json = lzw.lzw("./files/test.gif");

    console.log(json);

    const dictionary = json.Dictionary;
    const tuplas = json.Tuplas;
    const dictionaryL = json.DictionaryL;
    const tuplasL = json.TuplasL;

    const newFile = new files({
        dictionary,
        tuplas,
        dictionaryL,
        tuplasL,
    });

    newFile.save()
    .then(() => res.json('New File Uploaded'))
    .catch(err => res.status(400).json('Error: ' + err));

})

module.exports = router;