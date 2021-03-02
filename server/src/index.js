import express from 'express';
import ws from 'ws';
import http from 'http';
import { v4 as uuidv4 } from 'uuid';
//import products from "./example.js"; // TODO: Remove

const port = 8080;
const app = express();
const httpServer = http.createServer(app);
const wss = new ws.Server({ server: httpServer });
let clientMap = new Map(); // Unique IDs for currently connected clients
var clientUUID; // Holds the UUID for the client
var otherUUIDD = null;

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

  wss.on('connection', client => {
    clientUUID = uuidv4(); // Store UUID for this client

    if (wss.clients.size === 1)
    {
      otherUUIDD = clientUUID;
      client.send("i see you");
    }

    clientMap.set(clientUUID, client);
    console.log('[Server] Client %d connected, UUID=%s', wss.clients.size, clientUUID);

    client.on ('message', data => {

      if (data === 'website')
      {
        clientMap.set(client, "page")
      }
      else if (data === requestuuid)
      {
        client.send(clientUUID);
      }

      console.log('[Server] Recieved: %s', data);
    })

    client.on('close', () => { 
      clientMap.delete(clientUUID);
      console.log('[Server] Client disconnected (Total: %d)', wss.clients.size);
      console.log('Clients Size: %d', clientMap.size);
    })
  })

  httpServer.listen(port);
})();