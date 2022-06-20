const router = require('express').Router();
let files = require('../models/files.model');
const lzw = require("./build/Release/lzwNode");


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
    files.find();

    

    const json = lzw.lzw("./files/temp.pdf");
    
    console.log(json);
    
    const dictionary = json.body.Dictionary;
    const code = json.body.Code;
    const dictionaryL = json.body.DictionaryL;
    const codeL = json.body.CodeL;

    const newFile = new files({
        dictionary,
        code,
        dictionaryL,
        codeL,
    });

    newFile.save()
    .then(() => res.json('New File Uploaded'))
    .catch(err => res.status(400).json('Error: ' + err));
})

module.exports = router;