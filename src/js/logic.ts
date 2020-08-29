import { ipcRender } from "./ipc";

function sendMessage() {
    ipcRender.send(
        "IpcFromJsDemo",
        (<HTMLInputElement>document.getElementById("message")).value,
        (response) => {
            let res1 = document.getElementById('res1') as HTMLInputElement;
            res1.value = 'Response: '+response;
        },
        (error_code, error_message) => {
            console.log(error_code, error_message)
        }
    );
}

ipcRender.on("IpcToJsDemo", (body: string) => {
    let res2 = document.getElementById('res2') as HTMLInputElement;
    res2.value = 'Request body: '+ body;
});