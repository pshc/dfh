express = require('express')
fs = require('fs')
util = require('util')

zmq_sock = require('zeromq').createSocket 'sub'
zmq_sock.bind 'tcp://127.0.0.1:6767', (err) ->
    if err then throw err
    zmq_sock.subscribe ''
    zmq_sock.on 'message', on_zmq_message


on_zmq_message = (msg) ->
    if not msg.length
        throw Error 'Empty message'
    msg = msg.toString 'base64'
    io.sockets.send msg

app = express.createServer()
app.use express.static require('path').join __dirname, '..', 'www'

index_html = fs.readFileSync 'index.html', 'UTF-8'
app.get '/', (req, resp) ->
    resp.send index_html

io = require('socket.io').listen app
#io.on 'connection', (client) ->

app.listen 8000
