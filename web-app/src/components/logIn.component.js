import React, { Component } from 'react';
import axios from 'axios';

export default class LogIn extends Component {
    constructor(props) {
        super(props);

        this.onChangeUsername = this.onChangeUsername.bind(this);
        this.onChangePassword = this.onChangePassword.bind(this);
        this.onSubmit = this.onSubmit.bind(this);

        this.state = {
            username: "",
            password: "",
            files: [],
        }
    }

    onChangeUsername(e) {
        this.setState({
            username: e.target.value
        });
    }

    onChangePassword(e) {
        this.setState({
            password: e.target.value
        });
    }

    onSubmit(e) {
        e.preventDefault();

        const newUser = {
            username: this.state.username,
            password: this.state.password,
            files: this.state.files
        }

        axios.post('http://localhost:3000/user/add', newUser)
                .then(res => console.log(res.data))
                .catch(error => console.log(error));

                console.log("You are loged in as "  + this.state.username);

                console.log(newUser);

        /*
                axios.get('http://localhost:3000/user/')
        .then(response => {
            if (response.data.lenght = 0) {
                axios.post('http://localhost:3000/user/add', newUser)
                .then(res => console.log(res.data))
                .catch(error => console.log(error));

                console.log("You are loged in as "  + this.state.username);

                console.log(newUser);
            }
            if (response.data.lenght > 0) {
                var n = response.data.lenght;
                var i = 0;
                while (i < n) {
                    if (response.data.username[i] = this.state.username) {
                        if (response.data.username[i] = this.state.password) {
                            console.log("You are loged in as "  + this.state.username);
                            break 
                        }
                        else {
                            console.log("This Username is already taken, please use another one");
                            break
                        }   
                    } 
                    else {
                        i++;
                    }
                }
                axios.post('http://localhost:3000/user/add', newUser)
                .then(res => console.log(res.data))
                .catch(error => console.log(error));

                console.log("You are loged in as "  + this.state.username);

                console.log(newUser);
                
            }
        })
        */


    }

    render() {
        return (
            <div>
                <h3>Log In</h3>
                <form onSubmit = {this.onSubmit}>
                    <div className = "form-group">
                        <label>Username: </label>
                        <input type = "text"
                            required
                            className = "form-control"
                            value = {this.state.username}
                            onChange = {this.onChangeUsername}
                            />
                    </div>
                    <div className = "form-group">
                        <label>Password: </label>
                        <input type = "password"
                            required
                            style = {StyleSheet.hiddenInput}
                            className = "form-control"
                            value = {this.state.password}
                            onChange = {this.onChangePassword}
                            />
                    </div>
                    <div className = "form-group">
                        <input type = "submit" value = "Log In" className = "btn btn-primary" />
                    </div>
                </form>
            </div>
        )
    }
}