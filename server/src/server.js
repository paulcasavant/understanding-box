// server.js

const WebSocket = require('ws');
const http = require("http");
const express = require("express");
const path = require("path");

(function main() {
  const app = express();
  const httpServer = http.createServer(app);
  const wss = new WebSocket.Server({ server: httpServer });

  app
    .use(express.static(path.resolve("./public")))
    .get("/clients", (req, res) => {
      return res.status(200).json({
        num_clients: wss.clients.size
      });
    });

  // setup websocket handlers
  wss.on('connection', function connection(ws) {
    ws.on('message', function incoming(message) {
      console.log('received: %s', message);

      Array.from(wss.clients.values()).forEach((client) => {
        client.send(message);
      });
    });
  });

  // Listen on this port
  httpServer.listen(8080);

})();