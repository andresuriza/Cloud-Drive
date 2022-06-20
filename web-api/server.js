const express = require('express');
const cors = require('cors');
const mongoose = require('mongoose');

const app = express();
const port = process.env.PORT || 3000;

app.get('/hello', (req, res) => {
    res.send('Hello World!')
})

app.use(cors());
app.use(express.json());

const uri = 'mongodb+srv://Sapos_Datos2:DanielSapoHP@cluster0.ryokwe7.mongodb.net/?retryWrites=true';
mongoose.connect(uri);
const connection = mongoose.connection;
connection.once('open', () => {
    console.log('Connected to the Data Base')
})

const userRouter = require('./routes/user');
const filesRouter = require('./routes/files');

app.use('/user', userRouter);
app.use('/files', filesRouter);

app.listen(port, () => {
    console.log(`Server is listening on port: ${port}`);
});