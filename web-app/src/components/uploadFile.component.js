import React, { Component } from 'react';
import axios from 'axios';

/*
import {compressLZW} from "./compression";

function compress(path) {
    const json = compressLZW(path);
    return json;
}
*/

export default class UploadFile extends Component {
    constructor(props) {
        super(props);

        this.onChangeFileName = this.onChangeFileName.bind(this);
        this.onChangeFileType = this.onChangeFileType.bind(this);
        this.onSubmit = this.onSubmit.bind(this);

        this.state = {
            fileType: "",
            fileName: "",
            user: "",
        }
    }

    onChangeFileName(e) {
        this.setState({
            fileName: e.target.value
        });
    }

    onChangeFileType(e) {
        this.setState({
            fileType: e.target.value
        });
    }

    onSubmit(e) {
        e.preventDefault();

        const newFile = {
            fileType: this.state.fileType,
            fileName: this.state.fileName,
            user: this.state.user
        }

        const path = this.state.fileName;
        console.log(path);

        if (path === "pdf") {
            axios.get('http://localhost:3000/files/compressLZW')
            .then(res => console.log(res.data))
            .catch(error => console.log(error));
        } else if (path === "gif") {
            axios.get('http://localhost:3000/files/compressLZ78')
            .then(res => console.log(res.data))
            .catch(error => console.log(error));
        }

        

        
    }

    render() {
        return (
            <div>
                <h3>Upload File</h3>
                <form onSubmit = {this.onSubmit}>
                    <div className = "form-group">
                        <label>File Name: </label>
                        <input type = "text"
                            required
                            className = "form-control"
                            value = {this.state.fileName}
                            onChange = {this.onChangeFileName}
                            />
                    </div>
                    <div className="mb-3">
                        <label htmlFor="formFile" className="form-label">Selesct the desired file</label>
                        <input className="form-control" type="file" id="formFile" 
                        onChange = {this.onChangeFileType} />
                    </div>
                    <div className = "form-group">
                        <input type = "submit" value = "Upload File" className = "btn btn-primary" />
                    </div>
                </form>
            </div>
        )
    }
}