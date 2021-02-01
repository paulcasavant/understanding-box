from flask import Flask, render_template
from flask_socketio import SocketIO, send

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
app.config['DEBUG'] = True
socketio = SocketIO(app)

# @socket.on('message')
# def handle_message(data):
#     print('received message: ' + data)


@app.route('/')
def index():
    return render_template('index.html')


@socketio.on('message')
def receive_message(message):
    print('#########: {}'.format(message))
    send('This is a message from Flask.')


@socketio.on('custom event')
def receive_custom_event(message):
    print('THE CUSTOM MESSAGE IS {}', message)


# @app.route('/cakes')
# def cakes():
#     return render_template('other.html')


if __name__ == '__main__':
    socketio.run(app)
