const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const userSchema = new Schema({
    username: { type: String, required: true, trim: true },
    password: { type: String, required: true, trim: true },
    files: { type: Array, required: false },
})

const user = mongoose.model('user', userSchema);

module.exports = user;