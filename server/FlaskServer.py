from flask import Flask, render_template
from flask_socketio import SocketIO, send
import os


app = Flask(__name__)
app.config['SECRET-KEY'] = os.urandom(24).hex()
socketio = SocketIO(app)


@app.route('/')
def index():
    return render_template('index.html')


@socketio.on('message')
def handle_message(msg):
    send(msg, broadcast=True)


if __name__ == '__main__':
    socketio.run(app, host='127.0.0.1', port=5000)

