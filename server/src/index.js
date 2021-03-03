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
let understandMap = new Map(); // Tracks whether a client understands or not
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
    understandMap.set(clientUUID, true);
    console.log('[Server] Client %d connected, UUID=%s', wss.clients.size, clientUUID);

    /* If the webpage has registered, notify of client connection */
    if (clientMap.has(WEB_UUID)) {
      clientMap.get(WEB_UUID).send(JSON.stringify({
        type: UPDATE,
        status: understandStatus(),
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
        clientMap.get(WEB_UUID).send(JSON.stringify({
          type: UPDATE,
          status: understandStatus(),
          size: wss.clients.size
        }))

        switch(data) {
          /* Client sets status to understand */
          case 'understand':
            understandMap.set(clientUUID, true);
            break;

          /* Client sets status to confused */
          case 'confused':
            understandMap.set(clientUUID, false);
            break;

          /* Client requests their UUID */
          case 'uuid':
            client.send("[Server] UUID: " + clientUUID);
            break;

          /* Client requests their current understanding status */
          case 'status':
            if (understandMap.get(clientUUID))
            {
              client.send("You understand.");
            }
            else
            {
              client.send("You are confused.");
            }
          case 'custom':
            client.send("[Server] You called the custom message!");
        }
      }
    })

    client.on('close', () => { 
      clientMap.delete(clientUUID);
      console.log('[Server] Client disconnected (Total: %d)', wss.clients.size);
      console.log('Clients Size: %d', clientMap.size);

      /* If the webpage has registered, notify of webpage client disconnection */
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
function updateWebpage(status)
{
  if (clientMap.has(WEB_UUID)) {
    clientMap.get(WEB_UUID).send(JSON.stringify({
      type: status,
      status: understandStatus(),
      size: wss.clients.size
    })) 
  }
}