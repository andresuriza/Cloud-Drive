import React from 'react';
import "bootstrap/dist/css/bootstrap.min.css";
import { BrowserRouter, Routes, Route } from "react-router-dom";

import Navbar from "./components/navbar.component";
import LogIn from "./components/logIn.component";
import UploadFile from "./components/uploadFile.component";
import PersonalFiles from "./components/personalFiles.component";

function App() {
  return (
    <BrowserRouter>
      <div className = "container">
      <Navbar fixed = "top"/>
        <br/>   
      <Routes>
        <Route path = "/" element = {<LogIn />} />
        <Route path = "/upload" element = {<UploadFile />} />
        <Route path = "/personal" element = {<PersonalFiles />} />
      </Routes>
      </div>
    </BrowserRouter>
  )
}

export default App;
