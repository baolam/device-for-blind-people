const express = require("express");
const http = require("http");
const ip = require("ip");
const path =require("path");
const socektio = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = new socektio.Server(server, {
  allowEIO3 : true,
  cors : { origin : "*" }
});

const PORT = process.env.PORT || 3000;
const SPATH = `${ip.address()}`;

app.use(express.json());
app.use(express.urlencoded({ extended : false }));
app.use(express.static(path.join(__dirname, "static")));

app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "index.html"));
});

app.get("/test", (req, res) => {
  res.status(200).send("OK");
});

const rasp = io.of("/rasp");
const user = io.of("/user");

rasp.on("connection", (_socket) => {
  console.log("Raspberry pi đã kết nối");
  _socket.emit("receive-value", { ip : SPATH });
  _socket.on("disconnect", () => {
    console.log("Raspberry pi đã ngắt kết nối");
  });
  _socket.on("notification", (d) => {
    user.emit("notification", d);
  })
});

user.on("connection", (_socket) => {
  _socket.on("result", (infor) => {
    rasp.emit("recognize", infor);
  });
});

server.listen(PORT ,() => {
  console.log(`Server is listening at ${SPATH}`);
})