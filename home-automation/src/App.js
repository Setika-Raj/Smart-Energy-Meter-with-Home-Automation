import React ,{useState} from 'react'
import {requests} from "./request.js";
import axios from "axios";
import './App.css';
import Button from './Button.js';


function App() {
  
  
  return (
    <div className="App">
      <div className="Header">
       CONTROL BULBS

      </div>
      <div className="Content">
<div className="Row1">
<Button text="BULB ON" type="on"
option={requests.ON}
/>
<Button text="BULB OFF" type="off"
option={requests.OFF}
/>
</div>
<div className="Row2">
  <Button text="AUTOMATE" type="auto"
  option={requests.AUTOMATE}
 />
</div>

      </div>
     
    </div>
  );
}

export default App;
