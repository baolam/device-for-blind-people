const express = require("express");
const http = require("http");
const path = require("path");

const app = express();
const server = http.createServer(app);

const PORT = process.env.PORT || 3001;

app.use(express.json());
app.use(express.urlencoded({ extended : false }));

app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "index.html"));
});

server.listen(PORT, () => {
  console.log('Server is listening on port ' + PORT);
});