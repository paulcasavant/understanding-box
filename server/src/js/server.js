import express from 'express';
import ws from 'ws';
import http from 'http';
import { v4 as uuidv4 } from 'uuid';
import path from 'path';
import * as MESSAGE_TYPES from './messageTypes.js'

const PORT = 8080; // Port to listen on
const WEB_UUID = 'webpage' // UUID to be assigned to the webpage

/* Initialize structures for storing client data */
let clientUUIDMap = new Map(); // Maps client to UUID
let understandMap = new Map(); // Maps UUID to understanding bool

/* Intialize WS on express HTTP server */
const app = express();
const httpServer = http.createServer(app);
const wss = new ws.Server({ server: httpServer });

(function main() {
  /* Host the HTML in the ./public folder */
  app.use(express.static(path.resolve('../')))

  /* If a client connected */
  wss.on('connection', client => {
    clientUUIDMap.set(client, uuidv4()); // Store the map of this client to UUID
    understandMap.set(client, true); // Maps client to understanding bool 
    
    /* Update webpage with client connection */
    updateWebpage()
 
    /* Print client connection status */
    console.log('[Server] Client %d connected, UUID=%s', wss.clients.size, clientUUIDMap.get(client));

    /* Handler for receiving message */
    client.on ('message', data => {

      /* Print message that was received */
      console.log('[Server] Received: %s', data);

      /* If client sends message identifying it as the webpage */
      if (data === MESSAGE_TYPES.WEBPAGE) {
        understandMap.delete(client); // Remove from understandMap since it is the webpage
        console.log("size after: %d", understandMap.size)
        clientUUIDMap.set(WEB_UUID, client) // Correspond webpage UUID to client 
        clientUUIDMap.set(client, WEB_UUID); // Correspond client to webpage UUID

        updateWebpage(); // Send first update to webpage

        /* Print webpage connection status */
        console.log('[Server] Webpage connected.');
      }

      /* If message was received from the webpage */
      else if (client === clientUUIDMap.get(WEB_UUID)) {
        client.send('[Server] You are the webpage.');
      }

      /* Otherwise, message was received from a client */
      else
      {
        /* Switch for handling different message types */
        switch(data) {
          /* Client sets status to understand */
          case 'understand':
            understandMap.set(client, true);
            updateWebpage();
            break;

          /* Client sets status to confused */
          case 'confused':
            understandMap.set(client, false);
            updateWebpage();
            break;

          /* Client requests their UUID */
          case 'uuid':
            client.send("[Server] UUID: " + clientUUIDMap.get(client));
            break;

          /* Client requests their current understanding status */
          case 'status':
            if (understandMap.get(client))
            {
              client.send("This client understands.");

            }
            else
            {
              client.send("This client is confused.");
            }
            break;
        }
      }
    })

    client.on('close', () => { 
      understandMap.delete(client);
      clientUUIDMap.delete(client);
      console.log('[Server] Client disconnected (Total: %d)', wss.clients.size);

      /* Update webpage with client disconnection */
      updateWebpage();
    })
  })

  httpServer.listen(PORT);
})();

/**
 * Returns the number of clients that currently understand.
 */
function numUnderstand()
{
  var count = 0;

  for (let aClient of understandMap.keys())
  {
    if (understandMap.get(aClient))
    {
      count++;
    }
  }

  return count;
}

/**
 * Returns the number of clients that understand as a percentage.
 */
function percentUnderstand()
{
  var percentage = 0;

  /* If there are clients other than the webpage */
  if (wss.clients.size > 1)
  {
    return parseFloat(numUnderstand() / (wss.clients.size - 1) * 100).toFixed(2) + "%"
  }

  return percentage;
}

/**
 * Sends an update to the webpage with the latest data.
 */
function updateWebpage()
{
  if (clientUUIDMap.has(WEB_UUID)) {
    clientUUIDMap.get(WEB_UUID).send(JSON.stringify({
      type: MESSAGE_TYPES.UPDATE,
      status: percentUnderstand(),
      size: wss.clients.size - 1 // Minus 1 to adjust for count of webpage
    })) 
  }

  return 0;
}