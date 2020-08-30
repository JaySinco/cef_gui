import React from 'react';
import ReactDOM from "react-dom";
import { ipcRender } from "./ipc";

class App extends React.Component {
    public render() {
        return (
            <form>
                <br/><input type="button" onClick={this.sendMessage} value="get_resdir"/>
                <br/><textarea id="res" rows={5} cols={40}/>
            </form>
        );
    }

    public async sendMessage() {
        let respath = await ipcRender.send("get_resdir");
        let res = document.getElementById('res') as HTMLInputElement;
        res.value = respath;
    }
};

ReactDOM.render(<App/>, document.getElementById('container'));


