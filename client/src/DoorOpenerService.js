import ipAddress from "./ipaddress"

class DoorOpenerService {
    status = 'connecting'

    constructor(component) {
        this.component = component
        var req = new XMLHttpRequest()
			  req.onreadystatechange = () => {
            if(req.status === 200)
                component.displayEspStatus(JSON.parse(req.responseText))
            else
                component.displayEspStatus({status : "not connected"})
				}
        req.open("GET", "http://" +  ipAddress + '/connection')
        req.send()
    }

    saveSettings(data) {
        var component = this.component
        var req = new XMLHttpRequest()
				req.onreadystatechange = () => {
            if(req.status === 200)
                component.displayEspStatus(JSON.parse(req.responseText))
            else
                component.displayEspStatus({status : "not connected"})
				}
        req.open("POST", "http://" +  ipAddress + '/settings')
        req.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        req.send(JSON.stringify(data));
    }

    execute(command) {
        var req = new XMLHttpRequest()
        req.addEventListener("load", function() {
            console.log(this.responseText)
        })
        req.open("GET", "http://" +  ipAddress + '/' + command)
        req.send()
    }
}

export default DoorOpenerService
