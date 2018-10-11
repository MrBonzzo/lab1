from flask import Flask, render_template, request
from datetime import datetime
app = Flask(__name__)


sensors = []

@app.route("/")
def index():
    return render_template("index.html", sensors=sensors)

@app.route("/sensors/<sensor>")
def catch(sensor):
	if request.headers.get('User-Agent') == sensor:
		who = sensor
		time = str(now.hour) + ':' + str(now.minute) + ':' + str(now.second)
		sensors.append([who, time])
	return ''


if __name__ == "__main__":
	app.run(host='0.0.0.0', port=80)
