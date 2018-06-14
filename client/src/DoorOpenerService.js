import ipAddress from "./ipaddress"

class DoorOpenerService {
    status = 'connecting'

    constructor(component) {
        this.component = component
        var req = new XMLHttpRequest()
        req.addEventListener("load", function() {
            if(this.status === 200)
                component.displayEspStatus(JSON.parse(this.responseText))
            else
                component.displayEspStatus({status : "not connected"})
        })
        req.open("GET", "http://" +  ipAddress + '/connection')
        req.send()
    }

    saveSettings(data) {
        var component = this.component
        var req = new XMLHttpRequest()
        req.addEventListener("load", function() {
            if(this.status === 200)
                component.displaySettingsResult(JSON.parse(this.responseText))
            else
                component.displaySettingsResult({status : "error"})
        })
        req.open("POST", "http://" +  ipAddress + '/settings')
        req.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        req.send(JSON.stringify(data));
    }

    execute(command) {
        var component = this.component
        var req = new XMLHttpRequest()
        req.addEventListener("load", function() {
            console.log(this.responseText)
        })
        req.open("GET", "http://" +  ipAddress + '/' + command)
        req.send()
    }
}

export default DoorOpenerService