/*
* Part: Graphical User Interface
* Module: Websocket Client
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

// creat websocket
var ws = new WebSocket('ws://localhost:8080');

// event when the websocket is opened
ws.onopen = function () {
    console.log('Connected to server.');
}

// event when client receive a message
ws.onmessage = function (evt) {
    mainReceive(evt.data);
}

// event when error happens
ws.onerror = function () {
    alert('There is something wrong with your network, please check your server.')
    console.log('Error occured.')
}

// event when the websocket is closed
ws.onclose = function () {
    alert('Connection closed.');
    console.log('Connection closed.');
}

// receive data from server
function mainReceive (data) {
    
    data = data.replace('\n', '<br/>');

    /*TO BE WRITE*/

    document.getElementById('console').innerHTML += data;
}

// send request to server
function mainSend () {
    
    var buffer = '';
    
    for (let arg of arguments) {
        if (arguments == '') {
            buffer += ' #';
        }
        else {
            buffer += ' ' + arg;
        }
    }
    ws.send(buffer);
}