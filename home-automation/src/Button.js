import axios from "axios";
import './Button.css'
import React from 'react'
var add;



function Button(props) {

    async function fetchData(){
        var response=await axios.get(props.option)
        console.log(response)
        
        
        return response
        }

if(props.type=="on"){
         add="btn-on";
    }
    else if(props.type=="off"){
add="btn-off";

    }
    else{
        add="auto"
    }

    return (
        
          <button className={`btn ${add}`}
          onClick={()=>
            fetchData()}>
              {props.text} 
              </button>  
        
    )
}

export default Button
