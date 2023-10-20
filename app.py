from flask import Flask, render_template, request, jsonify
import simulator


class IncompleteInput(Exception):
    def __str__(self):
        return "The input is incomplete. `s`, `p`, `d` and `f` have to be filled in."


app = Flask(__name__)


@app.route('/', methods=['GET'])
def index():
    return render_template("index.html")


@app.route('/api/simulator', methods=['POST'])
def api_simulator():
    data = request.json.get('data')
    try:
        s, p, d, f, range_start, range_end = (int(data['s']), int(data['p']), int(data['d']), int(data['f']),
                                              int(data['range_start']), int(data['range_end']))
    except UnboundLocalError:
        raise IncompleteInput

    sim = simulator.Simulator(s, p, d, f)
    terms = simulator.Groups(sim)
    return jsonify(
        s=sim.combinations.s, p=sim.combinations.p, d=sim.combinations.d, f=sim.combinations.f,
        range_start=range_start, range_end=range_end,
        rows=sim[range_start:range_end],
        rows_len=len(sim),
        terms=list(terms)
    )


if __name__ == '__main__':
    app.run()
