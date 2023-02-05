const express = require("express");
const http = require("http");
const path = require("path");
const socketio = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = new socketio.Server(server, {
  allowEIO3 : true,
  cors : { origin : "*" }
})

const PORT = process.env.PORT || 3001;

app.use(express.json());
app.use(express.urlencoded({ extended : false }));
app.use(express.static(path.join(__dirname, "static")));

app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "index.html"));
});

server.listen(PORT, () => {
  console.log('Server is listening on port ' + PORT);
});