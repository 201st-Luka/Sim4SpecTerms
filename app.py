from os import environ

from flask import Flask, render_template, request

app = Flask(__name__)

app.debug = environ.get("DEBUG")


@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template("index.html")


if __name__ == '__main__':
    app.run()
