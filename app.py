from os import environ

from flask import Flask, render_template, request, jsonify
import simulator

app = Flask(__name__)

app.debug = environ.get("DEBUG")


@app.route('/', methods=['GET'])
def index():
    s, p, d, f, slicer = (request.args.get('s'), request.args.get('p'), request.args.get('d'), request.args.get('f'),
                          request.args.get('slice'))
    if s is None:
        s = 0
    elif not isinstance(s, int):
        s = int(s)
    if p is None:
        p = 0
    elif not isinstance(p, int):
        p = int(p)
    if d is None:
        d = 0
    elif not isinstance(d, int):
        d = int(d)
    if f is None:
        f = 0
    elif not isinstance(f, int):
        f = int(f)
    if slicer is None:
        slicer = 0
    elif not isinstance(slicer, int):
        slicer = int(slicer)
    sim = simulator.Simulator(s, p, d, f)
    return render_template(
        "index.html",
        s=s, p=p, d=d, f=f,
        sim=sim,
        slice=slicer
    )


@app.route('/api')
def api():
    s, p, d, f, range_start, range_end = (int(request.args.get('s')), int(request.args.get('p')),
                                          int(request.args.get('d')), int(request.args.get('f')),
                                          int(request.args.get('range_start')), int(request.args.get('range_end')))
    sim = simulator.Simulator(s, p, d, f)
    return jsonify(s=s, p=p, d=d, f=f, range_start=range_start, range_end=range_end, rows=list(sim))



if __name__ == '__main__':
    app.run()
