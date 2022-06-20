const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const filesSchema = new Schema({
    fileType: { type: String, required: false },
    fileName: { type: String, required: false },
    user: { type: String, required: false },
    codes: { type: Array, required: false },
    codesL: { type: Number, required: false},
    tuplas: { type: Array, required: false },
    tuplasL: { type: Number, required: false},
    dictionary: { type: Array, required: false },
    dictionaryL: { type: Number, required: false},
    tags: { type: String, required: false},
})

const files = mongoose.model('files', filesSchema);

module.exports = files;