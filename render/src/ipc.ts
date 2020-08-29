type OnReceiveCallback = (body: any) => void;
type OnSendSuccessFunc = (response: string) => void;
type OnSendFailedFunc = (err_code: string, err_msg: string) => void;

interface CefQueryArg {
    request: string,
    onSuccess: OnSendSuccessFunc;
    onFailure: OnSendFailedFunc;
}

declare global {
    interface Window {
        cefQuery: (arg: CefQueryArg) => void;
        onReceiveIpcMessage: (request: string) => void; 
    }
}

class IpcRender {
    static ipcMapping: Map<string, OnReceiveCallback> = new Map;

    public on(header: string, callback: OnReceiveCallback) {
        IpcRender.ipcMapping.set(header, callback);
    }

    public off(header: string) {
        IpcRender.ipcMapping.delete(header);
    }

    public clear() {
        IpcRender.ipcMapping.clear();
    }

    static onReceiveIpcMessage(request: string): void {
        const parts = request.split(": ");
        if (parts.length !== 2) {
            console.error(`invalid ipc message: ${request}`);
            return;
        }
        if (!IpcRender.ipcMapping.has(parts[0])) {
            console.error(`unregistered ipc message header: ${parts[0]}`);
            return;
        }
        IpcRender.ipcMapping.get(parts[0])!(JSON.parse(parts[1]));
    }

    public send(header: string, body: any, onSuccess: OnSendSuccessFunc, 
        onFailure: OnSendFailedFunc)
    {
        window.cefQuery({
            request: `${header}: ${JSON.stringify(body)}`,
            onSuccess,
            onFailure,
        });
    }
}

window.onReceiveIpcMessage = IpcRender.onReceiveIpcMessage;

export let ipcRender = new IpcRender();