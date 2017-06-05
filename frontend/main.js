'use strict';

var express = require('express');
var bodyParser = require('body-parser');
var serveStatic = require('serve-static');
var http = require('http');
var path = require('path');
var app = express();
var pg = require('pg');

// create a config to configure both pooling behavior
// and client options
// note: all config is optional and the environment variables
// will be read if the config is not present
var config = {
  user: 'sensei', //env var: PGUSER
  database: 'sensei', //env var: PGDATABASE
  password: 'ohCou2rei9ouL0ae', //env var: PGPASSWORD
  host: 'localhost', // Server hosting the postgres database
  port: 5432, //env var: PGPORT
  max: 10, // max number of clients in the pool
  idleTimeoutMillis: 30000, // how long a client is allowed to remain idle before being closed
};

var pool = new pg.Pool(config);
var httpsrv = require('http').createServer(app);

app.set('port', 5654);
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));

app.use(serveStatic(path.join(__dirname, "public")));

httpsrv.listen(app.get('port'), function() {
    console.log("HTTP Server listening on port "+app.get("port"));
});

app.get("/bmedata", function(req,res) {
    var from = new Date(req.query["from"]*1000);
    var to = new Date(req.query["to"]*1000);
    pool.query("SELECT * FROM bmedata WHERE datetime > $1 and datetime < $2",[from,to],function(err,dbres) {
        if(err) {
            return console.error('SQL Error: ',err);
        }
        var answer = {type:"bmedata", success:"true", rows:dbres.rows};
        res.send(JSON.stringify(answer));
    });
});
