import express from "express";
import ws from "ws";
import http from "http";
import products from "./example.js";

const port = 8080;
const app = express();
const httpServer = http.createServer(app)
const wss = new ws.Server({ server: httpServer });

// Request, response callback function
// app.get("/", (req, res) => {
//   //res.send("Hello world");
//   res.json(products);
// })

app.get("/clients", (req, res) => {
  return res.status(200).json({
    num_clients: wss.clients.size
  });
});

wss.on("connection", ws => {
  console.log("New client connected");

  ws.on ("message", data => {
    console.log('Recieved: %s', data);
    ws.send(data.toUpperCase());
  })
  ws.on("close", () => {
    console.log("Client has disconnected");
  })
})

httpServer.listen(port);