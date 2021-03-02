// TODO: Create constants file 
// TODO: Use try/catches?
// FIXME: Does webpage only register on open?
import express from 'express';
import ws from 'ws';
import http from 'http';
import { v4 as uuidv4 } from 'uuid';
import path from 'path';

const PORT = 8080;
const WEB_UUID = 'webpage' // Webpage UUID
const CLIENT_CONNECT = 'client_connect';
const IDENTITY_WEBPAGE = 'identity_webpage';
const UPDATE = 'update';
const app = express();
const httpServer = http.createServer(app);
const wss = new ws.Server({ server: httpServer });
let clientMap = new Map(); // UniqueIDs for currently connected clients
var clientUUID; // Holds the UUID for the client

(function main() {
  // Request, response callback function
  // app.get("/", (req, res) => {
  //   //res.send("Hello world");
  //   res.json(products);
  // })

  // app.get("/clients", (req, res) => {
  //   return res.status(200).json({
  //     num_clients: wss.clients.size
  //   });
  // });

  app.use(express.static(path.resolve("./public")))

  wss.on('connection', client => {
    clientUUID = uuidv4(); // Store UUID for this client

    clientMap.set(clientUUID, client);
    console.log('[Server] Client %d connected, UUID=%s', wss.clients.size, clientUUID);

    /* If the webpage has registered, notify of client connection */
    if (clientMap.has(WEB_UUID)) {
      clientMap.get(WEB_UUID).send(JSON.stringify({
        type: UPDATE,
        message: CLIENT_CONNECT,
        size: wss.clients.size
      }))
    }

    client.on ('message', data => {
      console.log('[Server] Received: %s', data);

      /* If client identifies as the webpage */
      if (data === IDENTITY_WEBPAGE) {
        clientMap.set(WEB_UUID, client);
        client.send('[Server] You are the webpage.')
        console.log('[Server] Webpage is UUID=' + clientUUID);
      }
      /* Message was received from the webpage */
      else if (client === clientMap.get('webpage')) {
        client.send('[Server] You are the webpage.');
      }
      /* Otherwise, message was received from a client */
      else {
        clientMap.get(WEB_UUID).send(data);

        switch(data) {
          /* Request a message response with the UUID for this client */
          case 'uuid':
            client.send("[Server] UUID: " + clientUUID);
            break;
          
          case 'custom':
            client.send("[Server] You called the custom message!");
        }
      }
    })

    client.on('close', () => { 
      clientMap.delete(clientUUID);
      console.log('[Server] Client disconnected (Total: %d)', wss.clients.size);
      console.log('Clients Size: %d', clientMap.size);

    /* If the webpage has registered, notify of client disconnection */
    if (clientMap.has(WEB_UUID)) {
      clientMap.get(WEB_UUID).send(JSON.stringify({
        type: UPDATE,
        message: CLIENT_CONNECT,
        size: wss.clients.size
      }))
    }
    
    })
  })

  httpServer.listen(PORT);
})();