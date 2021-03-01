// server.js

const WebSocket = require('ws');
const http = require("http");
const express = require("express");
const path = require("path");

(function main() {
  const app = express();
  const httpServer = http.createServer(app);
  const wss = new WebSocket.Server({ server: httpServer });
  CLIENTS=[];
  
  app
    .use(express.static(path.resolve("./public")))
    .get("/clients", (req, res) => {
      return res.status(200).json({
        num_clients: wss.clients.size
      });
    });


  wss.on('connection', function(ws) {
      CLIENTS.push(ws);
      ws.on('message', function(message) {
          console.log('received: %s', message);
          sendAll(message);
      });
      ws.send("NEW USER JOINED");
  });

  function sendAll (message) {
      for (var i=0; i<CLIENTS.length; i++) {
          CLIENTS[i].send("Message: " + message);
      }
  }

  // // setup websocket handlers
  // wss.on('connection', function connection(ws) {
  //   ws.on('message', function incoming(message) {
  //     console.log('received: %s', message);

  //     if (message === '')
  //     {
  //       console.log('it is paul');
  //     }

  //     // Array.from(wss.clients.values()).forEach((client) => {
  //     //   client.send(message);
  //     // });
  //   });
  // });

  // Listen on this port
  httpServer.listen(8080);

})();