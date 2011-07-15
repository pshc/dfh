canvas = document.getElementsByTagName('canvas')[0]
context = canvas.getContext '2d'
context.fillRect 0, 0, 640, 400

socket = null

on_message = (message) ->
    console.log message
    for x in [0...80]
        for y in [0...25]
            tile = 3
            u = tile % 16
            v = Math.floor(tile / 16)
            context.drawImage tileset, u*16, v*16, 16, 16, x*16, y*16, 16, 16

tileset = new Image
tileset.src = 'curses.png'
tileset.onload = ->
    socket = io.connect()
    socket.on 'message', on_message
