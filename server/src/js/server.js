// TODO: Create constants file 
// TODO: Use try/catches?
// FIXME: Does webpage only register on open?
import express from 'express';
import ws from 'ws';
import http from 'http';
import { v4 as uuidv4 } from 'uuid';
import path from 'path';
import * as MESSAGE_TYPES from './messageTypes.js'

const PORT = 8080; // Server listener port
const WEB_UUID = 'webpage' // UUID assigned to the webpage

/* Initialize structures for storing client data */
let clientMap = new Map(); // Maps client UUID to client object
let understandMap = new Map(); // Maps UUID to understanding bool
var clientUUID = 0; // Holds the UUID for the current client
var webpageOnline = false; // Status flag indicating whether the webpage client is online

/* Intialize WS on express HTTP server */
const app = express();
const httpServer = http.createServer(app);
const wss = new ws.Server({ server: httpServer });

(function main() {
  /* Host the HTML in the ./public folder */
  app.use(express.static(path.resolve('../')))

  /* If a client connected */
  wss.on('connection', client => {
    clientUUID = uuidv4(); // Assign a randomized UUID to this client
    clientMap.set(clientUUID, client); // Store the map of this client to UUID
    understandMap.set(clientUUID, true); // Understanding bool for this client is true initially
    
    /* Print client connection status */
    console.log('[Server] Client %d connected, UUID=%s', wss.clients.size, clientUUID);

    /* Update webpage with client connection */
    updateWebpage()

    client.on ('message', data => {

      /* Print message that was received */
      console.log('[Server] Received: %s', data);

      /* If client sends message identifying it as the webpage */
      if (data === MESSAGE_TYPES.WEBPAGE) {
        clientMap.set(WEB_UUID, client); // Set webpage UUID to map to the client

        /* Print webpage connection status */
        console.log('[Server] Webpage is UUID=' + clientUUID);
      }

      /* If message was received from the webpage */
      else if (client === clientMap.get('webpage')) {
        client.send('[Server] You are the webpage.');
      }

      /* Otherwise, message was received from a client */
      else
      {
        switch(data) {
          /* Client sets status to understand */
          case 'understand':
            understandMap.set(clientUUID, true);
            updateWebpage();
            break;

          /* Client sets status to confused */
          case 'confused':
            understandMap.set(clientUUID, false);
            updateWebpage();
            break;

          /* Client requests their UUID */
          case 'uuid':s
            client.send("[Server] UUID: " + clientUUID);
            break;

          /* Client requests their current understanding status */
          case 'status':
            if (understandMap.get(clientUUID))
            {
              client.send("This client understands.");

            }
            else
            {
              client.send("This client is confused.");
            }
            break;
          case 'custom':
            client.send("[Server] You called the custom message!");
            break;
        }
      }
    })

    client.on('close', () => { 
      clientMap.delete(clientUUID);
      console.log('[Server] Client disconnected (Total: %d)', wss.clients.size);
      console.log('Clients Size: %d', clientMap.size);

      /* Update webpage with client disconnection */
      updateWebpage();
    })
  })

  httpServer.listen(PORT);
})();

function understandStatus()
{
  let count = 0;

  for (let uuid of understandMap.keys())
  {
    if (clientMap.get(uuid))
    {
      count++;
    }
  }

  return count;
}

/* If the webpage has registered, send an update to the webpage */
function updateWebpage()
{
  if (clientMap.has(WEB_UUID)) {
    clientMap.get(WEB_UUID).send(JSON.stringify({
      type: MESSAGE_TYPES.UPDATE,
      status: understandStatus(),
      size: wss.clients.size
    })) 
  }

  return 0;
}