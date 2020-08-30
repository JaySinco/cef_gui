import React from 'react';
import ReactDOM from "react-dom";
import { ipcRender } from "./ipc";

class App extends React.Component {
    public render() {
        return (
            <form>
                Message: <input type="text" id="message" value="My Message" onChange={()=>{}}/>
                <br/><input type="button" onClick={this.sendMessage} value="Send Message" readOnly={true}/>
                <br/>You should see the reverse of your message below:
                <br/><textarea id="res1" rows={5} cols={40}/>
                <br/><textarea id="res2" rows={5} cols={40}/>
            </form>
        );
    }

    public sendMessage() {
        ipcRender.send(
            "IpcFromJsDemo",
            (document.getElementById("message") as HTMLInputElement).value,
            (response) => {
                let res1 = document.getElementById('res1') as HTMLInputElement;
                res1.value = 'Response: '+response;
            },
            (error_code, error_message) => {
                console.log(error_code, error_message)
            }
        );
    }
};

ipcRender.on("IpcToJsDemo", (body: string) => {
    let res2 = document.getElementById('res2') as HTMLInputElement;
    res2.value = 'Request body: '+ body;
});

ReactDOM.render(<App/>, document.getElementById('container'));


