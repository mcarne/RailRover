#!/usr/bin/env python3

import logging
from flask import Flask
import paho.mqtt.publish as publish

# levels: DEBUG, INFO, WARNING, ERROR, CRITICAL
logging.basicConfig(level=logging.DEBUG,
                    format="%(asctime)s:%(levelname)s:%(message)s")

BROKER = '192.168.178.116'
TOPIC = 'rr'

app = Flask(__name__)

HTML_PAGE="""<html>
   <head>
      <style>
        h1 { font-size: 60px;}
        p { font-size: 48px;}
      </style>
      <title>RailRover Control</title>
   </head>
   <body>
      <h1>RailRover Control</h1>
      <p><a href="/rr/f">FORWARD</a></p>
      <p><a href="/rr/h">HALT</a></p>
      <p><a href="/rr/r">REVERSE</a></p>
      <p><a href="/rr/t">Test</a></p>

   </body>
</html>"""

@app.route('/')
def index():
    return HTML_PAGE

@app.route('/rr/<command>')
def rover_command(command):
    if (command=='f'):
        publish.single(TOPIC, payload='f', hostname=BROKER)
        return HTML_PAGE
    elif (command=='r'):
        publish.single(TOPIC, payload='r', hostname=BROKER)
        return HTML_PAGE
    elif (command == 'h'):
        publish.single(TOPIC, payload='h', hostname=BROKER)
        return  HTML_PAGE
    elif (command == 't'):
        publish.single(TOPIC, payload='t', hostname=BROKER)
        return  HTML_PAGE
    else:
        return "unknown command"