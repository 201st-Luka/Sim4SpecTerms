from os import environ

from flask import Flask, render_template, request
import simulator

app = Flask(__name__)

app.debug = environ.get("DEBUG")


@app.route('/', methods=['GET'])
def index():
    s, p, d, f = request.args.get('s'), request.args.get('p'), request.args.get('d'), request.args.get('f')
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
    combinations = (simulator.combinations(s, 2),
                    simulator.combinations(p, 6),
                    simulator.combinations(d, 10),
                    simulator.combinations(f, 14))
    possibilities = (simulator.possibilities(combinations[0], s, 2),
                     simulator.possibilities(combinations[1], p, 6),
                     simulator.possibilities(combinations[2], d, 10),
                     simulator.possibilities(combinations[3], f, 14))
    simulator.econfig((1, 1, 1, 1), (1, 1, 1, 1))
    return render_template(
        "index.html",
        s=s, p=p, d=d, f=f,
        combinations=combinations,
        possibilities=(simulator.tf_poss(possibilities[0], 2),
                       simulator.tf_poss(possibilities[1], 6),
                       simulator.tf_poss(possibilities[2], 10),
                       simulator.tf_poss(possibilities[3], 14))
    )


if __name__ == '__main__':
    app.run()
