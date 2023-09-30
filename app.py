from os import environ

from flask import Flask, render_template, request, jsonify
import simulator

app = Flask(__name__)

app.debug = environ.get("DEBUG")


@app.route('/', methods=['GET'])
def index():
    return render_template("index.html")


@app.route('/api/simulator')
def api_simulator():
    s, p, d, f, range_start, range_end = (int(request.args.get('s')), int(request.args.get('p')),
                                          int(request.args.get('d')), int(request.args.get('f')),
                                          int(request.args.get('range_start')), int(request.args.get('range_end')))
    sim = simulator.Simulator(s, p, d, f)
    return jsonify(s=s, p=p, d=d, f=f, range_start=range_start, range_end=range_end, rows=sim[range_start:range_end])


@app.route('/api/full')
def api_full():
    s, p, d, f, range_start, range_end = (int(request.args.get('s')), int(request.args.get('p')),
                                          int(request.args.get('d')), int(request.args.get('f')),
                                          int(request.args.get('range_start')), int(request.args.get('range_end')))
    sim = simulator.Simulator(s, p, d, f)

    return jsonify(
        s=s, p=p, d=d, f=f,
        range_start=range_start, range_end=range_end,
        rows=sim[range_start:range_end]
    )


if __name__ == '__main__':
    app.debug = True
    app.run()
