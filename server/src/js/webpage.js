import * as MESSAGE_TYPES from './messageTypes.js'

// TODO: Client count will not register when webpage has to reconnect
const ws = new WebSocket("ws://localhost:8080");

ws.onopen = function(event) {
    console.log('[Webpage] Connected to WebSocket');
    ws.send(MESSAGE_TYPES.WEBPAGE);
};

ws.onmessage = function(event) {
    //document.getElementById('title').innerHTML = JSON.parse(event.data).type;
    //document.getElementById('custom').innerHTML = JSON.parse(event.data).type;

    // TODO: Probably need error trapping here, showing error in webpage console
    /* Switch between message types */
    switch(JSON.parse(event.data).type)
    {
        case 'update':
            document.getElementById('num_clients').innerHTML = "Clients: " + JSON.parse(event.data).size;
            document.getElementById('understanding').innerHTML = "Understanding: " + JSON.parse(event.data).status;
        break;
    }
}