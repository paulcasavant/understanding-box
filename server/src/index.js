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
    console.log('[Server] Client connected (Total: %d)', wss.clients.size);

    clientMap.set(uuidv4(), client);
    console.log('Clients Size: %d', clientMap.size);

    client.on ('message', data => {
      console.log('[Server] Recieved: %s', data);
      client.send(data.toUpperCase());
    })
    client.on('close', () => {
      console.log('Client has disconnected');
    })
  })

  httpServer.listen(port);
})();