import React, { Component } from 'react';
import './App.css';
import { Label, Button } from 'react-bootstrap'; 
import DoorOpenerService from './DoorOpenerService';

class App extends Component {
  doorOpener = new DoorOpenerService(this)
  temporarySettings = {}
  state = {
    espStatus : 'connecting',
    timeSettingsAreVisible : true
  }

  displayEspStatus(data) {
    this.setState({
      espStatus : data.status,
      openingTime : data.openingTime,
      closingTime : data.closingTime,
      openingDuration : data.openingDuration,
      closingDuration : data.closingDuration
    })
    this.temporarySettings.openingTime = data.openingTime
    this.temporarySettings.closingTime = data.closingTime
    this.temporarySettings.openingDuration = data.openingDuration
    this.temporarySettings.closingDuration = data.closingDuration
  }

  displaySettingsResult(data) {
    this.setState({
      openingTime : data.openingTime,
      closingTime : data.closingTime,
      openingDuration : data.openingDuration,
      closingDuration : data.closingDuration
    })
    this.hideSettings()
  }

  showSettings() {
    this.setState({
      settingsAreVisible : true
    })
  }

  hideSettings() {
    this.setState({
      settingsAreVisible : false
    })
  }

  saveSettings() {
    this.doorOpener.saveSettings(this.temporarySettings)
  }

  hadleSettingsChange(event) {
    var key = event.target.name
    var value = this.cleanUpValue(event.target.value)
    this.temporarySettings[key] = value
  }

  cleanUpValue(value) {
    value = value.slice(-1) === "s" ? value.slice(0,value.length - 1) : value
    if(value.includes(':'))
      return value
    else
      return Number(value)
  }

  handleCommandButton(event) {
    this.doorOpener.execute(event.target.name)
  }

  render() {
    let renderedPage
    switch (this.state.espStatus) {
      case 'connecting':
        renderedPage = this.renderConnecting()
        break
      case 'connected':
        renderedPage = this.renderNormalState()
        break
      default:
        renderedPage = this.renderConnectionError()
    }
      
    return (
      <div>
        {renderedPage}
      </div>
    );
  }
  
  renderConnecting() {
    return (
      <div className="App">
        <div className="info"><p><Label bsStyle="warning">Checking connection...</Label></p></div>
        <div className="loader"/>
      </div>
    )
  }

  renderConnectionError() {
    return (
      <div className="App">
        <div className="info"><p><Label bsStyle="danger">Not connected</Label></p></div>
      </div>
    )
  }

  renderParameter(image, name, displayName, value) {
    if(this.state.settingsAreVisible)
      return (
        <div className="simple-div">
          <div className="input-group">
            <span className="input-group-addon" id="sizing-addon2"><img alt="sunrise" height="20px" src={"icons8-" + image + "-52.png"}/></span>
            <input type="text" name={name} className="form-control" placeholder={displayName} aria-describedby="sizing-addon2" defaultValue={value} onChange={this.hadleSettingsChange.bind(this)}/>
          </div>
        </div>
      )
    else
      return (
        <div className="simple-div">
        <span>
        <img alt="sunrise" height="30px" src={"icons8-" + image + "-52.png"}/>
        </span>
        <span style={{verticalAlign: "bottom"}}> {displayName}: <b style={{verticalAlign: "bottom"}}>{value}</b></span>
        </div>
      )
  }

  renderButtons() {
    if(this.state.settingsAreVisible)
      return (
        <div>
        <Button bsStyle="primary" onClick={() => {this.saveSettings()}}>Save</Button>
        <Button bsStyle="default" onClick={() => {this.hideSettings()}}>Cancel</Button>
        </div>
      )
    else
      return (
        <Button bsStyle="default" onClick={() => {this.showSettings()}}>Change</Button>
      )
  }

  renderNormalState() {
    return (
      <div className="App">
        <div className="info"><p><Label bsStyle="success">Connected</Label></p></div>
              <div className="btn-group simple-div" role="group" aria-label="...">
                <Button name="open" onClick={event => this.handleCommandButton(event)} bsStyle="primary"><span className="glyphicon glyphicon-arrow-up" aria-hidden="true"/> Open</Button>
                <Button name="close" onClick={event => this.handleCommandButton(event)} bsStyle="primary"><span className="glyphicon glyphicon-arrow-down" aria-hidden="true"/> Close</Button>
              </div>              
              <div className="btn-group simple-div" role="group" aria-label="...">
                <Button name="up"  onClick={event => this.handleCommandButton(event)}><span className="glyphicon glyphicon-arrow-up" aria-hidden="true"/> Move Up</Button>
                <Button name="down" onClick={event => this.handleCommandButton(event)}><span className="glyphicon glyphicon-arrow-down" aria-hidden="true"/> Move Down</Button>
              </div>              
              <Button name="stop" onClick={event => this.handleCommandButton(event)} bsStyle="danger"><span className="glyphicon glyphicon-remove-circle" aria-hidden="true"/> Stop</Button>
        <hr/>
        {this.renderParameter('sunrise','openingTime','Opening Time', this.state.openingTime)}
        {this.renderParameter('sunset','closingTime','Closing Time', this.state.closingTime)}
        <hr/>
        {this.renderParameter('sunrise','openingDuration','Opening Duration', this.state.openingDuration + "s")}
        {this.renderParameter('sunset','closingDuration','Closing Duration', this.state.closingDuration + "s")}
        <div style={{textAlign : "right"}}>
        {this.renderButtons()}
        </div>
      </div>
    )
  }
}


export default App;
