var socket = io.connect('127.0.0.1:5000')

socket.on('connect', function()
{
    socket.send('I am now connected!');
    socket.emit('custom event', 'The custom event message!');
    socket.on('message', function(msg)
    {
        alert(msg);
    });
});