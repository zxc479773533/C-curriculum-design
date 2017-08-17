/*
* Part: Graphical User Interface
* Module: Websocket Client
* Author: zxc479773533@github.com
* LICENSE: GNU GENERAL PUBLIC LICENSE V3.0
*/

// websocket object
var ws = new WebSocket('ws://localhost:8080');

// event when the websocket is opened
ws.onopen = function () {
    console.log('Connected to server.');
}

// event when client receive a message
ws.onmessage = function (evt) {
    main(evt.data);
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

function main (data) {
    data = data.replace('\n', '<br/>');
    document.getElementById('console').innerHTML += data;
}